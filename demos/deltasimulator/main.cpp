#include "rabi_demo.h"
#include "/workdir/demos/deltasimulator/sc_hal_block.hpp"
#include <iostream>
#include <systemc>
#include <limits>
#include <sstream>
#include <Python.h>

using namespace std;
using namespace sc_core;
using namespace sc_dt;

int sc_main(__attribute__((unused)) int argc, __attribute__((unused)) char** argv) {
  sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated",
                                           sc_core::SC_DO_NOTHING );  
  Py_Initialize();

  cout << "hello" << endl;

  sc_trace_file *Tf;
  Tf = sc_create_vcd_trace_file("/workdir/rabi_deltaruntime_run");
  sc_clock clk("clk", sc_time(1, SC_NS));
  sc_trace(Tf, clk, "clk");
  sc_signal<bool> rst; sc_trace(Tf, rst, "rst"); rst.write(0);

  Rabi_Demo rabi("RABI", Tf);
  rabi.clk.bind(clk);
  rabi.rst.bind(rst);

  // The hal block can be configured to include latency.
  HalBlock hal = HalBlock("HAL", 5);
  hal.clk.bind(clk);
  hal.rst.bind(rst);
  hal.hello();

  // bind output of command sender to input of HAL
  hal.hal_command_data.bind(rabi.command_sender_1_hal_command_out_data);
  hal.hal_command_valid.bind(rabi.command_sender_1_hal_command_out_valid);
  hal.hal_command_ready.bind(rabi.command_sender_1_hal_command_in_ready);

  // bind input of result aggregator to output of HAL
  hal.hal_result_data.bind(rabi.result_aggregator_0_hal_result_in_data);
  hal.hal_result_valid.bind(rabi.result_aggregator_0_hal_result_in_valid);
  hal.hal_result_ready.bind(rabi.result_aggregator_0_hal_result_out_ready);

  rst.write(1);
  sc_start(5, SC_NS);
  rst.write(0);

  try {
    sc_start(1000, SC_NS);
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
