/*
This file contains a basic adaptation of the misoc eth_phy to be used with TLM models
2020 Marco Ghibaudi, Riverlane
Release History:
V0.0.1: basic ETH_PHY functionality
*/
#ifndef __TLM_ETH_PHY__
#define __TLM_ETH_PHY__

#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include <sstream>

using namespace std;

SC_MODULE(TLM_ETH_PHY) {
    tlm_utils::simple_target_socket<TLM_ETH_PHY> dataBus;
    tlm::tlm_response_status retstatus;

    uint32_t base_address;

    TLM_ETH_PHY(sc_module_name name, uint32_t base_address) {
        dataBus.register_b_transport(this, &TLM_ETH_PHY::b_transport);
        this->base_address = base_address;
    };

    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        cout << "eth module received " << tlm_info(trans) << endl;
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        return;
    }
};

#endif



