#include "rabi_demo.h"

#include <iostream>
#include <systemc>
#include <limits>
#include <sstream>

using namespace std;

struct MockBlock: sc_module
{
     sc_in<sc_bv<32>> cmd_data {"cmd_data"};
     sc_in<sc_bv<1>> cmd_valid {"cmd_valid"};
     sc_out<sc_bv<1>> cmd_ready {"cmd_ready"};
     sc_out<sc_bv<32>> readout_data {"readout_data"};
     sc_out<sc_bv<1>> readout_valid {"readout_valid"};
     sc_in<sc_bv<1>> readout_ready {"readout_ready"};

     void handle_cmd(){
         cmd_ready.write(1);
         if (cmd_valid.read() == 1){
             std::cout << sc_time_stamp() << " Command received was: " << std::hex << cmd_data.read() << std::endl;
         }
     }

    void handle_readout(){
        if (readout_ready.read() == 1){
            std::cout << sc_time_stamp() << " Sending stuff back " << std::endl;
            readout_data.write(0xbabecafe);
            readout_valid.write(1);
        } else {
            readout_valid.write(0);
       }
    }

    explicit MockBlock(sc_module_name name): sc_module(name) {
         SC_METHOD(handle_cmd);
         sensitive << cmd_valid << cmd_data;
         SC_METHOD(handle_readout);
         sensitive << readout_ready;
         dont_initialize();
       }

     SC_HAS_PROCESS(MockBlock);
};



int sc_main(__attribute__((unused)) int argc, __attribute__((unused)) char** argv) {
  Py_Initialize();

  cout << "hello" << endl;
  sc_trace_file *Tf;
  Tf = sc_create_vcd_trace_file("/workdir/rabi_test");
  sc_clock clk("clk", sc_time(1, SC_NS));
  sc_trace(Tf, clk, "clk");
  sc_signal<bool> rst; sc_trace(Tf, rst, "rst"); rst.write(0);

  Rabi_Demo demo = Rabi_Demo("rabi", Tf);
  demo.clk.bind(clk);
  demo.rst.bind(rst);

  MockBlock mock = MockBlock("mock");

  mock.cmd_data.bind(demo.command_sender_1_hal_command_out_data);
  mock.cmd_valid.bind(demo.command_sender_1_hal_command_out_valid);
  mock.cmd_ready.bind(demo.command_sender_1_hal_command_in_ready);

  mock.readout_data.bind(demo.result_aggregator_0_hal_result_in_data);
  mock.readout_valid.bind(demo.result_aggregator_0_hal_result_in_valid);
  mock.readout_ready.bind(demo.result_aggregator_0_hal_result_out_ready);

  rst.write(1);
  sc_start(5, SC_NS);
  rst.write(0);
  try {
    sc_start(200, SC_NS);
    sc_close_vcd_trace_file(Tf);
  } catch (...){
    cout << "exiting on error" << endl;
    Py_Finalize();
    sc_close_vcd_trace_file(Tf);
    throw;
  }
  Py_Finalize();


  return 0;
}
