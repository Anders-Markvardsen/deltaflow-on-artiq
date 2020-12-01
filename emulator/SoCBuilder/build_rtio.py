from artiq.gateware.rtio.phy import ttl_simple
from helpers import SimpleSocWithAWishboneMaster
from migen import Module, Signal
#from migen.build.generic_platform import Pins, Subsignal
from migen.fhdl.decorators import ClockDomainsRenamer
from artiq.gateware import rtio
from migen.genlib.resetsync import AsyncResetSynchronizer

class DummyAsyncResetSynchronizerImpl(Module):
     def __init__(self, cd, async_reset):
         # TODO: asynchronous set
         # This naive implementation has a minimum reset pulse
         # width requirement of one clock period in cd.
         self.comb += cd.rst.eq(async_reset)

class DummyAsyncResetSynchronizer:
     @staticmethod
     def lower(dr):
         return DummyAsyncResetSynchronizerImpl(dr.cd, dr.async_reset)




class WBPassThrough(Module):
        def __init__(self, dwidth=32, awidth=30):
            self.adr = Signal(awidth)
            self.dat_w = Signal(dwidth)
            self.dat_r = Signal(dwidth)
            self.we = Signal(1)
            self.sel = Signal(int(dwidth/8))
            self.stb = Signal(1)
            self.ack = Signal(1)
            self.cyc = Signal(1)
            self.cti = Signal(3)
            self.bte = Signal(2)
            self.err = Signal(1)

            from misoc.interconnect import wishbone
            self.bus = bus = wishbone.Interface()

            self.comb += (
                self.bus.adr.eq(self.adr),
                self.bus.dat_w.eq(self.dat_w),
                self.bus.we.eq(self.we),
                self.bus.sel.eq(self.sel),
                self.bus.stb.eq(self.stb),
                self.bus.cyc.eq(self.cyc),
                self.bus.cti.eq(self.cti),
                self.bus.bte.eq(self.bte),

                self.dat_r.eq(self.bus.dat_r),
                self.ack.eq(self.bus.ack),
                self.err.eq(self.bus.err),
            )

class RTIOWrapper(Module):
    def __init__(self):
        wb_passthrough = WBPassThrough()
        top = SimpleSocWithAWishboneMaster(wb_passthrough)
        top.csr_devices = []

        self.adr = Signal(30)
        self.dat_w = Signal(32)
        self.dat_r = Signal(32)
        self.sel = Signal(4)
        self.we = Signal(1)
        self.we = Signal(1)
        self.stb = Signal(1)
        self.ack = Signal(1)
        self.cyc = Signal(1)
        self.cti = Signal(3)
        self.bte = Signal(2)
        self.err = Signal(1)
        self.rtio_laser1 = Signal(1)
        self.rtio_laser2 = Signal(1)

        top.comb += (
           wb_passthrough.adr.eq(self.adr),
           wb_passthrough.dat_w.eq(self.dat_w),
           wb_passthrough.we.eq(self.we),
           wb_passthrough.stb.eq(self.stb),
           wb_passthrough.cyc.eq(self.cyc),
           wb_passthrough.cti.eq(self.cti),
           wb_passthrough.bte.eq(self.bte),
           wb_passthrough.sel.eq(self.sel),
           self.dat_r.eq(wb_passthrough.dat_r),
           self.ack.eq(wb_passthrough.ack),
           self.err.eq(wb_passthrough.err))

        self.inputs = [self.adr, self.dat_w, self.sel,  self.we, self.stb, self.cti, self.cyc, self.bte]
        self.outputs = [self.dat_r, self.ack, self.err, self.rtio_laser1, self.rtio_laser2]
        self.io = set(self.inputs) | set(self.outputs)

        rtio_channels = []

        # Number of lanes should be multiple of 2
        phy1 = ttl_simple.Output(self.rtio_laser1)
        top.submodules += phy1
        phy2 = ttl_simple.Output(self.rtio_laser2)
        top.submodules += phy2

        rtio_channels.append(rtio.Channel.from_phy(phy1, ofifo_depth=64))
        rtio_channels.append(rtio.Channel.from_phy(phy2, ofifo_depth=64))

        self.add_rtio(top, rtio_channels)

        overrides = {AsyncResetSynchronizer: DummyAsyncResetSynchronizer}

        from migen.fhdl.verilog import convert
        print(convert(top, self.io, name="rtio_wrapper", special_overrides=overrides), file=open("/workdir/build/rtio_wrapper.v", "w"))


    def add_rtio(self, top, rtio_channels):
        #self.submodules.rtio_crg = _RTIOCRG(self.platform, self.crg.cd_sys.clk)
        #self.csr_devices.append("rtio_crg")
        #self.config["HAS_RTIO_CLOCK_SWITCH"] = None
        top.submodules.rtio_tsc = rtio.TSC("sync", glbl_fine_ts_width=3)
        top.submodules.rtio_core = rtio.Core(top.rtio_tsc, rtio_channels, lane_count=2)
        #self.submodules.rtio_core = rtio.Core(rtio_channels)
        top.csr_devices.append("rtio_core")
        top.submodules.rtio = rtio.KernelInitiator(top.rtio_tsc)
        #self.submodules.rtio_dma = ClockDomainsRenamer("sys_kernel")(
        #    rtio.DMA(self.get_native_sdram_if()))
        #self.register_kernel_cpu_csrdevice("rtio")
        #self.register_kernel_cpu_csrdevice("rtio_dma")
        top.submodules.cri_con = rtio.CRIInterconnectShared(
            [top.rtio.cri], #self.rtio_dma.cri],
            [top.rtio_core.cri])
        #self.register_kernel_cpu_csrdevice("cri_con")
        top.submodules.rtio_moninj = rtio.MonInj(rtio_channels)
        top.csr_devices.append("rtio_moninj")

        #self.platform.add_period_constraint(self.rtio_crg.cd_rtio.clk, 8.)
        #self.platform.add_false_path_constraints(
        #    self.crg.cd_sys.clk,
        #    self.rtio_crg.cd_rtio.clk)

        #self.submodules.rtio_analyzer = rtio.Analyzer(self.rtio_tsc,self.rtio_core.cri,
        #                                              self.get_native_sdram_if())
        #self.csr_devices.append("rtio_analyzer")

rtio_model = RTIOWrapper()

