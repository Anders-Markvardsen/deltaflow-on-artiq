/*
This file contains a CPU reset handler.
This is useful for catching reset calls and decide whether to forward them to the CPU or not
2020 Marco Ghibaudi, Riverlane

V1: basic functionalities no redirection
*/
#ifndef __TLM_KERN_RST__
#define __TLM_KERN_RST__

#include <systemc>
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlms/commons/logging.hpp"

using namespace std;

SC_MODULE(TLM_KERN_RST) {

    tlm_utils::simple_target_socket<TLM_KERN_RST> dataBus;
    uint64_t base_address;
    uint64_t RST_OFFSET = 0x0;
    bool stop_sim;
    bool in_reset = true;

    TLM_KERN_RST(sc_module_name name, uint64_t base_address, bool stop_sim = true) {
        dataBus.register_b_transport(this, &TLM_KERN_RST::b_transport);
        this->base_address = base_address;
        this->stop_sim = stop_sim;
    };

    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    addr = trans.get_address();
        unsigned char*   ptr = trans.get_data_ptr();

        if (addr != (RST_OFFSET + base_address))
            SC_REPORT_ERROR("TLM_KERN_RST", "TLM_KERN_RST - only accesses to the RESET_ADDR are supported");

        if (cmd == tlm::TLM_WRITE_COMMAND){
            if (ptr[3] && 0x1) {
                std::cout << sc_time_stamp() << ": TLM_KERN_RST: the CPU tries to enter a restart phase " << std::endl;
                in_reset = true;
                if (stop_sim) {
                    SC_REPORT_ERROR("TLM_KERN_RST", "TLM_KERN_RST - Stopping simulation");
                }
            } else {
                in_reset = false;
                std::cout << sc_time_stamp() << " : TLM_KERN_RST: the CPU tries to exit the restart phase " << std::endl;
            }
        } else {
            memset(ptr, 0, 3);
            ptr[3] = (uint8_t) in_reset;
            std::cout << sc_time_stamp() << " : TLM_KERN_RST: returning reset status: " << in_reset << std::endl;
        }
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        return;
    }
};

#endif
