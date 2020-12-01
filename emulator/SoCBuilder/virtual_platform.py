#!/usr/bin/env python3
"""File derived from ARTIQ artiq/artiq/gateware/targets/kasli.py

Used under GPL, copyright ARTIQ devs/M-Labs.

Modified to produce only the build system and binary blob for kasli.
"""

import argparse
from migen.build.platforms.sinara import kasli

from migen.build.xilinx import XilinxPlatform
from migen.build.generic_platform import Pins, Subsignal
def Pins_s(count):
    return Pins(" ".join(map(str, range(count))))

_io = [
    ("sysclk", 0, Pins_s(1)),
    ("sys_reset", 1, Pins_s(1)),

    ("clk", 0, Pins_s(1)),

    ("reset", 0, Pins_s(1)),
    ("rtiox4", 0, Pins_s(1)),

    ("irq", 0, Pins_s(32)),
    ("quantum_cmd", 0, Pins_s(1)),
    ("quantum_valid", 0, Pins_s(1)),

    # model "PHY" pads
    ("model_eth", 0,
        Subsignal("source_valid",   Pins_s(1)),
        Subsignal("source_data",    Pins_s(8)),
        Subsignal("sink_valid",     Pins_s(1)),
        Subsignal("sink_data",   Pins_s(8),),
    ),

    # Wishbone
    ("wishbone", 0,
        Subsignal("adr",   Pins_s(30)),
        Subsignal("dat_r", Pins_s(32)),
        Subsignal("dat_w", Pins_s(32)),
        Subsignal("sel",   Pins_s(4)),
        Subsignal("cyc",   Pins_s(1)),
        Subsignal("stb",   Pins_s(1)),
        Subsignal("ack",   Pins_s(1)),
        Subsignal("we",    Pins_s(1)),
        Subsignal("cti",   Pins_s(3)),
        Subsignal("bte",   Pins_s(2)),
        Subsignal("err",   Pins_s(1))
    ),

    # Kernel cpu ibus
    ("wishbone_kernel_ibus", 0,
        Subsignal("adr",   Pins_s(30)),
        Subsignal("dat_r", Pins_s(32)),
        Subsignal("ack",   Pins_s(1)),
    ),

    # Kernel cpu dbus
    ("wishbone_kernel_dbus", 0,
        Subsignal("adr",   Pins_s(30)),
        Subsignal("dat_r", Pins_s(32)),
        Subsignal("dat_w", Pins_s(32)),
        Subsignal("we", Pins_s(1)),
        Subsignal("ack",   Pins_s(1)),
    ),


    # Sdram backdoor
    ("sdram_backdoor", 0,
        Subsignal("copy_from",  Pins_s(32)),
        Subsignal("copy_to",    Pins_s(32)),
        Subsignal("copy_size",  Pins_s(32)),
        Subsignal("copy",       Pins_s(1)),
        Subsignal("clear_from", Pins_s(32)),
        Subsignal("clear_to",   Pins_s(32)),
        Subsignal("clear",      Pins_s(1))
    ),

    ("kernel_cpu_rst", 0, Pins_s(1)),

    ("cmd", 0, Pins_s(32)),
    ("cmd_valid", 0, Pins_s(1)),
    ("cmd_ready", 0, Pins_s(1)),

    ("readout", 0, Pins_s(32)),
    ("readout_valid", 0, Pins_s(1)),
    ("readout_ready", 0, Pins_s(1)),

    ("virt_spiflash", 0,
        Subsignal("clk", Pins_s(1)),
        Subsignal("cs_n", Pins_s(1)),
        Subsignal("dq", Pins_s(4)),
    ),

    ("virt_ddr", 0,
        Subsignal("cs_n", Pins_s(1)),
        Subsignal("a", Pins_s(12)),
        Subsignal("ba", Pins_s(2)),
        Subsignal("dq_i", Pins_s(32)),
        Subsignal("dq_o", Pins_s(32)),
        Subsignal("dq_oe", Pins_s(1)),
        Subsignal("dm", Pins_s(4)),
        Subsignal("cke", Pins_s(1)),
        Subsignal("ras_n", Pins_s(1)),
        Subsignal("cas_n", Pins_s(1)),
        Subsignal("we_n", Pins_s(1)),
    ),

    ("serial", 0,
        Subsignal("rx", Pins_s(1)),  # FPGA input, schematics TxD_2V5
        Subsignal("tx", Pins_s(1)),  # FPGA output, schematics RxD_2V5
    ),


]
