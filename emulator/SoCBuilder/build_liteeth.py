from litex.build.sim import SimPlatform
from litex.build.sim.verilator import SimVerilatorToolchain #, _compile_sim
from litex.build.generic_platform import Pins, Subsignal
from litex.build.io import CRG
from litex.soc.integration.soc_core import SoCMini

from liteeth.mac import LiteEthMAC
from liteeth.phy.model import LiteEthPHYModel

from migen import Module

# external IO for the eth module.
_io = [
    ("sys_clock", 0, Pins(1)),
    ("sys_reset", 1, Pins(1)),

    ("interrupt_request", 0, Pins(1)),

    # model "PHY" pads
    ("model_eth", 0,
        Subsignal("source_valid",   Pins(1)),
        Subsignal("source_data",    Pins(8)),
        Subsignal("sink_valid",     Pins(1)),
        Subsignal("sink_data",   Pins(8),),
    ),

    # Wishbone
    ("wishbone", 0,
        Subsignal("adr",   Pins(30)),
        Subsignal("dat_r", Pins(32)),
        Subsignal("dat_w", Pins(32)),
        Subsignal("sel",   Pins(4)),
        Subsignal("cyc",   Pins(1)),
        Subsignal("stb",   Pins(1)),
        Subsignal("ack",   Pins(1)),
        Subsignal("we",    Pins(1)),
        Subsignal("cti",   Pins(3)),
        Subsignal("bte",   Pins(2)),
        Subsignal("err",   Pins(1))
    ),
]

build_dir = "/workdir/build/liteeth"

platform = SimPlatform("SIM", _io)

core_config = {
 'clk_freq': 125000000,
 'core': 'wishbone',
 'endianness': 'big',
 'soc': {'mem_map': {'ethmac': 805306368}}
}


class ModelMACCore(SoCMini):
    """This is a rough conglomeration of the LiteX eth MACCore and the
    model PHY, that the example does not support."""
    def __init__(self, platform, core_config):
        # # PHY --------------------------------------------------------------------------------------
        # PHYCore.__init__(self, platform, core_config)

        # SoC parameters ---------------------------------------------------------------------------
        soc_args = {}
        if "soc" in core_config:
            soc_config = core_config["soc"]

            for arg in soc_config:
                if arg in ("csr_map", "interrupt_map", "mem_map"):
                    getattr(self, arg).update(soc_config[arg])
                else:
                    soc_args[arg] = soc_config[arg]

        # SoCMini ----------------------------------------------------------------------------------
        SoCMini.__init__(self, platform, clk_freq=core_config["clk_freq"], **soc_args)

        # CRG --------------------------------------------------------------------------------------
        self.submodules.crg = CRG(platform.request("sys_clock"),
                                  platform.request("sys_reset"))

        # PHY  ----------------------------------------------------------------------------------
        ethphy = LiteEthPHYModel(pads=platform.request("model_eth"))
        self.submodules.ethphy = ethphy
        self.add_csr("ethphy")

        # MAC --------------------------------------------------------------------------------------
        self.submodules.ethmac = LiteEthMAC(
            phy        = self.ethphy,
            dw         = 32,
            interface  = "wishbone",
            endianness = core_config["endianness"])
        self.add_wb_slave(self.mem_map["ethmac"], self.ethmac.bus)
        self.add_memory_region("ethmac", self.mem_map["ethmac"], 0x2000, type="io")
        self.add_csr("ethmac")

        # Wishbone Interface -----------------------------------------------------------------------
        class _WishboneBridge(Module):
            def __init__(self, interface):
                self.wishbone = interface
                self.wishbone.data_width = 32

        bridge = _WishboneBridge(self.platform.request("wishbone"))
        self.submodules += bridge
        self.add_wb_master(bridge.wishbone)

        # Interrupt Interface ----------------------------------------------------------------------
        self.comb += self.platform.request("interrupt_request").eq(self.ethmac.ev.irq)
        #import code; code.interact(local=locals())

soc = ModelMACCore(platform, core_config)

verilator = SimVerilatorToolchain()
verilator.build(platform, soc.get_fragment(), build_name="liteeth",
                run=False, build_dir=build_dir)

# os.chdir(build_dir)
# _compile_sim("liteeth", verbose=True)
