/* This file contains a porting of the identifier block.
2020 Tom Parks, Riverlane

V1: basic functionalities no redirection
*/
#ifndef __TLM_DBG__
#define __TLM_DBG__

#include <systemc>
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

using namespace std;


SC_MODULE(TLM_DBG) {

    // TLM cpu port
    tlm_utils::simple_target_socket<TLM_DBG> dataBus;
    tlm::tlm_response_status retstatus;
    int hit_count;

    TLM_DBG(sc_module_name name, tlm::tlm_response_status retstatus_) {
        dataBus.register_b_transport(this, &TLM_DBG::b_transport);
        retstatus = retstatus_;
        hit_count = 0;
    };

    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        tlm::tlm_command cmd = trans.get_command();
        // sc_dt::uint64    addr = trans.get_address() / 4;
        unsigned char*   ptr = trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();

        // cout << "WARNING: got a generic transaction to debug endpoint " << name() <<
        //         "\n" << tlm_info(trans) <<
        //         "\nreturning: " << retstatus <<
        //         "\nthis endpoint hit count: " << hit_count << endl;

        if (cmd == tlm::TLM_WRITE_COMMAND) {
            cout << "msg: ";
            for (size_t i=0; i<len; i++) {
                if (isprint(ptr[i])) {
                    cout << static_cast<char>(ptr[i]);
                }
            }
        }
        trans.set_response_status(retstatus);
        hit_count++;
        return;
    }
};

#endif //__TLM_DBG__

