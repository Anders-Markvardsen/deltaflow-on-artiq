#include <string>
#include <systemc>
using namespace sc_core;
/*[[[cog
    if body_type is not PyConstBody:
        cog.outl('#include "Python.h"')
]]]*/
#include "Python.h"
//[[[end]]]

//[[[cog cog.outl(f"#ifndef __{top_p.name.upper()}_MODULE__"); cog.outl(f"#define __{top_p.name.upper()}_MODULE__") ]]]
#ifndef __PASS_INT_3_MODULE__
#define __PASS_INT_3_MODULE__
//[[[end]]]

//[[[cog cog.outl(f"class {module_name} : public sc_module") ]]]
class Pass_Int_3_module : public sc_module
//[[[end]]]
{
private:
    /*[[[cog
        if body_type is not PyConstBody:
            cog.outl("PyObject *pBody, *pName, *pModule, *pyC, *pExit;")
    ]]]*/
    PyObject *pBody, *pName, *pModule, *pyC, *pExit;
    //[[[end]]]
    uint64_t no_ins, no_outs;
    /*[[[cog
        if body_type is not PyConstBody:
            for port in top_p.inPorts:
                cog.outl(f"PyObject* type_{self.get_sysc_port_name(port)};")
                cog.outl(f"PyObject* get_{self.get_sysc_port_name(port)}();")
                cog.outl(f"{self.as_c_type(port.type)} bits_{self.get_sysc_port_name(port)};")
                cog.outl(f"char bytes_{self.get_sysc_port_name(port)}[{self.load_port_type(port).size.in_bytes + 1}];")
        for port in top_p.outPorts:
            if body_type is not PyConstBody:
                cog.outl(f"PyObject* type_{self.get_sysc_port_name(port)};")
                cog.outl(f"{self.as_c_type(port.type)} bits_{self.get_sysc_port_name(port)};")
            cog.outl(f"void set_{self.get_sysc_port_name(port)}();")
    ]]]*/
    PyObject* type_sysc_val;
    PyObject* get_sysc_val();
    sc_dt::sc_bv<32> bits_sysc_val;
    char bytes_sysc_val[5];
    //[[[end]]]
public:
    uint64_t no_inputs, no_outputs;
    /*[[[cog
            if body_type is not PyConstBody:
                for port in top_p.inPorts:
                    cog.outl(f"sc_fifo<{self.as_c_type(port.type)}>* {self.get_sysc_port_name(port)};")
            for port in top_p.outPorts:
                cog.outl(f"sc_fifo<{self.as_c_type(port.type)}>* {self.get_sysc_port_name(port)};")
    ]]]*/
    sc_fifo<sc_dt::sc_bv<32>>* sysc_val;
    //[[[end]]]
    int get_no_inputs() const;
    int get_no_outputs() const;
    void body();
    //[[[cog cog.outl(f"SC_HAS_PROCESS({module_name});"); cog.outl(f"{module_name}(sc_module_name name);") ]]]
    SC_HAS_PROCESS(Pass_Int_3_module);
    Pass_Int_3_module(sc_module_name name);
    //[[[end]]]
};
#endif
#include <string>
#include <systemc>
using namespace sc_core;
/*[[[cog
    if body_type is not PyConstBody:
        cog.outl('#include "Python.h"')
]]]*/
#include "Python.h"
//[[[end]]]

//[[[cog cog.outl(f"#ifndef __{top_p.name.upper()}_MODULE__"); cog.outl(f"#define __{top_p.name.upper()}_MODULE__") ]]]
#ifndef __EXPERIMENT_STOPPER_4_MODULE__
#define __EXPERIMENT_STOPPER_4_MODULE__
//[[[end]]]

//[[[cog cog.outl(f"class {module_name} : public sc_module") ]]]
class Experiment_Stopper_4_module : public sc_module
//[[[end]]]
{
private:
    /*[[[cog
        if body_type is not PyConstBody:
            cog.outl("PyObject *pBody, *pName, *pModule, *pyC, *pExit;")
    ]]]*/
    PyObject *pBody, *pName, *pModule, *pyC, *pExit;
    //[[[end]]]
    uint64_t no_ins, no_outs;
    /*[[[cog
        if body_type is not PyConstBody:
            for port in top_p.inPorts:
                cog.outl(f"PyObject* type_{self.get_sysc_port_name(port)};")
                cog.outl(f"PyObject* get_{self.get_sysc_port_name(port)}();")
                cog.outl(f"{self.as_c_type(port.type)} bits_{self.get_sysc_port_name(port)};")
                cog.outl(f"char bytes_{self.get_sysc_port_name(port)}[{self.load_port_type(port).size.in_bytes + 1}];")
        for port in top_p.outPorts:
            if body_type is not PyConstBody:
                cog.outl(f"PyObject* type_{self.get_sysc_port_name(port)};")
                cog.outl(f"{self.as_c_type(port.type)} bits_{self.get_sysc_port_name(port)};")
            cog.outl(f"void set_{self.get_sysc_port_name(port)}();")
    ]]]*/
    PyObject* type_sysc_completed;
    PyObject* get_sysc_completed();
    sc_dt::sc_bv<8> bits_sysc_completed;
    char bytes_sysc_completed[2];
    //[[[end]]]
public:
    uint64_t no_inputs, no_outputs;
    /*[[[cog
            if body_type is not PyConstBody:
                for port in top_p.inPorts:
                    cog.outl(f"sc_fifo<{self.as_c_type(port.type)}>* {self.get_sysc_port_name(port)};")
            for port in top_p.outPorts:
                cog.outl(f"sc_fifo<{self.as_c_type(port.type)}>* {self.get_sysc_port_name(port)};")
    ]]]*/
    sc_fifo<sc_dt::sc_bv<8>>* sysc_completed;
    //[[[end]]]
    int get_no_inputs() const;
    int get_no_outputs() const;
    void body();
    //[[[cog cog.outl(f"SC_HAS_PROCESS({module_name});"); cog.outl(f"{module_name}(sc_module_name name);") ]]]
    SC_HAS_PROCESS(Experiment_Stopper_4_module);
    Experiment_Stopper_4_module(sc_module_name name);
    //[[[end]]]
};
#endif
// Verilated -*- SystemC -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VCOMMAND_SENDER_1_H_
#define _VCOMMAND_SENDER_1_H_  // guard

#include "systemc.h"
#include "verilated_sc.h"
#include "verilated.h"

class Vcommand_sender_1__Syms;

//----------

SC_MODULE(Vcommand_sender_1) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    sc_in<sc_bv<10> > angle_in_data;
    sc_in<sc_bv<1> > angle_in_valid;
    sc_out<sc_bv<1> > angle_in_ready;
    sc_out<sc_bv<32> > hal_command_out_data;
    sc_out<sc_bv<1> > hal_command_out_valid;
    sc_in<sc_bv<1> > hal_command_out_ready;
    sc_in<sc_bv<1> > sys_clk;
    sc_in<sc_bv<1> > sys_rst;
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*1:0*/ command_sender_1__DOT__state;
    CData/*1:0*/ command_sender_1__DOT__next_state;
    CData/*0:0*/ command_sender_1__DOT__rotation_command_next_value_ce0;
    CData/*0:0*/ command_sender_1__DOT__out_port_valid_next_value1;
    CData/*0:0*/ command_sender_1__DOT__out_port_valid_next_value_ce1;
    CData/*0:0*/ command_sender_1__DOT__out_port_data_next_value_ce2;
    IData/*31:0*/ command_sender_1__DOT__rotation_command;
    IData/*31:0*/ command_sender_1__DOT__rotation_command_next_value0;
    IData/*31:0*/ command_sender_1__DOT__out_port_data_next_value2;
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ __Vcellinp__command_sender_1__sys_clk;
    CData/*0:0*/ __Vcellinp__command_sender_1__sys_rst;
    CData/*0:0*/ __Vcellout__command_sender_1__hal_command_out_valid;
    CData/*0:0*/ __Vcellinp__command_sender_1__angle_in_valid;
    CData/*0:0*/ __Vclklast__TOP____Vcellinp__command_sender_1__sys_clk;
    SData/*9:0*/ __Vcellinp__command_sender_1__angle_in_data;
    IData/*31:0*/ __Vcellout__command_sender_1__hal_command_out_data;
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vcommand_sender_1__Syms* __VlSymsp;  // Symbol table
    
    // PARAMETERS
    // Parameters marked /*verilator public*/ for use by application code
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vcommand_sender_1);  ///< Copying not allowed
  public:
    SC_CTOR(Vcommand_sender_1);
    virtual ~Vcommand_sender_1();
    
    // API METHODS
  private:
    void eval();
  public:
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(Vcommand_sender_1__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vcommand_sender_1__Syms* symsp, bool first);
  private:
    static QData _change_request(Vcommand_sender_1__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__3(Vcommand_sender_1__Syms* __restrict vlSymsp);
    static void _combo__TOP__6(Vcommand_sender_1__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vcommand_sender_1__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vcommand_sender_1__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vcommand_sender_1__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _initial__TOP__2(Vcommand_sender_1__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__5(Vcommand_sender_1__Syms* __restrict vlSymsp);
    static void _settle__TOP__1(Vcommand_sender_1__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _settle__TOP__4(Vcommand_sender_1__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(128);

#endif  // guard
// Verilated -*- SystemC -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VRESULT_AGGREGATOR_0_H_
#define _VRESULT_AGGREGATOR_0_H_  // guard

#include "systemc.h"
#include "verilated_sc.h"
#include "verilated.h"

class Vresult_aggregator_0__Syms;

//----------

SC_MODULE(Vresult_aggregator_0) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    sc_in<sc_bv<32> > hal_result_in_data;
    sc_in<sc_bv<1> > hal_result_in_valid;
    sc_out<sc_bv<1> > hal_result_in_ready;
    sc_out<sc_bv<32> > agg_result_out_data;
    sc_out<sc_bv<1> > agg_result_out_valid;
    sc_in<sc_bv<1> > agg_result_out_ready;
    sc_out<sc_bv<8> > completed_out_data;
    sc_out<sc_bv<1> > completed_out_valid;
    sc_in<sc_bv<1> > completed_out_ready;
    sc_out<sc_bv<10> > next_angle_out_data;
    sc_out<sc_bv<1> > next_angle_out_valid;
    sc_in<sc_bv<1> > next_angle_out_ready;
    sc_in<sc_bv<1> > sys_clk;
    sc_in<sc_bv<1> > sys_rst;
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    CData/*3:0*/ result_aggregator_0__DOT__angle_rom_index;
    CData/*0:0*/ result_aggregator_0__DOT__final_angle;
    CData/*1:0*/ result_aggregator_0__DOT__state;
    CData/*1:0*/ result_aggregator_0__DOT__next_state;
    CData/*0:0*/ result_aggregator_0__DOT__agg_result_out_port_valid_next_value0;
    CData/*0:0*/ result_aggregator_0__DOT__agg_result_out_port_valid_next_value_ce0;
    CData/*0:0*/ result_aggregator_0__DOT__completed_out_port_valid_next_value1;
    CData/*0:0*/ result_aggregator_0__DOT__completed_out_port_valid_next_value_ce1;
    CData/*0:0*/ result_aggregator_0__DOT__final_angle_t_next_value0;
    CData/*0:0*/ result_aggregator_0__DOT__final_angle_t_next_value_ce0;
    CData/*0:0*/ result_aggregator_0__DOT__high_hal_results_t_next_value_ce1;
    CData/*3:0*/ result_aggregator_0__DOT__angle_rom_index_t_next_value2;
    CData/*0:0*/ result_aggregator_0__DOT__angle_rom_index_t_next_value_ce2;
    CData/*0:0*/ result_aggregator_0__DOT__aggregation_counter_t_next_value_ce3;
    CData/*7:0*/ result_aggregator_0__DOT__completed_out_port_data_t_next_value4;
    CData/*0:0*/ result_aggregator_0__DOT__completed_out_port_data_t_next_value_ce4;
    CData/*0:0*/ result_aggregator_0__DOT__next_angle_out_port_valid_t_next_value5;
    CData/*0:0*/ result_aggregator_0__DOT__next_angle_out_port_valid_t_next_value_ce5;
    CData/*0:0*/ result_aggregator_0__DOT__agg_result_out_port_data_f_next_value_ce0;
    CData/*0:0*/ result_aggregator_0__DOT__next_angle_out_port_data_f_next_value_ce1;
    CData/*3:0*/ result_aggregator_0__DOT__memadr;
    SData/*9:0*/ result_aggregator_0__DOT__next_angle_out_port_data_f_next_value1;
    IData/*31:0*/ result_aggregator_0__DOT__aggregation_counter;
    IData/*31:0*/ result_aggregator_0__DOT__high_hal_results;
    IData/*31:0*/ result_aggregator_0__DOT__high_hal_results_t_next_value1;
    IData/*31:0*/ result_aggregator_0__DOT__aggregation_counter_t_next_value3;
    IData/*31:0*/ result_aggregator_0__DOT__agg_result_out_port_data_f_next_value0;
    SData/*9:0*/ result_aggregator_0__DOT__ANGLE_ROM[16];
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ __Vcellinp__result_aggregator_0__sys_clk;
    CData/*0:0*/ __Vcellinp__result_aggregator_0__sys_rst;
    CData/*0:0*/ __Vcellout__result_aggregator_0__next_angle_out_valid;
    CData/*0:0*/ __Vcellout__result_aggregator_0__completed_out_valid;
    CData/*7:0*/ __Vcellout__result_aggregator_0__completed_out_data;
    CData/*0:0*/ __Vcellout__result_aggregator_0__agg_result_out_valid;
    CData/*0:0*/ __Vcellinp__result_aggregator_0__hal_result_in_valid;
    CData/*0:0*/ __Vclklast__TOP____Vcellinp__result_aggregator_0__sys_clk;
    SData/*9:0*/ __Vcellout__result_aggregator_0__next_angle_out_data;
    IData/*31:0*/ __Vcellout__result_aggregator_0__agg_result_out_data;
    IData/*31:0*/ __Vcellinp__result_aggregator_0__hal_result_in_data;
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vresult_aggregator_0__Syms* __VlSymsp;  // Symbol table
    
    // PARAMETERS
    // Parameters marked /*verilator public*/ for use by application code
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(Vresult_aggregator_0);  ///< Copying not allowed
  public:
    SC_CTOR(Vresult_aggregator_0);
    virtual ~Vresult_aggregator_0();
    
    // API METHODS
  private:
    void eval();
  public:
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(Vresult_aggregator_0__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vresult_aggregator_0__Syms* symsp, bool first);
  private:
    static QData _change_request(Vresult_aggregator_0__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__3(Vresult_aggregator_0__Syms* __restrict vlSymsp);
    static void _combo__TOP__6(Vresult_aggregator_0__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(Vresult_aggregator_0__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(Vresult_aggregator_0__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(Vresult_aggregator_0__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _initial__TOP__2(Vresult_aggregator_0__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__5(Vresult_aggregator_0__Syms* __restrict vlSymsp);
    static void _settle__TOP__1(Vresult_aggregator_0__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _settle__TOP__4(Vresult_aggregator_0__Syms* __restrict vlSymsp) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(128);

#endif  // guard

/*[[[cog
    cog.outl(f'#ifndef __{self._prog_name.upper()}__')
    cog.outl(f'#define __{self._prog_name.upper()}__')
]]]*/
#ifndef __RABI_DEMO__
#define __RABI_DEMO__
//[[[end]]]
#include <systemc>
using namespace sc_core;
using namespace sc_dt;

// TODO: Find better way of handling trace files
sc_trace_file *Tf;

// Converts clock signals to bit vectors for Migen nodes
SC_MODULE(ClkToBV) {
    sc_in<bool> clk;
    sc_out<sc_bv<1>> clkout;

    void run() {
        clkout.write(clk.read());
    }

    SC_CTOR(ClkToBV) {
        SC_METHOD(run);
        sensitive << clk;
    }
};

// Adaptor for going from Python to Migen
template <class T> SC_MODULE(PythonToMigen) {
    sc_in<bool> clk;
    sc_out<T> migen_data_out;
    sc_out<sc_bv<1>> migen_valid_out;
    sc_in<sc_bv<1>> migen_ready_in;
    sc_fifo<T>* py_in;

    void run() {
        if (migen_ready_in.read() == 1) {
            T val;
            if (py_in->nb_read(val)) {
                migen_data_out.write(val);
                migen_valid_out.write(1);
            } else {
                migen_valid_out.write(0);
            }
        }
    }

    SC_CTOR(PythonToMigen) {
        SC_METHOD(run);
        sensitive << clk;
    }
};

// Adaptor for going from Migen to Python
template <class T> SC_MODULE(MigenToPython) {
    sc_in<bool> clk;
    sc_in<T> migen_in;
    sc_in<sc_bv<1>> migen_valid_in;
    sc_out<sc_bv<1>> migen_ready_out;
    sc_fifo<T>* py_out;

    void run() {
        while (true) {
            wait();
            if (py_out->num_free() > 0) {
                migen_ready_out.write(1);
                if (migen_valid_in.read() == 1) {
                    py_out->write(migen_in.read());
                }
            } else {
                migen_ready_out.write(0);
            }
        }
    }

    SC_CTOR(MigenToPython) {
        SC_THREAD(run);
        sensitive << clk;
    }
};

/*[[[cog
    cog.outl(f'SC_MODULE({self._get_top_name()}){{')
]]]*/
SC_MODULE(Rabi_Demo){
//[[[end]]]

// Python nodes to Python nodes just need a queue
/*[[[cog
    for wire in self._py_to_py:
        name = self.get_wire_name(wire)
        cog.outl(f'sc_fifo<{PythonatorEnv.as_c_type(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort].type)}> {name};')
]]]*/
//[[[end]]]

// Migen nodes to Migen nodes need data wires,
// as well as valid and ready signals
/*[[[cog
    for wire in self._migen_to_migen:
        name = self.get_wire_name(wire)
        cog.outl(f'sc_signal<{VerilatorEnv.as_c_type(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort].type)}> {name}_data;')
        cog.outl(f'sc_signal<sc_bv<1>> {name}_valid;')
        cog.outl(f'sc_signal<sc_bv<1>> {name}_ready;')
]]]*/
sc_signal<sc_bv<10>> wire_0_0_1_0_data;
sc_signal<sc_bv<1>> wire_0_0_1_0_valid;
sc_signal<sc_bv<1>> wire_0_0_1_0_ready;
//[[[end]]]

// Python to Migen need an adaptor which maps a queue
// to data, valid and ready signals.
/*[[[cog
    for wire in self._py_to_migen:
        name = self.get_wire_name(wire)
        cog.outl(f'sc_fifo<{PythonatorEnv.as_c_type(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort].type)}> {name}_py_out;')
        cog.outl(f'sc_signal<{VerilatorEnv.as_c_type(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort].type)}> {name}_migen_in;')
        cog.outl(f'sc_signal<sc_bv<1>> {name}_in_valid;')
        cog.outl(f'sc_signal<sc_bv<1>> {name}_in_ready;')
        cog.outl(f'PythonToMigen<{VerilatorEnv.as_c_type(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort].type)}> {name}_adaptor;')
]]]*/
//[[[end]]]

// Migen to Python need an adaptor which maps signals
// to a queue
/*[[[cog
    for wire in self._migen_to_py:
        name = self.get_wire_name(wire)
        cog.outl(f'sc_fifo<{PythonatorEnv.as_c_type(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort].type)}> {name}_py_in;')
        cog.outl(f'sc_signal<{VerilatorEnv.as_c_type(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort].type)}> {name}_migen_out;')
        cog.outl(f'sc_signal<sc_bv<1>> {name}_out_valid;')
        cog.outl(f'sc_signal<sc_bv<1>> {name}_out_ready;')
        cog.outl(f'MigenToPython<{VerilatorEnv.as_c_type(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort].type)}> {name}_adaptor;')
]]]*/
sc_fifo<sc_dt::sc_bv<32>> wire_0_1_3_0_py_in;
sc_signal<sc_bv<32>> wire_0_1_3_0_migen_out;
sc_signal<sc_bv<1>> wire_0_1_3_0_out_valid;
sc_signal<sc_bv<1>> wire_0_1_3_0_out_ready;
MigenToPython<sc_bv<32>> wire_0_1_3_0_adaptor;
sc_fifo<sc_dt::sc_bv<8>> wire_0_2_4_0_py_in;
sc_signal<sc_bv<8>> wire_0_2_4_0_migen_out;
sc_signal<sc_bv<1>> wire_0_2_4_0_out_valid;
sc_signal<sc_bv<1>> wire_0_2_4_0_out_ready;
MigenToPython<sc_bv<8>> wire_0_2_4_0_adaptor;
//[[[end]]]

// Python to template need a public queue
/*[[[cog
    for wire in self._py_to_template:
        name = self.get_template_wire_name(wire, body="python", direction="out")
        cog.outl('sc_fifo<{PythonatorEnv.as_c_type(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort].type)}> {name};')
]]]*/
//[[[end]]]

// Template to python need a public queue
/*[[[cog
    for wire in self._template_to_py:
        name = self.get_template_wire_name(wire, body="python", direction="in")
        cog.outl('sc_fifo<{PythonatorEnv.as_c_type(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort].type)}> {name};')
]]]*/
//[[[end]]]

// Migen to template need public data, valid and ready signals
/*[[[cog
    for wire in self._migen_to_template:
        data, valid, ready = self.get_template_wire_name(wire, body="migen", direction="out")
        cog.outl(f'sc_signal<{VerilatorEnv.as_c_type(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort].type)}> {data};')
        cog.outl(f'sc_signal<sc_bv<1>> {valid};')
        cog.outl(f'sc_signal<sc_bv<1>> {ready};')
]]]*/
sc_signal<sc_bv<32>> command_sender_1_hal_command_out_data;
sc_signal<sc_bv<1>> command_sender_1_hal_command_out_valid;
sc_signal<sc_bv<1>> command_sender_1_hal_command_in_ready;
//[[[end]]]

// Template to Migen need public data, valid and ready signals
/*[[[cog
    for wire in self._template_to_migen:
        data, valid, ready = self.get_template_wire_name(wire, body="migen", direction="in")
        cog.outl(f'sc_signal<{VerilatorEnv.as_c_type(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort].type)}> {data};')
        cog.outl(f'sc_signal<sc_bv<1>> {valid};')
        cog.outl(f'sc_signal<sc_bv<1>> {ready};')
]]]*/
sc_signal<sc_bv<32>> result_aggregator_0_hal_result_in_data;
sc_signal<sc_bv<1>> result_aggregator_0_hal_result_in_valid;
sc_signal<sc_bv<1>> result_aggregator_0_hal_result_out_ready;
//[[[end]]]

sc_in<bool> clk, rst;
sc_signal<sc_dt::sc_bv<1>> rst_bv;

// Node modules
/*[[[cog
    for node in self._py_nodes:
        cog.outl(f'{PythonatorEnv.get_module_name(node)} {node.name};')
]]]*/
Pass_Int_3_module pass_int_3;
Experiment_Stopper_4_module experiment_stopper_4;
//[[[end]]]

/*[[[cog
    for node in self._migen_nodes:
        cog.outl(f'{VerilatorEnv.get_module_name(node)} {node.name};')
        cog.outl(f'ClkToBV {node.name}_clk;')
        cog.outl(f'sc_signal<sc_bv<1>> {node.name}_sysclk;')
]]]*/
Vresult_aggregator_0 result_aggregator_0;
ClkToBV result_aggregator_0_clk;
sc_signal<sc_bv<1>> result_aggregator_0_sysclk;
Vcommand_sender_1 command_sender_1;
ClkToBV command_sender_1_clk;
sc_signal<sc_bv<1>> command_sender_1_sysclk;
//[[[end]]]

// Constructor first initialises queues, modules and adaptors
/*[[[cog
    cog.outl(f'typedef {self._get_top_name()} SC_CURRENT_USER_MODULE;')
    cog.outl(f'{self._get_top_name()}(sc_module_name name, sc_trace_file *Tf):')

    init_modules = []
    for wire in self._py_to_py:
        init_modules.append(f'{self.get_wire_name(wire)}("{self.get_wire_name(wire)}")')

    for wire in self._py_to_migen:
        name = self.get_wire_name(wire)
        init_modules.append(f'{name}_py_out("{name}_py_out")')
        init_modules.append(f'{name}_adaptor("{name}_adaptor")')

    for wire in self._migen_to_py:
        name = self.get_wire_name(wire)
        init_modules.append(f'{name}_py_in("{name}_py_in")')
        init_modules.append(f'{name}_adaptor("{name}_adaptor")')

    for node in self._py_nodes:
        init_modules.append(f'{node.name}("{node.name}")')

    for node in self._migen_nodes:
        init_modules.append(f'{node.name}("{node.name}")')
        init_modules.append(f'{node.name}_clk("{node.name}_clk")')

    cog.out(",\n".join(init_modules))
]]]*/
typedef Rabi_Demo SC_CURRENT_USER_MODULE;
Rabi_Demo(sc_module_name name, sc_trace_file *Tf):
wire_0_1_3_0_py_in("wire_0_1_3_0_py_in"),
wire_0_1_3_0_adaptor("wire_0_1_3_0_adaptor"),
wire_0_2_4_0_py_in("wire_0_2_4_0_py_in"),
wire_0_2_4_0_adaptor("wire_0_2_4_0_adaptor"),
pass_int_3("pass_int_3"),
experiment_stopper_4("experiment_stopper_4"),
result_aggregator_0("result_aggregator_0"),
result_aggregator_0_clk("result_aggregator_0_clk"),
command_sender_1("command_sender_1"),
command_sender_1_clk("command_sender_1_clk")
//[[[end]]]
{
    SC_METHOD(rstprop);
    sensitive << rst;

    // Wiring the clock to the Migen nodes
    /*[[[cog
        for node in self._migen_nodes:
            cog.outl(f'{node.name}_clk.clk(clk);')
            cog.outl(f'{node.name}_clk.clkout.bind({node.name}_sysclk);')
            cog.outl(f'{node.name}.sys_clk.bind({node.name}_sysclk);')
            cog.outl(f'sc_trace(Tf, {node.name}_sysclk, "{node.name}_sysclk");')
            cog.outl(f'{node.name}.sys_rst.bind(rst_bv);')
    ]]]*/
    result_aggregator_0_clk.clk(clk);
    result_aggregator_0_clk.clkout.bind(result_aggregator_0_sysclk);
    result_aggregator_0.sys_clk.bind(result_aggregator_0_sysclk);
    sc_trace(Tf, result_aggregator_0_sysclk, "result_aggregator_0_sysclk");
    result_aggregator_0.sys_rst.bind(rst_bv);
    command_sender_1_clk.clk(clk);
    command_sender_1_clk.clkout.bind(command_sender_1_sysclk);
    command_sender_1.sys_clk.bind(command_sender_1_sysclk);
    sc_trace(Tf, command_sender_1_sysclk, "command_sender_1_sysclk");
    command_sender_1.sys_rst.bind(rst_bv);
    //[[[end]]]

    // Wiring the Python to Python nodes
    /*[[[cog
        for wire in self._py_to_py:
            name = self.get_wire_name(wire)
            cog.outl(f'{self._capnp_nodes[wire.srcNode].name}.{PythonatorEnv.get_sysc_port_name(self._capnp_nodes[wire.srcNode].outPorts[wire.srcOutPort])} = &{name};')
            cog.outl(f'{self._capnp_nodes[wire.destNode].name}.{PythonatorEnv.get_sysc_port_name(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort])} = &{name};')
    ]]]*/
    //[[[end]]]

    // Wiring the Migen to Migen nodes
    /*[[[cog
        for wire in self._migen_to_migen:
            name = self.get_wire_name(wire)
            cog.outl(f'{self._capnp_nodes[wire.srcNode].name}.{VerilatorEnv.get_port_output(self._capnp_nodes[wire.srcNode].outPorts[wire.srcOutPort], direction="out")}.bind({name}_data);')
            cog.outl(f'{self._capnp_nodes[wire.destNode].name}.{VerilatorEnv.get_port_output(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort], direction="in")}.bind({name}_data);')
            cog.outl(f'{self._capnp_nodes[wire.srcNode].name}.{VerilatorEnv.get_port_valid(self._capnp_nodes[wire.srcNode].outPorts[wire.srcOutPort], direction="out")}.bind({name}_valid);')
            cog.outl(f'{self._capnp_nodes[wire.destNode].name}.{VerilatorEnv.get_port_valid(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort], direction="in")}.bind({name}_valid);')
            cog.outl(f'{self._capnp_nodes[wire.srcNode].name}.{VerilatorEnv.get_port_ready(self._capnp_nodes[wire.srcNode].outPorts[wire.srcOutPort], direction="in")}.bind({name}_ready);')
            cog.outl(f'{self._capnp_nodes[wire.destNode].name}.{VerilatorEnv.get_port_ready(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort], direction="out")}.bind({name}_ready);')
    ]]]*/
    result_aggregator_0.next_angle_out_data.bind(wire_0_0_1_0_data);
    command_sender_1.angle_in_data.bind(wire_0_0_1_0_data);
    result_aggregator_0.next_angle_out_valid.bind(wire_0_0_1_0_valid);
    command_sender_1.angle_in_valid.bind(wire_0_0_1_0_valid);
    result_aggregator_0.next_angle_out_ready.bind(wire_0_0_1_0_ready);
    command_sender_1.angle_in_ready.bind(wire_0_0_1_0_ready);
    //[[[end]]]

    // Wiring the Python to Migen nodes
    /*[[[cog
        for wire in self._py_to_migen:
            name = self.get_wire_name(wire)
            cog.outl(f'{self._capnp_nodes[wire.srcNode].name}.{PythonatorEnv.get_sysc_port_name(self._capnp_nodes[wire.srcNode].outPorts[wire.srcOutPort])} = &{name}_py_out;')
            cog.outl(f'{name}_adaptor.clk(clk);')
            cog.outl(f'{name}_adaptor.py_in = &{name}_py_out;')
            cog.outl(f'{name}_adaptor.migen_data_out.bind({name}_migen_in);')
            cog.outl(f'{name}_adaptor.migen_valid_out.bind({name}_in_valid);')
            cog.outl(f'{name}_adaptor.migen_ready_in.bind({name}_in_ready);')
            cog.outl(f'{self._capnp_nodes[wire.destNode].name}.{VerilatorEnv.get_port_output(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort], direction="in")}.bind({name}_migen_in);')
            cog.outl(f'{self._capnp_nodes[wire.destNode].name}.{VerilatorEnv.get_port_valid(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort], direction="in")}.bind({name}_in_valid);')
            cog.outl(f'{self._capnp_nodes[wire.destNode].name}.{VerilatorEnv.get_port_ready(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort], direction="out")}.bind({name}_in_ready);')
    ]]]*/
    //[[[end]]]

    // Wiring the Migen to Python nodes
    /*[[[cog
        for wire in self._migen_to_py:
            name = self.get_wire_name(wire)
            cog.outl(f'{name}_adaptor.clk(clk);')
            cog.outl(f'{name}_adaptor.migen_in.bind({name}_migen_out);')
            cog.outl(f'{name}_adaptor.migen_valid_in.bind({name}_out_valid);')
            cog.outl(f'{name}_adaptor.migen_ready_out.bind({name}_out_ready);')
            cog.outl(f'{name}_adaptor.py_out = &{self.get_wire_name(wire)}_py_in;')
            cog.outl(f'{self._capnp_nodes[wire.srcNode].name}.{VerilatorEnv.get_port_output(self._capnp_nodes[wire.srcNode].outPorts[wire.srcOutPort], direction="out")}.bind({name}_migen_out);')
            cog.outl(f'{self._capnp_nodes[wire.srcNode].name}.{VerilatorEnv.get_port_valid(self._capnp_nodes[wire.srcNode].outPorts[wire.srcOutPort], direction="out")}.bind({name}_out_valid);')
            cog.outl(f'{self._capnp_nodes[wire.srcNode].name}.{VerilatorEnv.get_port_ready(self._capnp_nodes[wire.srcNode].outPorts[wire.srcOutPort], direction="in")}.bind({name}_out_ready);')
            cog.outl(f'{self._capnp_nodes[wire.destNode].name}.{PythonatorEnv.get_sysc_port_name(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort])} = &{name}_py_in;')
    ]]]*/
    wire_0_1_3_0_adaptor.clk(clk);
    wire_0_1_3_0_adaptor.migen_in.bind(wire_0_1_3_0_migen_out);
    wire_0_1_3_0_adaptor.migen_valid_in.bind(wire_0_1_3_0_out_valid);
    wire_0_1_3_0_adaptor.migen_ready_out.bind(wire_0_1_3_0_out_ready);
    wire_0_1_3_0_adaptor.py_out = &wire_0_1_3_0_py_in;
    result_aggregator_0.agg_result_out_data.bind(wire_0_1_3_0_migen_out);
    result_aggregator_0.agg_result_out_valid.bind(wire_0_1_3_0_out_valid);
    result_aggregator_0.agg_result_out_ready.bind(wire_0_1_3_0_out_ready);
    pass_int_3.sysc_val = &wire_0_1_3_0_py_in;
    wire_0_2_4_0_adaptor.clk(clk);
    wire_0_2_4_0_adaptor.migen_in.bind(wire_0_2_4_0_migen_out);
    wire_0_2_4_0_adaptor.migen_valid_in.bind(wire_0_2_4_0_out_valid);
    wire_0_2_4_0_adaptor.migen_ready_out.bind(wire_0_2_4_0_out_ready);
    wire_0_2_4_0_adaptor.py_out = &wire_0_2_4_0_py_in;
    result_aggregator_0.completed_out_data.bind(wire_0_2_4_0_migen_out);
    result_aggregator_0.completed_out_valid.bind(wire_0_2_4_0_out_valid);
    result_aggregator_0.completed_out_ready.bind(wire_0_2_4_0_out_ready);
    experiment_stopper_4.sysc_completed = &wire_0_2_4_0_py_in;
    //[[[end]]]

    // Wiring the Python to template nodes
    /*[[[cog
        for wire in self._py_to_template:
            name = self.get_template_wire_name(wire, body="python", direction="out")
            cog.outl(f'{self._capnp_nodes[wire.srcNode].name}.{PythonatorEnv.get_sysc_port_name(self._capnp_nodes[wire.srcNode].outPorts[wire.srcOutPort])} = &{name};')
    ]]]*/
    //[[[end]]]

    // Wiring the template to Python nodes
    /*[[[cog
        for wire in self._template_to_py:
            name = self.get_template_wire_name(wire, body="python", direction="in")
            cog.outl(f'{self._capnp_nodes[wire.destNode].name}.{PythonatorEnv.get_sysc_port_name(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort])} = &{name};')
    ]]]*/
    //[[[end]]]

    // Wiring the Migen to template nodes
    /*[[[cog
        for wire in self._migen_to_template:
            data, valid, ready = self.get_template_wire_name(wire, body="migen", direction="out")
            cog.outl(f'{self._capnp_nodes[wire.srcNode].name}.{VerilatorEnv.get_port_output(self._capnp_nodes[wire.srcNode].outPorts[wire.srcOutPort], direction="out")}.bind({data});')
            cog.outl(f'{self._capnp_nodes[wire.srcNode].name}.{VerilatorEnv.get_port_valid(self._capnp_nodes[wire.srcNode].outPorts[wire.srcOutPort], direction="out")}.bind({valid});')
            cog.outl(f'{self._capnp_nodes[wire.srcNode].name}.{VerilatorEnv.get_port_ready(self._capnp_nodes[wire.srcNode].outPorts[wire.srcOutPort], direction="in")}.bind({ready});')
    ]]]*/
    command_sender_1.hal_command_out_data.bind(command_sender_1_hal_command_out_data);
    command_sender_1.hal_command_out_valid.bind(command_sender_1_hal_command_out_valid);
    command_sender_1.hal_command_out_ready.bind(command_sender_1_hal_command_in_ready);
    //[[[end]]]

    // Wiring the template to Migen nodes
    /*[[[cog
        for wire in self._template_to_migen:
            data, valid, ready = self.get_template_wire_name(wire, body="migen", direction="in")
            cog.outl(f'{self._capnp_nodes[wire.destNode].name}.{VerilatorEnv.get_port_output(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort], direction="in")}.bind({data});')
            cog.outl(f'{self._capnp_nodes[wire.destNode].name}.{VerilatorEnv.get_port_valid(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort], direction="in")}.bind({valid});')
            cog.outl(f'{self._capnp_nodes[wire.destNode].name}.{VerilatorEnv.get_port_ready(self._capnp_nodes[wire.destNode].inPorts[wire.destInPort], direction="out")}.bind({ready});')
    ]]]*/
    result_aggregator_0.hal_result_in_data.bind(result_aggregator_0_hal_result_in_data);
    result_aggregator_0.hal_result_in_valid.bind(result_aggregator_0_hal_result_in_valid);
    result_aggregator_0.hal_result_in_ready.bind(result_aggregator_0_hal_result_out_ready);
    //[[[end]]]

    // Add tracing
    /*[[[cog
        for wire in self._py_to_py:
            name = self.get_wire_name(wire)
            cog.outl(f'{name}.trace(Tf);')
    ]]]*/
    //[[[end]]]

    /*[[[cog
        for wire in self._migen_to_migen:
            name = self.get_wire_name(wire)
            cog.outl(f'sc_trace(Tf, {name}_data, "{name}_data");')
            cog.outl(f'sc_trace(Tf, {name}_valid, "{name}_valid");')
            cog.outl(f'sc_trace(Tf, {name}_ready, "{name}_ready");')
    ]]]*/
    sc_trace(Tf, wire_0_0_1_0_data, "wire_0_0_1_0_data");
    sc_trace(Tf, wire_0_0_1_0_valid, "wire_0_0_1_0_valid");
    sc_trace(Tf, wire_0_0_1_0_ready, "wire_0_0_1_0_ready");
    //[[[end]]]

    /*[[[cog
        for wire in self._py_to_migen:
            name = self.get_wire_name(wire)
            cog.outl(f'{name}_py_out.trace(Tf);')
            cog.outl(f'sc_trace(Tf, {name}_migen_in, "{name}_migen_in");')
            cog.outl(f'sc_trace(Tf, {name}_in_valid, "{name}_in_valid");')
            cog.outl(f'sc_trace(Tf, {name}_in_ready, "{name}_in_ready");')
    ]]]*/
    //[[[end]]]

    /*[[[cog
        for wire in self._migen_to_py:
            name = self.get_wire_name(wire)
            cog.outl(f'{name}_py_in.trace(Tf);')
            cog.outl(f'sc_trace(Tf, {name}_migen_out, "{name}_migen_out");')
            cog.outl(f'sc_trace(Tf, {name}_out_valid, "{name}_out_valid");')
            cog.outl(f'sc_trace(Tf, {name}_out_ready, "{name}_out_ready");')
    ]]]*/
    wire_0_1_3_0_py_in.trace(Tf);
    sc_trace(Tf, wire_0_1_3_0_migen_out, "wire_0_1_3_0_migen_out");
    sc_trace(Tf, wire_0_1_3_0_out_valid, "wire_0_1_3_0_out_valid");
    sc_trace(Tf, wire_0_1_3_0_out_ready, "wire_0_1_3_0_out_ready");
    wire_0_2_4_0_py_in.trace(Tf);
    sc_trace(Tf, wire_0_2_4_0_migen_out, "wire_0_2_4_0_migen_out");
    sc_trace(Tf, wire_0_2_4_0_out_valid, "wire_0_2_4_0_out_valid");
    sc_trace(Tf, wire_0_2_4_0_out_ready, "wire_0_2_4_0_out_ready");
    //[[[end]]]

    /*[[[cog
        for wire in self._py_to_template:
            name = self.get_template_wire_name(wire, 'python', 'out')
            cog.outl(f'{name}.trace(Tf);')
    ]]]*/
    //[[[end]]]

    /*[[[cog
        for wire in self._template_to_py:
            name = self.get_template_wire_name(wire, 'python', 'in')
            cog.outl(f'{name}.trace(Tf);')
    ]]]*/
    //[[[end]]]

    /*[[[cog
        for wire in self._migen_to_template:
            data, valid, ready = self.get_template_wire_name(wire, body="migen", direction="out")
            cog.outl(f'sc_trace(Tf, {data}, "{data}");')
            cog.outl(f'sc_trace(Tf, {valid}, "{valid}");')
            cog.outl(f'sc_trace(Tf, {ready}, "{ready}");')
    ]]]*/
    sc_trace(Tf, command_sender_1_hal_command_out_data, "command_sender_1_hal_command_out_data");
    sc_trace(Tf, command_sender_1_hal_command_out_valid, "command_sender_1_hal_command_out_valid");
    sc_trace(Tf, command_sender_1_hal_command_in_ready, "command_sender_1_hal_command_in_ready");
    //[[[end]]]

    /*[[[cog
        for wire in self._template_to_migen:
            data, valid, ready = self.get_template_wire_name(wire, body="migen", direction="in")
            cog.outl(f'sc_trace(Tf, {data}, "{data}");')
            cog.outl(f'sc_trace(Tf, {valid}, "{valid}");')
            cog.outl(f'sc_trace(Tf, {ready}, "{ready}");')
    ]]]*/
    sc_trace(Tf, result_aggregator_0_hal_result_in_data, "result_aggregator_0_hal_result_in_data");
    sc_trace(Tf, result_aggregator_0_hal_result_in_valid, "result_aggregator_0_hal_result_in_valid");
    sc_trace(Tf, result_aggregator_0_hal_result_out_ready, "result_aggregator_0_hal_result_out_ready");
    //[[[end]]]
}

void rstprop() {
    // Propogate reset signal to Migen nodes
    rst_bv.write(rst.read());
}

};

#endif
