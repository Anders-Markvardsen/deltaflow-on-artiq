#!/usr/bin/env python3

import argparse

from migen import *
from migen.build.generic_platform import *
from migen.build.xilinx.vivado import XilinxVivadoToolchain
from migen.build.xilinx.ise import XilinxISEToolchain

from misoc.interconnect import wishbone
from misoc.interconnect.csr import *
from misoc.cores import gpio
from misoc.integration.soc_sdram import SoCSDRAM
from misoc.cores.liteeth_mini.mac import LiteEthMAC
from migen.build.platforms.sinara import kasli
from misoc.cores import spi_flash
from misoc.interconnect import stream
from misoc.cores.liteeth_mini.common import eth_phy_layout

from artiq.gateware import rtio
from artiq.gateware.amp import AMPSoC

from misoc.cores.sdram_phy.gensdrphy import GENSDRPHY
from misoc.cores.sdram_settings import MT48LC4M16
from misoc.cores.mor1kx.core import MOR1KX
from artiq.gateware.rtio.phy import ttl_simple
# from artiq.gateware.rtio.phy import spi2 as spi
from artiq.build_soc import *
import virtual_platform
from liteeth_model import LiteEthPHYModel
from misoc.interconnect.dfi import *
from misoc.cores import sdram_settings

from artiq_hal_interface import ArtiqHALInterface

class Sniffer(Module):
    """Sniffes a bus."""
    def __init__(self, pads, bus, **kwargs):
        for pad_name,_ in pads.layout:
            self.comb += [getattr(pads, pad_name).eq(getattr(bus, pad_name))]

class BridgeCPU(Module):
    """Bridges a wb port and inturrupts out to a higher CPU ISA model.

    _layout = [
        ("adr",             30, DIR_M_TO_S),
        ("dat_w", "data_width", DIR_M_TO_S),
        ("dat_r", "data_width", DIR_S_TO_M),
        ("sel",    "sel_width", DIR_M_TO_S),
        ("cyc",              1, DIR_M_TO_S),
        ("stb",              1, DIR_M_TO_S),
        ("ack",              1, DIR_S_TO_M),
        ("we",               1, DIR_M_TO_S),
        ("cti",              3, DIR_M_TO_S),
        ("bte",              2, DIR_M_TO_S),
        ("err",              1, DIR_S_TO_M)
    ]
    """
    data_width = 32
    sel_width = data_width//8

    def __init__(self, platform, **kwargs):
        self.dbus = d = wishbone.Interface()
        self.interrupt = Signal(32)
        int_pads, wb_tgt_pads = platform.request(
            "irq"), platform.request("wishbone")

        self.comb += [int_pads.eq(self.interrupt)]
        for (pad_name, width, direction) in wishbone._layout:
            if direction == DIR_M_TO_S:
                # from init (external) to tgt (us)
                self.comb += [getattr(d,
                                      pad_name).eq(getattr(wb_tgt_pads, pad_name))]
            else:
                self.comb += [getattr(wb_tgt_pads,
                                      pad_name).eq(getattr(d, pad_name))]


class BridgeDFI(Module, AutoCSR):
    def __init__(self, pads):
        addressbits = len(pads.a)
        bankbits = len(pads.ba)
        databits = len(pads.dq_i)
        assert (len(pads.dq_i) == len(pads.dq_o))
        read_latency = 4
        self.settings = sdram_settings.PhySettings(
            memtype="SDR",
            dfi_databits=databits,
            nphases=1,
            rdphase=0,
            wrphase=0,
            rdcmdphase=0,
            wrcmdphase=0,
            cl=2,
            read_latency=read_latency,
            write_latency=0
        )

        self.dfi = Interface(addressbits, bankbits, databits)
           #
        # Command/address
        #
        self.comb += [
            pads.a.eq(self.dfi.p0.address),
            pads.ba.eq(self.dfi.p0.bank),
            pads.cke.eq(self.dfi.p0.cke),
            pads.cas_n.eq(self.dfi.p0.cas_n),
            pads.ras_n.eq(self.dfi.p0.ras_n),
            pads.we_n.eq(self.dfi.p0.we_n)
            ]
        if hasattr(pads, "cs_n"):
            self.sync += pads.cs_n.eq(self.dfi.p0.cs_n)

        #
        # DQ/DQS/DM data
        #
        self.comb += pads.dq_o.eq(self.dfi.p0.wrdata)
        self.comb += \
            If(self.dfi.p0.wrdata_en,
                pads.dm.eq(self.dfi.p0.wrdata_mask)
            ).Else(
                pads.dm.eq(0)
            )
        sd_dq_in_ps = Signal(databits)
        self.comb += sd_dq_in_ps.eq(pads.dq_i)
        self.comb += self.dfi.p0.rddata.eq(sd_dq_in_ps)

        #
        # DQ/DM control
        #
        d_dfi_wrdata_en = Signal()
        self.comb += d_dfi_wrdata_en.eq(self.dfi.p0.wrdata_en)
        self.comb += pads.dq_oe.eq(d_dfi_wrdata_en)

        rddata_sr = Signal(read_latency)
        self.comb += self.dfi.p0.rddata_valid.eq(rddata_sr[read_latency-1])
        self.sync += rddata_sr.eq(Cat(self.dfi.p0.rddata_en, rddata_sr[:read_latency-1]))

        # These registers are read always by the kernel code - but in reality they have no use
        # for our pass-through model
        self._en_vtc = CSRStorage(reset=1)
        self._wlevel_en = CSRStorage()
        self._wlevel_strobe = CSR()
        self._dly_sel = CSRStorage(32//8)
        self._rdly_dq_rst = CSR()
        self._rdly_dq_inc = CSR()
        self._rdly_dq_bitslip = CSR()
        self._wdly_dq_rst = CSR()
        self._wdly_dq_inc = CSR()
        self._wdly_dqs_rst = CSR()
        self._wdly_dqs_inc = CSR()
        self._wdly_dqs_taps = CSRStatus(9)


class _RTIOCRG(Module, AutoCSR):
    def __init__(self, platform, rtio_internal_clk):
        self._clock_sel = CSRStorage()
        self._pll_reset = CSRStorage(reset=1)
        self._pll_locked = CSRStatus()
        self.clock_domains.cd_rtio = ClockDomain()
        self.clock_domains.cd_rtiox4 = ClockDomain(reset_less=True)

        self.comb += [ self._pll_locked.status.eq(1)]



class _KasliCRG(Module):
    def __init__(self, platform):
        self.clock_domains.cd_sys = ClockDomain()
        # self.clock_domains.cd_sys4x = ClockDomain(reset_less=True)
        # self.clock_domains.cd_sys4x_dqs = ClockDomain(reset_less=True)
        # self.clock_domains.cd_clk200 = ClockDomain()
        self.clock_domains.cd_sys_ps = ClockDomain()

        # clk125 = platform.request("clk125_gtp")

        self.comb += [
            self.cd_sys.clk.eq(platform.request("sysclk")),
            self.cd_sys_ps.clk.eq(self.cd_sys.clk)
            # self.cd_sys4x.clk.eq(platform.request("sysclk4x")),
            # self.cd_sys4x_dqs.clk.eq(platform.request("sys4x_dqs")),
            # self.cd_clk200.clk.eq(platform.request("clk200")),
        ]


class VirtualBaseSoC(SoCSDRAM):
    def __init__(self, sdram_controller_type="minicon", hw_rev=None,
                 **kwargs):
        if hw_rev is None:
            hw_rev = "v1.0"
        platform = kasli.Platform(hw_rev=hw_rev)
        platform.add_extension(virtual_platform._io)

        SoCSDRAM.__init__(self, platform,
                          clk_freq=125e6*14.5/16, cpu_reset_address=0x400000,
                          **kwargs)

        self.remove_subsystem_by_type("MOR1KX")
        self.submodules.cpu = bridge_cpu = BridgeCPU(platform=platform)
        self.add_wb_master(bridge_cpu.dbus)

        self.config["HW_REV"] = hw_rev
        self.submodules.crg = _KasliCRG(platform)

        self.submodules.ddrphy = BridgeDFI(
            pads=self.platform.request("virt_ddr"))
        sdram_module = MT48LC4M16(self.clk_freq, "1:1")
        self.register_sdram(self.ddrphy, sdram_controller_type,
                            sdram_module.geom_settings, sdram_module.timing_settings)
        self.csr_devices.append("ddrphy")

        if not self.integrated_rom_size:
            spiflash_pads = self.platform.request("virt_spiflash")
            spiflash_dummy_clks = 3
            self.submodules.spiflash = spi_flash.SpiFlash(
                spiflash_pads, dummy=spiflash_dummy_clks, div=2)
            self.config["SPIFLASH_DUMMY_CLKS"] = spiflash_dummy_clks
            self.config["SPIFLASH_PAGE_SIZE"] = 256
            self.config["SPIFLASH_SECTOR_SIZE"] = 0x10000
            self.flash_boot_address = 0xb40000
            self.register_rom(self.spiflash.bus, 16*1024*1024)
            self.csr_devices.append("spiflash")

    def remove_subsystem_by_type(self, type_filter):
        for elem, val in self.submodules._fm._submodules:
            if val:
                if type_filter in str(type(val)):
                    print(
                        "remove_subsystem_by_type:: About to remove module : " + str(type(val)))
                    self.submodules._fm._submodules.remove((elem, val))

class VirtualMiniSoC(VirtualBaseSoC):
    mem_map = {
        "ethmac": 0x30000000,  # (shadow @0xb0000000)
    }
    mem_map.update(VirtualBaseSoC.mem_map)

    def __init__(self, *args, ethmac_nrxslots=2, ethmac_ntxslots=2, **kwargs):
        VirtualBaseSoC.__init__(self,
                                cpu_type="or1k",
                                sdram_controller_type="minicon",
                                l2_size=1024*16,
                                integrated_sram_size=8192,
                                **kwargs)
        self.csr_devices += ["ethmac", "ethphy"]
        self.interrupt_devices.append("ethmac")
        # TODO: add pads
        ethphy = LiteEthPHYModel(pads=self.platform.request("model_eth"))
        self.submodules.ethphy = ethphy
        self.submodules.ethmac = LiteEthMAC(
            phy=self.ethphy, dw=32, interface="wishbone",
            nrxslots=ethmac_nrxslots, ntxslots=ethmac_ntxslots)
        ethmac_len = (ethmac_nrxslots + ethmac_ntxslots) * 0x800
        self.add_wb_slave(self.mem_map["ethmac"], ethmac_len, self.ethmac.bus)
        self.add_memory_region("ethmac",
                               self.mem_map["ethmac"] | self.shadow_base, ethmac_len)


class _StandaloneBase(VirtualMiniSoC, AMPSoC):
    mem_map = {
        "cri_con":       0x10000000,
        "hal":          0x18000000,
        "rtio":          0x20000000,
        "rtio_dma":      0x30000000,
        "mailbox":       0x70000000
    }
    mem_map.update(VirtualMiniSoC.mem_map)

    def __init__(self, **kwargs):
        VirtualMiniSoC.__init__(self,
                                ethmac_nrxslots=4,
                                ethmac_ntxslots=4,
                                **kwargs)
        AMPSoC.__init__(self)
        add_identifier(self)
        self.config["HAS_DDS"] = None
        self.clock_domains.rtio = ClockDomain()
        self.comb += self.rtio.clk.eq(ClockSignal("sys"))

        self.submodules.rtio_tsc = rtio.TSC("async", glbl_fine_ts_width=3)



    def add_rtio(self, rtio_channels):
        self.submodules.rtio_crg = _RTIOCRG(self.platform, self.crg.cd_sys.clk)
        self.csr_devices.append("rtio_crg")
        self.config["HAS_RTIO_CLOCK_SWITCH"] = None
        self.submodules.rtio_tsc = rtio.TSC("async", glbl_fine_ts_width=3)
        self.submodules.rtio_core = rtio.Core(self.rtio_tsc, rtio_channels)
        #self.submodules.rtio_core = rtio.Core(rtio_channels)
        self.csr_devices.append("rtio_core")
        self.submodules.rtio = rtio.KernelInitiator(self.rtio_tsc)
        self.submodules.rtio_dma = ClockDomainsRenamer("sys_kernel")(
            rtio.DMA(self.get_native_sdram_if()))
        self.register_kernel_cpu_csrdevice("rtio")
        self.register_kernel_cpu_csrdevice("rtio_dma")
        self.submodules.cri_con = rtio.CRIInterconnectShared(
            [self.rtio.cri, self.rtio_dma.cri],
            [self.rtio_core.cri])
        self.register_kernel_cpu_csrdevice("cri_con")
        self.submodules.rtio_moninj = rtio.MonInj(rtio_channels)
        self.csr_devices.append("rtio_moninj")
        #self.submodules.rtio_analyzer = rtio.Analyzer(self.rtio_tsc,self.rtio_core.cri,
        #                                              self.get_native_sdram_if())
        #self.csr_devices.append("rtio_analyzer")


class Emulator(Module, AutoCSR):
    def __init__(self, pads):
        self.is_emulator = CSRStorage(reset=1)
        self.sdram_cp_from = CSRStorage(32)
        self.sdram_cp_to = CSRStorage(32)
        self.sdram_cp_size = CSRStorage(32)
        self.sdram_cp = CSRStorage()

        self.sdram_clear_from = CSRStorage(32)
        self.sdram_clear_to = CSRStorage(32)
        self.sdram_clear = CSRStorage()

        self.sync += [
            pads.copy_from.eq(self.sdram_cp_from.storage),
            pads.copy_to.eq(self.sdram_cp_to.storage),
            pads.copy_size.eq(self.sdram_cp_size.storage),
            pads.copy.eq(self.sdram_cp.re),

            pads.clear_from.eq(self.sdram_clear_from.storage),
            pads.clear_to.eq(self.sdram_clear_to.storage),
            pads.clear.eq(self.sdram_clear.re),
        ]


class VirtualKasli(_StandaloneBase):
    def __init__(self, **kwargs):
        _StandaloneBase.__init__(self, **kwargs)

        # Adding a sniffer on the kernel_cpu ibus
        self.submodules += Sniffer(self.platform.request("wishbone_kernel_ibus", 0), self.kernel_cpu.cpu.ibus)
        # Adding a sniffer on the kernel_cpu dbus
        self.submodules += Sniffer(self.platform.request("wishbone_kernel_dbus", 0), self.kernel_cpu.cpu.dbus)

        self.add_hal_interface()
        # RTIO Channels
        self.rtio_channels = []

        quantum_cmd_out = ttl_simple.Output(self.platform.request("quantum_cmd", 0))
        self.submodules += quantum_cmd_out
        quantum_valid_out = ttl_simple.Output(self.platform.request("quantum_valid", 0))
        self.submodules += quantum_valid_out

        self.rtio_channels.append(rtio.Channel.from_phy(quantum_cmd_out))
        self.rtio_channels.append(rtio.Channel.from_phy(quantum_valid_out))

        self.rtio_channels.append(rtio.LogChannel())
        self.config["RTIO_LOG_CHANNEL"] = len(self.rtio_channels)
        self.add_rtio(self.rtio_channels)
        # self.config["HAS_RTIO_LOG"] = None
        self.submodules.emulator = Emulator(pads=self.platform.request("sdram_backdoor"))
        self.csr_devices.append("emulator")

        # This value represents the start address of the <.callMain>:
        # function in the runtime.obj. The entry point 0x40000000 would
        # otherwise perform quite a long clearing of the .bss section that
        # is still clean in this case.
        self.config["second_stage_offset"] = 0x40001034

    def add_hal_interface(self):
        self.submodules.hal = ArtiqHALInterface()
        # Externalizing I/Os
        cmd, cmd_valid, cmd_ready = self.platform.request("cmd"), self.platform.request("cmd_valid"),  self.platform.request("cmd_ready")
        readout, readout_valid, readout_ready = self.platform.request("readout"), self.platform.request("readout_valid"), self.platform.request("readout_ready")

        self.comb += [self.hal.cmd.data.eq(cmd), self.hal.cmd.valid.eq(cmd_valid), cmd_ready.eq(self.hal.cmd.ready)]
        self.comb += [readout.eq(self.hal.readout.data), readout_valid.eq(self.hal.readout.valid), self.hal.readout.ready.eq(readout_ready)]
        # This block will be visible from the kernel side
        self.register_kernel_cpu_csrdevice("hal")
