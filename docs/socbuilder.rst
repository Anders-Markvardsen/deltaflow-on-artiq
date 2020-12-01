SoCBuilder
==========

This component defines a virtual platform (kasli-inspired) that uses the migen
flow to create both gateware and software outpus.

Components
----------

HAL Interface
+++++++++++++

- :code:`artiq_hal_interface.py` defines the bridge that exposes the Hardware
  Abstraction Layer to the outside world. This block receives HAL commands and return measurements.

Platform and configuration
++++++++++++++++++++++++++

- :code:`emulator/SoCBuilder/virtual_kasli.py`: defines the SoC in terms of
  connectivity and modules. It mimics the definition in
  :code:`misoc/misoc/targets/kasli.py` with the following differences:

  1. The data-bus of the boot-CPU is "externalized/exposed" to enable the
     connection to the or1ksim simulator.
     This allows fast execution and enhanced visibility on the CPU.

  2. The timing of the booting/configuration flash is boosted to accelerate
     the loading images.

  3. The SDRAM interface is simplified to allow fast loading/storing.
     Related to that, two backdoors are exposed to perform copies and clearing
     of SDRAM locations. A bank of CSR registers (and associated rust library
     in :code:`artiq/firmware/libriverlane/emulator.rs`) is used to enable
     these fast operations.

  4. The ethernet phy is reduced to a pass-through of the MAC layer interface.

- :code:`virtual_platform.py`: defines all the top-level I/Os that the SoC
  will expose.
  No real pinout assignment - the emulator does not need the concept of
  pin location.

Toolchain
+++++++++

- :code:`build_kasli_software.py`: orchestrates the build of both gateware
  and software. The gateware is not compiled by the default.

.. note::
    
  To compile for Xilinx targets we would need to include Vivado in the docker
  container.
  We haven't done it yet to reduce docker image size and initial setup speed. 
