/*
This file contains a basic adaptation of the misoc uart port to be used with TLM models
2020 Marco Ghibaudi, Riverlane
Release History:
V0.0.1: basic UART functionality
*/
#ifndef __TLM_UART__
#define __TLM_UART__

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include <sstream>
#include <vector>
#include <iomanip>  // std::setw

using namespace std;

SC_MODULE(TLM_UART) {
  // From misoc/misoc/cores/uart/core.py
  // self._rxtx = CSR(8)
  // self._txfull = CSRStatus() [Note: size is 1]
  // self._rxempty = CSRStatus() [Note: size is 1]
  // with generated csr.rs
  // pub const RXTX_ADDR: *mut u32 = 0xe0000800 as *mut u32;
  // pub const TXFULL_ADDR: *mut u32 = 0xe0000804 as *mut u32;
  // pub const RXEMPTY_ADDR: *mut u32 = 0xe0000808 as *mut u32;
  //
  // TLM cpu port
  tlm_utils::simple_target_socket<TLM_UART> dataBus;
  tlm::tlm_response_status retstatus;
  std::vector<unsigned char> buffer;

  const uint64_t BASE_ADDRESS;
  const uint64_t CSR_REGION_SIZE {0x10};

  const uint64_t LOW = BASE_ADDRESS;
  const uint64_t HIGH = BASE_ADDRESS + CSR_REGION_SIZE;

  TLM_UART(sc_module_name name, uint64_t base_address): BASE_ADDRESS(base_address) {
    dataBus.register_b_transport(this, &TLM_UART::b_transport);
    buffer = std::vector<unsigned char>();
  };

  static constexpr sc_dt::uint64 RXTX_ADDR {0x0};
  static constexpr sc_dt::uint64 TXFULL_ADDR {0x4};
  static constexpr sc_dt::uint64 RXEMPTY_ADDR {0x8};

 void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
    tlm::tlm_command cmd = trans.get_command();
    sc_dt::uint64    addr = trans.get_address();
    unsigned char*   ptr = trans.get_data_ptr();
    unsigned int     len = trans.get_data_length();

    if (((cmd != tlm::TLM_WRITE_COMMAND) && (cmd != tlm::TLM_READ_COMMAND)) || (len != 4)){
        SC_REPORT_FATAL("TLM-UART", "We only support 32 bits READ and WRITE commands");
    }
    addr = addr - BASE_ADDRESS;
    switch (addr){
        case TLM_UART::RXTX_ADDR:
            if (cmd == tlm::TLM_WRITE_COMMAND){
                buffer.push_back(ptr[3]); // 7downto0 have the char
                if (ptr[3] == '\n') {
                    string str = string{buffer.begin(), buffer.end()};
                    buffer.clear();
                    std::cout << std::setw(16) << sc_time_stamp() << " : ARTIQ_CONSOLE: " << str << flush;
                }
            } else {  
                SC_REPORT_FATAL("TLM-2", "Read to RXTX_ADDR - Failing only to catch this transaction");
            }
            break;
        case TLM_UART::TXFULL_ADDR:
            if (cmd == tlm::TLM_READ_COMMAND){
                SC_REPORT_INFO_VERB("TLM_UART", "Polling full on TX", SC_DEBUG);
                memset(ptr, 0, 4);
            } else {
                SC_REPORT_FATAL("TLM-UART", "Write to TXFULL_ADDR??");
            }
            break;
        case TLM_UART::RXEMPTY_ADDR:
            std::cout << "EMULATOR @" << sc_time_stamp() << ": TLM_UART: Received a request on RXEMPTY_ADDR" << std::endl;
            SC_REPORT_FATAL("TLM-2", "Write to RXEMPTY_ADDR - Failing only to catch this transaction");
            break;
        default:
            std::cout << "WARNING: got a transaction to endpoint " << name() << " that I do not understand." << std::endl;
            trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
            return;
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    return;
  }
};

#endif //__TLM_UART__
