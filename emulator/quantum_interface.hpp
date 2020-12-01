/**
 * @file quantum_interface.hpp
 * @author Riverlane, 2020
 * @brief A simple bridge from I/O signals to quantum backends 
 */

#ifndef __QUANTUM_INTERFACE__
#define __QUANTUM_INTERFACE__

#include "systemc.h"
#include <sstream>

using namespace std;
using namespace sc_dt;

/**
 * A simple bridge from I/O signals to quantum backends 
*/
SC_MODULE(QUANTUM_INTERFACE) {

    sc_in<sc_bv<1>>  quantum_in_cmd;
    sc_in<sc_bv<1>>  quantum_in_valid;
    sc_out<sc_bv<32>> quantum_out_cmd;
    sc_out<sc_bv<1>> quantum_out_valid;

    uint32_t cmd, cnt = 0;

    void cmd_handler() {
        if (quantum_in_valid.read() == '1'){
            quantum_out_valid.write(0);
            if (cnt == 31){
                cmd |= (quantum_in_cmd.read().to_uint() << cnt);
                cnt = 0;
                quantum_out_cmd.write(cmd);
                quantum_out_valid.write(1);
                std::cout << std::setw(16) << sc_time_stamp() << " : QUANTUM_INTERFACE: <-- " << std::hex << cmd << std::endl;
            } else {
                cmd |= (quantum_in_cmd.read().to_uint() << cnt);
                cnt++;
            }
        }
    }

    QUANTUM_INTERFACE(sc_module_name name) : sc_module(name)
    {
        SC_METHOD(cmd_handler);
        sensitive << quantum_in_valid << quantum_in_cmd;
        dont_initialize();
    }

    SC_HAS_PROCESS(QUANTUM_INTERFACE);
};

#endif // __QUANTUM_INTERFACE__


