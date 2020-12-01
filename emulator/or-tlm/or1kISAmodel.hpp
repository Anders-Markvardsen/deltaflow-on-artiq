/*
Or1k systemc tlm wrapper.

this file is derived from
// This file is part of the example programs for "Building a Loosely Timed SoC
// Model with OSCI TLM 2.0"
with updates for systemc 2.3 and recent gcc versions with strict warnings.


// Copyright (C) 2008, 2010 Embecosm Limited <info@embecosm.com>

// Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

used under GPlv3.

// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 of the License, or (at your option)
// any later version.

// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.

// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <systemc.h>
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"

SC_MODULE(or1kISAmodel) {

  tlm_utils::simple_initiator_socket<or1kISAmodel> dataBus;
  sc_in_clk clk;

  // times in ns
  // time_quantum is the time to run per CPU invocation
  sc_time time_quantum{1, SC_US};
  sc_time input_clock_rate;

  or1kISAmodel(sc_module_name name,
               const char* configFile,
               const char* imageFile,
               sc_time input_clock_rate_,
               sc_time quantum_);

  ~or1kISAmodel();

  void run();
  bool finished;

  // I/O upcalls from Or1ksim, with a common synchronized transport
  // utility. These are not changed in later derived classes.
  static int staticReadUpcall (
        void              *instancePtr,
				unsigned long int  addr,
				unsigned char      mask[],
				unsigned char      rdata[],
				int                dataLen);

  static int staticWriteUpcall (
         void              *instancePtr,
				 unsigned long int  addr,
				 unsigned char      mask[],
				 unsigned char      wdata[],
				 int                dataLen);

  int readUpcall (
        unsigned long int  addr,
			  unsigned char      mask[],
			  unsigned char      rdata[],
			  int                dataLen);

  int writeUpcall (
         unsigned long int  addr,
			   unsigned char      mask[],
			   unsigned char      wdata[],
			   int                dataLen);


  tlm::tlm_generic_payload  trans;
  void doTrans( tlm::tlm_generic_payload &trans );

private:
  // need to provide a argv char[] * for the or1k init - needs to be
  // non-const for reasons and so make it have same lifetime as the class
  char* program_name;
  char* dash_q;
  char* dash_f;
  char* configFileBuf;
  char* imageFileBuf;
  char* nosrv;
};
