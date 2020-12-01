# This file is Copyright (c) 2015-2018 Florent Kermarrec <florent@enjoy-digital.fr>
# License: BSD

# This file is from liteeth, vendored here to allow the use of a bridge Phy
#  with a unmodified ARTIQ build enviroment.
# Migen vendors some of liteeth in migen.liteeth_mini, but not the model PHY.

from migen import *
from misoc.interconnect.csr import *
from misoc.cores.liteeth_mini.common import *
from misoc.interconnect import stream

class LiteEthPHYModelCRG(Module, AutoCSR):
    def __init__(self):
        self._reset = CSRStorage()

        # # #

        self.clock_domains.cd_eth_rx = ClockDomain()
        self.clock_domains.cd_eth_tx = ClockDomain()
        self.comb += [
            self.cd_eth_rx.clk.eq(ClockSignal()),
            self.cd_eth_tx.clk.eq(ClockSignal())
        ]

        reset = self._reset.storage
        self.comb += [
            self.cd_eth_rx.rst.eq(reset),
            self.cd_eth_tx.rst.eq(reset)
        ]


class LiteEthPHYModel(Module, AutoCSR):
    dw = 8
    def __init__(self, pads):
        self.submodules.crg = LiteEthPHYModelCRG()
        self.sink = sink = stream.Endpoint(eth_phy_layout(8))
        self.source = source = stream.Endpoint(eth_phy_layout(8))

        # import code; code.interact(local=locals())
        self.comb += [
            pads.source_valid.eq(self.sink.stb),
            pads.source_data.eq(self.sink.data),
            self.sink.ack.eq(1)
        ]

        self.sync += [
            self.source.stb.eq(pads.sink_valid),
            self.source.data.eq(pads.sink_data),
        ]
        self.comb += [
            self.source.eop.eq(~pads.sink_valid & self.source.stb),
        ]
