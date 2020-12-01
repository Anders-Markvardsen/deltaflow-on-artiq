#!/usr/bin/env python3
"""
First, will generate a CPU+peripheral in misoc, test that, and port to or1k.
"""


from litex.build.sim import SimPlatform
from litex.build.sim.verilator import SimVerilatorToolchain #, _compile_sim
from litex.build.generic_platform import Pins, Subsignal
from litex.build.io import CRG
from litex.soc.integration.soc_core import SoCMini

from liteeth.mac import LiteEthMAC
from liteeth.phy.model import LiteEthPHYModel

from migen import Module

name = "NoneCPUwLeftWB"

# external IO for a CPU core module.
_io = [
    ("sys_clock", 0, Pins(1)),
    ("sys_reset", 1, Pins(1)),

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

build_dir = f"/workdir/build/{name}"

class VirtualARTIQPlatform(SimPlatform):
    default_clk_name = "sys_clock"
    default_clk_period = 83.333

    def __init__(self, name, io):
        SimPlatform.__init__(self, name, io)

    def add_period_constraint(self, clk, period):
        pass

platform = VirtualARTIQPlatform(name, _io)

core_config = {
 'clk_freq': 125000000,
 'core': 'wishbone',
 'endianness': 'big',
 'soc': {'mem_map': dict()}
}

from misoc.integration.soc_core import SoCCore

"""
we want to make a SoCcore that just exports a wb bus, but is otherwise a drop-in
repalcement for this class

class SoCCore(Module):
    mem_map = {
        "rom":      0x00000000,
        "sram":     0x10000000,
        "main_ram": 0x40000000,
        "csr":      0x60000000,
    }
    def __init__(self, platform, clk_freq,
                cpu_type="lm32", cpu_reset_address=0x00000000,
                integrated_rom_size=0,
                integrated_sram_size=4096,
                integrated_main_ram_size=16*1024,
                shadow_base=0x80000000,
                csr_data_width=8, csr_address_width=14,
                with_uart=True, uart_baudrate=115200,
                ident="",
                with_timer=True):
"""

class SoCCore(Module):
    mem_map = {
        "rom":      0x00000000,
        "sram":     0x10000000,
        "main_ram": 0x40000000,
        "csr":      0x60000000,
    }
    def __init__(self, platform, clk_freq,
                cpu_type="lm32", cpu_reset_address=0x00000000,
                integrated_rom_size=0,
                integrated_sram_size=4096,
                integrated_main_ram_size=16*1024,
                shadow_base=0x80000000,
                csr_data_width=8, csr_address_width=14,
                with_uart=True, uart_baudrate=115200,
                ident="",
                with_timer=True):
        self.platform = platform
        self.clk_freq = clk_freq

        self.cpu_type = cpu_type
        if integrated_rom_size:
            cpu_reset_address = 0
        self.cpu_reset_address = cpu_reset_address

        self.integrated_rom_size = integrated_rom_size
        self.integrated_sram_size = integrated_sram_size
        self.integrated_main_ram_size = integrated_main_ram_size

        self.with_uart = with_uart
        self.uart_baudrate = uart_baudrate

        self.shadow_base = shadow_base

        self.csr_data_width = csr_data_width
        self.csr_address_width = csr_address_width

        self._memory_regions = []  # list of (name, origin, length)
        self._csr_regions = []  # list of (name, origin, busword, csr_list/Memory)
        self._constants = []  # list of (name, value)

        # and that should be done!
        # TODO: add a wb initiator that requests the wishbone IO :)
        self._wb_masters = []
        self._wb_slaves = WishboneSlaveManager(self.shadow_base)

        self.config = dict()

        self.csr_devices = [
            "uart_phy",
            "uart",
            "identifier",
            "timer0",
            "tmpu"
        ]
        self._memory_groups = []  # list of (group_name, (group_member0, group_member1, ...))
        self._csr_groups = []  # list of (group_name, (group_member0, group_member1, ...))
        self.interrupt_devices = []

        # need to add upstream initiator
        self.add_wb_master(TODO)


        if integrated_rom_size:
            # only need to propogate to the VP
            self.register_rom(self.rom.bus, integrated_rom_size)

        if integrated_sram_size:
            # only need to propogate to the VP
            self.register_mem("sram", self.mem_map["sram"], integrated_sram_size, self.sram.bus)

        self.submodules.wishbone2csr = wishbone2csr.WB2CSR(
            bus_csr=csr_bus.Interface(csr_data_width, csr_address_width))
        self.register_mem("csr", self.mem_map["csr"], 4*2**csr_address_width, self.wishbone2csr.wishbone)

        if with_uart:
            # need to ensure the SysC module turns up
            self.interrupt_devices.append("uart")

        self.config["CLOCK_FREQUENCY"] = int(clk_freq)
        self.config["SOC_PLATFORM"] = platform.name

        if with_timer:
            # use the upstream timer, no need to reinvent
            self.submodules.timer0 = timer.Timer()
            self.interrupt_devices.append("timer0")

    def add_wb_master(self, wbm):
        if self.finalized:
            raise FinalizeError
        self._wb_masters.append(wbm)

    def add_wb_slave(self, origin, length, interface):
        if self.finalized:
            raise FinalizeError
        self._wb_slaves.add(origin, length, interface)

    # This function simply registers the memory region for firmware purposes
    # (linker script, generated headers)
    def add_memory_region(self, name, origin, length):
        self._memory_regions.append((name, origin, length))

    def add_memory_group(self, group_name, members):
        self._memory_groups.append((group_name, members))

    def register_mem(self, name, origin, length, interface):
        self.add_wb_slave(origin, length, interface)
        self.add_memory_region(name, origin, length)

    def register_rom(self, interface, rom_size=0xa000):
        self.add_wb_slave(self.mem_map["rom"], rom_size, interface)
        assert self.cpu_reset_address < rom_size
        self.add_memory_region("rom", self.cpu_reset_address,
                               rom_size-self.cpu_reset_address)

    def get_memory_regions(self):
        return self._memory_regions

    def get_memory_groups(self):
        return self._memory_groups

    def check_csr_region(self, name, origin):
        for n, o, l, obj in self._csr_regions:
            if n == name or o == origin:
                raise ValueError("CSR region conflict between {} and {}".format(n, name))

    def add_csr_region(self, name, origin, busword, obj):
        self.check_csr_region(name, origin)
        self._csr_regions.append((name, origin, busword, obj))

    def add_csr_group(self, group_name, members):
        self._csr_groups.append((group_name, members))

    def get_csr_regions(self):
        return self._csr_regions

    def get_csr_groups(self):
        return self._csr_groups

    def get_constants(self):
        r = []
        for nr, name in enumerate(self.interrupt_devices):
            r.append((name.upper() + "_INTERRUPT", nr))
        r += self._constants
        return r

    def get_csr_dev_address(self, name, memory):
        if memory is not None:
            name = name + "_" + memory.name_override
        try:
            return self.csr_devices.index(name)
        except ValueError:
            return None

    def do_finalize(self):
        registered_mems = {regions[0] for regions in self._memory_regions}

        # Wishbone
        self.submodules.wishbonecon = wishbone.InterconnectShared(self._wb_masters,
            self._wb_slaves.get_interconnect_slaves(), register=True)

        # CSR
        self.submodules.csrbankarray = csr_bus.CSRBankArray(self,
            self.get_csr_dev_address,
            data_width=self.csr_data_width, address_width=self.csr_address_width)
        self.submodules.csrcon = csr_bus.Interconnect(
            self.wishbone2csr.csr, self.csrbankarray.get_buses())
        for name, csrs, mapaddr, rmap in self.csrbankarray.banks:
            self.add_csr_region(name, (self.mem_map["csr"] + 0x800*mapaddr) | self.shadow_base, self.csr_data_width, csrs)
        for name, memory, mapaddr, mmap in self.csrbankarray.srams:
            self.add_csr_region(name + "_" + memory.name_override, (self.mem_map["csr"] + 0x800*mapaddr) | self.shadow_base, self.csr_data_width, memory)
        for name, constant in self.csrbankarray.constants:
            self._constants.append(((name + "_" + constant.name).upper(), constant.value.value))
        for name, value in sorted(self.config.items(), key=itemgetter(0)):
            self._constants.append(("CONFIG_" + name.upper(), value))

        # Interrupts
        for nr, name in enumerate(self.interrupt_devices):
            self.comb += self.cpu.interrupt[nr].eq(getattr(self, name).ev.irq)

    def build(self, *args, **kwargs):
        self.platform.build(self, *args, **kwargs)


class BaseSoC(SoCCore):
    def __init__(self, platform, **kwargs):
        SoCCore.__init__(self, platform,
            clk_freq=int((1/(platform.default_clk_period))*1000000000),
            integrated_rom_size=0x8000,
            integrated_main_ram_size=16*1024,
            with_uart=False,
            **kwargs)
        self.submodules.crg = CRG(platform.request(platform.default_clk_name))

soc = BaseSoC(platform=platform)

verilator = SimVerilatorToolchain()
verilator.build(platform, soc.get_fragment(), build_name=name,
                run=False, build_dir=build_dir)

# os.chdir(build_dir)
# _compile_sim("liteeth", verbose=True)
