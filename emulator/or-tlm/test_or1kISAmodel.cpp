#include "or1kISAmodel.hpp"
#include <iostream>
#include <systemc>

using namespace std;

int sc_main(int argc, char** argv) {
  cout << "hello" << endl;
  or1kISAmodel cpu = or1kISAmodel("cpu",
                                  "/workdir/or-tlm/or1ksim.cfg",
                                  "/workdir/build/or1k_test_image.elf");
  cpu.run();
  return 0;
}
