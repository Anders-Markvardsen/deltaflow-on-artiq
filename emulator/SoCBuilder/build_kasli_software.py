#!/usr/bin/env python3
"""File derived from ARTIQ artiq/artiq/gateware/targets/kasli.py

Used under GPL, copyright ARTIQ devs/M-Labs.

Modified to produce only the build system and binary blob for kasli.
"""

import argparse

from misoc.targets.kasli import (soc_kasli_args, soc_kasli_argdict)
from misoc.integration.builder import builder_args, builder_argdict

from artiq.build_soc import build_artiq_soc

import virtual_kasli

class FakeArgs(dict):

    def __getattr__(self, key):
        try:
            return self[key]
        except KeyError:
            return super().__getattr__(self, key)

def software_main(soc_cls):
    args = {"cpu_type":None, "csr_csv":None, "hw_rev":None,
            "integrated_rom_size":None, "no_compile_gateware":True,
            "no_compile_software":False,
            "output_dir":'build/artiq_kasli'}
    args = FakeArgs(args)
    print(args)

    kasli_argdict = soc_kasli_argdict(args)
    print("kasli args", kasli_argdict)
    soc = soc_cls(**kasli_argdict)
    build_argdict = builder_argdict(args)
    print("builder args", build_argdict)
    build_artiq_soc(soc, build_argdict)


if __name__ == "__main__":
    software_main(soc_cls=virtual_kasli.VirtualKasli)
