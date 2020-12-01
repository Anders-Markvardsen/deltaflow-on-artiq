/*
This file contains a generic TLM read-only memory.
This is useful for mapping a binary file into the address space of a virtual cpu.

2020 Marco Ghibaudi, Riverlane

V1: basic ROM functionality
*/
#ifndef __TLM_TIMER__
#define __TLM_TIMER__

#include <systemc.h>
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

SC_MODULE(TLM_TIMER) {
    /*
         pub const LOAD_ADDR: *mut u32 = 0xe0001800 as *mut u32;
         pub const LOAD_SIZE: usize = 8;

         pub const RELOAD_ADDR: *mut u32 = 0xe0001820 as *mut u32;
         pub const RELOAD_SIZE: usize = 8;

         pub const EN_ADDR: *mut u32 = 0xe0001840 as *mut u32;
         pub const EN_SIZE: usize = 1;

         pub const UPDATE_VALUE_ADDR: *mut u32 = 0xe0001844 as *mut u32;
         pub const UPDATE_VALUE_SIZE: usize = 1;

         pub const VALUE_ADDR: *mut u32 = 0xe0001848 as *mut u32;
         pub const VALUE_SIZE: usize = 8;

         pub const EV_STATUS_ADDR: *mut u32 = 0xe0001868 as *mut u32;
         pub const EV_STATUS_SIZE: usize = 1;

         pub const EV_PENDING_ADDR: *mut u32 = 0xe000186c as *mut u32;
         pub const EV_PENDING_SIZE: usize = 1;

         pub const EV_ENABLE_ADDR: *mut u32 = 0xe0001870 as *mut u32;
         pub const EV_ENABLE_SIZE: usize = 1;

    */
    tlm_utils::simple_target_socket<TLM_TIMER> dataBus;

    const uint64_t BASE_ADDR;

    static constexpr sc_dt::uint64 LOAD_ADDR = 0x0;
    static constexpr sc_dt::uint64 LOAD_ADDR_UPPER = 0x1C;
    static constexpr sc_dt::uint64 RELOAD_ADDR = 0x20;
    static constexpr sc_dt::uint64 RELOAD_ADDR_UPPER = 0x3C ;

    static constexpr sc_dt::uint64 EN_ADDR = 0x40;
    static constexpr sc_dt::uint64 UPDATE_VALUE_ADDR = 0x44;
    static constexpr sc_dt::uint64 VALUE_ADDR = 0x48;
    static constexpr sc_dt::uint64 VALUE_ADDR_UPPER = 0x64;
    static constexpr sc_dt::uint64 EV_STATUS_ADDR = 0x68;
    static constexpr sc_dt::uint64 EV_PENDING_ADDR = 0x6C;
    static constexpr sc_dt::uint64 EV_ENABLE_ADDR = 0x70;
    const double HZ_TO_PS =  1000000000000.0; 

    sc_time time_reg;
    sc_time delta;
    sc_time cycletime;
    uint64_t cycles = 0;
    uint64_t value = 0;

    uint64_t reload_value = 0;
    uint64_t load_value = 0;
    uint8_t load_regs [8];
    uint8_t reload_regs [8];
    uint8_t value_regs [8];
    bool enable = false;
    bool debug;

    TLM_TIMER(sc_module_name name, uint64 base_addr, uint64_t clock_freq_Hz=100000000, bool debug=false): 
        BASE_ADDR(base_addr), 
    cycletime(HZ_TO_PS / clock_freq_Hz, SC_PS), debug(debug) {
        dataBus.register_b_transport(this, &TLM_TIMER::b_transport);
        std::cout << "TLM_TIMER: period is " << cycletime << std::endl;
    };


    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        tlm::tlm_command cmd = trans.get_command();
        sc_dt::uint64    addr = trans.get_address();
        unsigned char*   ptr = trans.get_data_ptr();
        unsigned int     len = trans.get_data_length();

        if (((cmd != tlm::TLM_WRITE_COMMAND) && (cmd != tlm::TLM_READ_COMMAND)) || (len != 4)) {
            SC_REPORT_FATAL("TLM_TIMER", "We only support 32 bits READ and WRITE commands");
        }

        addr = addr - BASE_ADDR;
        switch(addr) {
        case LOAD_ADDR ... LOAD_ADDR_UPPER:
            if (cmd == tlm::TLM_WRITE_COMMAND) {
                load_regs[7-(addr-LOAD_ADDR)/4] = ptr[3];
                memcpy(&load_value, load_regs, 8);
                if (debug)
                    cout << sc_time_stamp() << " - TLM_TIMER: address: 0x" << (addr-LOAD_ADDR) << "with value: 0x" << hex << (int)ptr[3] << " load_value is now: 0x" << hex << load_value << endl;
            } else {
                SC_REPORT_FATAL("TLM-TIMER", "Unexpected read of LOAD");
            }
            break;
        case RELOAD_ADDR ... RELOAD_ADDR_UPPER:
            if (cmd == tlm::TLM_WRITE_COMMAND) {
                reload_regs[7-(addr-RELOAD_ADDR)/4] = ptr[3];
                memcpy(&reload_value, reload_regs, 8);
                if (debug)
                    cout << sc_time_stamp() << " - TLM_TIMER: loading RELOAD_ADDR with: 0x" << hex << reload_value << endl;
            } else {
                SC_REPORT_FATAL("TLM-TIMER", "Unexpected read of RELOAD");
            }
            break;
        case EN_ADDR:
            enable = ptr[3] & 0x1;
            time_reg = sc_time_stamp(); // Time in ps
            if (debug)
                cout << "TLM_TIMER: " << (enable ? "Enabling" : "Disabling") << " Timer. Current timestamp is " << hex << time_reg << endl;
            break;
        case UPDATE_VALUE_ADDR:
            if (cmd == tlm::TLM_WRITE_COMMAND) {
                delta = sc_time_stamp() - time_reg;
                if (enable) {
                    uint64_t ticks = delta.to_double() / cycletime.to_double();
                    if (ticks <= load_value) {
                        value = load_value - ticks;
                    } else {
                        value = (ticks - load_value) % reload_value;
                    }
                    memcpy(value_regs, &value, 8);
                    if (debug)
                        cout << "UPDATE_VALUE_ADDR: 0x" << hex << value << endl;
                } else {
                    value = load_value;
                }
                if (debug)
                    cout << "[" << sc_time_stamp() << "]: TLM_TIMER: UPDATE_VALUE completed - current delta is " << hex << delta << " enable is " << enable << " and value is now: 0x" << hex << value << endl;
            } else {
                SC_REPORT_FATAL("TLM_TIMER", "Unexpected read of UPDTE_VALUE");
            }
            break;
        case VALUE_ADDR ... VALUE_ADDR_UPPER:
            if (cmd == tlm::TLM_WRITE_COMMAND) {
                SC_REPORT_FATAL("TLM_TIMER", "Unexpected write of VALUE");
            } else {
                memset(ptr, 0, 4);
                ptr[3] = value_regs[7-(addr-VALUE_ADDR)/4];
                if (debug)
                    cout <<  "[" << sc_time_stamp() << "]: TLM_TIMER [@" << addr << "] - value is 0x" << hex << (int)ptr[3] << " of register 0x" << hex << value << endl;
            }
            break;
        case EV_STATUS_ADDR:
            cout << "TLM_TIMER: write to EV_STATUS_ADDR of 0x" << hex <<value << endl;
            SC_REPORT_FATAL("TLM_TIMER", "We only support 32 bits READ and WRITE comman");
            break;
        case EV_PENDING_ADDR:
            cout << "TLM_TIMER: write to EV_PENDING_ADDR of 0x" << hex << value << endl;
            break;
        case EV_ENABLE_ADDR:
            cout << "TLM_TIMER: write to EV_ENABLE_ADDR of 0x" << hex << value << endl;
            break;
        default:
            SC_REPORT_FATAL("TLM-TIMER", "TLM_TIMER: Address unexpected");
        }
        wait(1, SC_NS);
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
        return;
    }

};

#endif // __TLM_TIMER__ 
