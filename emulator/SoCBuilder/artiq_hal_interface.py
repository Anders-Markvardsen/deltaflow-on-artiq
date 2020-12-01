from migen import *
from migen.genlib.fifo import SyncFIFO
from misoc.interconnect.csr import *
from misoc.interconnect.csr_bus import *

class ArtiqHALInterface(Module, AutoCSR):

    def __init__(self, num_qubits=1, fifo_depth=32):

        self.cmd  = Record([
            ("data", 32, DIR_M_TO_S),
            ("valid", 1,DIR_M_TO_S),
            ("ready", 1, DIR_S_TO_M)
        ])
        self.readout = Record([
            ("data", 32, DIR_S_TO_M),
            ("valid", 1,DIR_S_TO_M),
            ("ready", 1, DIR_M_TO_S)
        ])

        self.fifo_depth = fifo_depth
        self.max_fifo_depth = self.fifo_depth - 2

        self.READOUT_MASK = 0
        for i in range(num_qubits):
            self.READOUT_MASK = self.READOUT_MASK | (1 << i)

        # Note: we can buffer up to 512 cmds (1 BRAM on Xilinx FPGAs). We are not implementing
        # any backpressure - i.e. if we receive too many commands we can overflow.
        # Note2: fwft - first word fall through.
        self.cmd_fifo = SyncFIFO(512, self.fifo_depth, fwft=True)
        self.submodules += self.cmd_fifo

         # CSR registers. We are enabling the write_from_dev because we want to be able to write these two registers
        # from our logic. For the cntr0 we want to write the next cmd, while for status0 we want to clear the field
        # after the CPU has written on it.
        self.cntr0 = CSRStorage(32, atomic_write=True, write_from_dev=True, name='hal_cntr0', reset=0)
        self.status0 = CSRStorage(32, atomic_write=True, write_from_dev=True, name='hal_status0')

        # Reads the cmd bus and storing it in a FIFO for later consumption
        self.sync += [
            If(self.cmd != 0,
                self.cmd_fifo.we.eq(self.cmd.valid),
                self.cmd_fifo.din.eq(self.cmd.data)
            )
        ]

        # Generate the cmd_ready flag
        self.comb += [
            If (self.cmd_fifo.level < self.max_fifo_depth,
                self.cmd.ready.eq(1)
            ).Else(
                self.cmd.ready.eq(0)
            )
        ]

        # Fetching the next cmd when a write to CNTR0 occurs + updates the content of CNTR0. Because
        # of the fwft mode we have a word ready before popping. Ignoring the readable flag as we
        # don't implement backpressure.
        self.sync += [
            If (self.cntr0.re,
                self.cmd_fifo.re.eq(1),
                self.cntr0.we.eq(1),
                self.cntr0.dat_w.eq(self.cmd_fifo.dout)
            ).Else (
                self.cmd_fifo.re.eq(0),
                self.cntr0.we.eq(0)
            )
        ]

        # Receiving a readout from the CPU and propagating it back to the outside world. Assumption here is that we receive all
        # the readouts from the different qubits with one CPU write.
        self.readout_fifo = SyncFIFO(num_qubits, self.fifo_depth, fwft=True)
        self.submodules += self.readout_fifo

        self.sync += [
            self.readout_fifo.we.eq(self.status0.re),
            self.readout_fifo.din.eq(self.status0.storage & self.READOUT_MASK)
        ]

        self.sync += [
            If (self.readout.ready & self.readout_fifo.readable,
                self.readout_fifo.re.eq(1),
                self.readout.valid.eq(1),
                self.readout.eq(self.readout_fifo.dout)
            ).Else(
                self.readout_fifo.re.eq(0),
                self.readout.valid.eq(0)
            )
        ]



