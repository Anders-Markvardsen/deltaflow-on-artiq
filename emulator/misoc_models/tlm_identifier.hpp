/* This file contains a porting of the identifier block.
2020 Tom Parks, Riverlane

V1: basic functionalities no redirection
*/
#ifndef __TLM_IDENTIFIER__
#define __TLM_IDENTIFIER__

#include <systemc>
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "deltamodels/tlms/commons/logging.hpp"

using namespace std;

SC_MODULE(TLM_IDENTIFIER) {

    tlm_utils::simple_target_socket<TLM_IDENTIFIER> dataBus;
    sc_dt::uint64 LOW = 0xe0001000;
    sc_dt::uint64 HIGH = 0xe0001004; // single byte

    // 0xe0001000: address to read from the memory
    // 0xe0001004: data to read from address

    size_t idx = 0;
    string ident{"TLM_dummy_identifier"};

    TLM_IDENTIFIER(sc_module_name name) {
        dataBus.register_b_transport(this, &TLM_IDENTIFIER::b_transport);
    };

    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    addr = trans.get_address();
        uint32_t*        ptr = (uint32_t *)trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();

        if (len != 4) {
            trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
            return;
        }
        if (cmd == tlm::TLM_WRITE_COMMAND && addr == 0xe0001000) {
            idx = __bswap_32(*ptr);
            // cout << "ident req read from idx " << idx << endl;
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
            return;
        }
        if (cmd == tlm::TLM_READ_COMMAND && addr == 0xe0001004) {
            // cout << "reading from idx " << idx << endl;
            if (idx == 0) {
                *ptr = __bswap_32(ident.size());
            }
            if (idx > 0) {
                *ptr = __bswap_32(ident[idx-1]);
            }
            trans.set_response_status(tlm::TLM_OK_RESPONSE);
            return;
        }
        cout << name() << " unknown txn " << tlm_info(trans) << endl;
        trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        return;
    };
};
#endif //__TLM_IDENTIFIER__

