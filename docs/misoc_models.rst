MiSoC Models
============

These components provide a fast and simplified representation of some standard ancillary components of a generic ARTIQ platform.
They leverage the Transaction Level Model (TLM) methodology to provide fast execution time and enhanced debuggability. Note: models are not to be considered timing-representative.

Available Components:

- :code:`emulator/misoc_models/tlm_timer.hpp`: main timer used in the ARTIQ system, can be loaded and queried via a combination of CSR registers. Passage of time is computed via queries to the sc_time_stamp() function.

- :code:`emulator/misoc_models/tlm_identifier.hpp`: block that returns the identifier code of the system

- :code:`emulator/misoc_models/tlm_uart.hpp`: console/logging block. It converts transactions to the Uart block into direct output to the console. 

- :code:`emulator/misoc_models/tlm_kern_reset.hpp`: externalized version of the kernel_cpu reset block. Useful for debugging/investigations.

- :code:`emulator/misoc_models/tlm_eth_phy.hpp`: simplified version of the ethernet phy control block. Mainly used for mimicking accesses to the phy layer of the ethernet.

- :code:`emulator/misoc_models/tlm_dbg.hpp`: block used for helping debugging tlm transactions (by listing type, origin, destination etc).
