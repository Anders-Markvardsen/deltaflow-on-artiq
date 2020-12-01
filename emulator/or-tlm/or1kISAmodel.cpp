#include <or1ksim.h>
#include "or1kISAmodel.hpp"
#include <iostream>
#include <string.h>

using namespace std;

SC_HAS_PROCESS(or1kISAmodel);
typedef or1kISAmodel SC_CURRENT_USER_MODULE;
or1kISAmodel::or1kISAmodel(sc_module_name name,
                           const char* configFile,
                           const char* imageFile,
                           sc_time input_clock_rate_,
                           sc_time quantum_) : finished(0) {

  input_clock_rate = input_clock_rate_;
  time_quantum = quantum_;
   /* Dummy argv array to pass arguments to or1ksim_init. Varies depending on
      whether an image file is specified. */
   int   dummy_argc;
   char *dummy_argv[6];

   program_name = new char[13];
   dash_q = new char[3];
   dash_f = new char[3];
   nosrv = new char[8];

   if (configFile == NULL) {
     cout << "please provide a config file!" << endl;
     exit(1);
   }
   if (imageFile == NULL) {
     cout << "please provide a image file!" << endl;
     exit(1);
   }
   cout << "or1k CPU model using configuration from " << configFile << endl;
   cout << "or1k CPU model using binary from " << imageFile << endl;

   configFileBuf = new char[strlen(configFile)+1];
   imageFileBuf = new char[strlen(imageFile)+1];

   memcpy(program_name, "or1kISAmodel", 13);
   // memcpy(dash_q, "-t", 3);
   memcpy(dash_q, "-V", 3); // verbose
   // memcpy(dash_q, "-q", 3); // quiet
   memcpy(dash_f, "-f", 3);
   memcpy(nosrv, "--nosrv", 8);
   memcpy(configFileBuf, configFile, strlen(configFile)+1);
   memcpy(imageFileBuf, imageFile, strlen(imageFile)+1);

   dummy_argv[0] = program_name;
   dummy_argv[1] = dash_q;
   dummy_argv[2] = dash_f; // next arg is config file
   dummy_argv[3] = configFileBuf; // config file
   dummy_argv[4] = imageFileBuf; // image file
   dummy_argv[5] = nosrv; // don't start a gdb server

   dummy_argc = 6;

  int or1k_ok = or1ksim_init(dummy_argc, dummy_argv,
               this,
               staticReadUpcall,
               staticWriteUpcall);
  if (or1k_ok != 0) {
    cout << "or1k failed init with err code " << or1k_ok << endl;
    exit(or1k_ok);
  }
  SC_THREAD(run);
  sensitive << clk.pos();
}

or1kISAmodel::~or1kISAmodel() {
  free(program_name);
  free(dash_q);
  free(dash_f);
  free(configFileBuf);
  free(imageFileBuf);
  free(nosrv);
}

void or1kISAmodel::run()
{
  finished = 0;
  int total_count = 0;
  int ret = 0;
  long cycle_count = 0;
  long blank_cycles = time_quantum.value() / input_clock_rate.value();
  cout << "or1ksim will progress " << time_quantum << " every " << blank_cycles << " cycles." << endl;
  // (void)or1ksim_run(-1.0); // run forever
  while (true) {

    if (cycle_count == 0) {
      wait();
      // cout << "------------ global cycle " << total_count << " -----------" << endl;
      // 1e-7 is basically single-step, 1e-3 is pretty performant.
      // if we only ran this for 1 in 1e6 clocks this would be rate accurate if not
      // sequence accurate.
      // if (runtime.cpu.stalled) {
      //   continue;
      // }
      ret = or1ksim_run( time_quantum.to_seconds() ); // run for 1ms
      if (ret == OR1KSIM_RC_HALTED) {
        sc_stop();
      }
      if (ret == OR1KSIM_RC_BRKPT) {
        // wait for the breakpoint to be cleared? we don't have access to the runtime...
        exit(1);
      }
    }

    cycle_count = (cycle_count + 1) % blank_cycles;
    total_count++;
  }
  finished = 1;
}

// ----------------------------------------------------------------------------
//! Static upcall for read from the underlying Or1ksim ISS

//! Static utility routine is used (for easy C linkage!) that will call the
//! class callback routine. Takes standard C types (the underlying ISS is in
//! C) as arguments, but calls the member routine, ::readUpcall(), with fixed
//! width types (from stdint.h).

//! @note In theory this ought to be possible by using member pointers to
//! functions. However given the external linkage is to C and not C++, this way
//! is much safer!

//! @param[in]  instancePtr  The pointer to the class member associated with
//!                          this upcall (originally passed to or1ksim_init in
//!                          the constructor, ::Or1ksimSC()).
//! @param[in]  addr         The address for the read
//! @param[in]  mask         The byte enable mask for the read
//! @param[out] rdata        Vector for the read data
//! @param[in]  dataLen      The number of bytes to read

//! @return  Zero on success. A return code otherwise.
// ----------------------------------------------------------------------------
int
or1kISAmodel::staticReadUpcall (void              *instancePtr,
			     unsigned long int  addr,
			     unsigned char      mask[],
			     unsigned char      rdata[],
			     int                dataLen)
{
  // cout << "read sent to " << addr << endl;
  or1kISAmodel *classPtr = (or1kISAmodel *) instancePtr;
  return  classPtr->readUpcall (addr, mask, rdata, dataLen);

}	// staticReadUpcall()


// ----------------------------------------------------------------------------
//! Static upcall for write to the underlying Or1ksim ISS

//! Static utility routine is used (for easy C linkage!) that will call the
//! class callback routine. Takes standard C types (the underlying ISS is in
//! C) as arguments, but calls the member routine, ::writeUpcall(), with fixed
//! width types (from stdint.h).

//! @note In theory this ought to be possible by using member pointers to
//! functions. However given the external linkage is to C and not C++, this way
//! is much safer!

//! @param[in] instancePtr  The pointer to the class member associated with
//!                         this upcall (originally passed to or1ksim_init in
//!                         the constructor, ::Or1ksimSC()).
//! @param[in] addr         The address for the write
//! @param[in] mask         The byte enable mask for the write
//! @param[in] wdata        Vector of data to write
//! @param[in] dataLen      The number of bytes to write

//! @return  Zero on success. A return code otherwise.
// ----------------------------------------------------------------------------
int
or1kISAmodel::staticWriteUpcall (void              *instancePtr,
			      unsigned long int  addr,
			      unsigned char      mask[],
			      unsigned char      wdata[],
			      int                dataLen)
{
  // cout << "write sent to " << addr << endl;
  // for (int i=0; i<dataLen; i++) {
  //   cout << "payload " << wdata[i] << endl;
  // }
  or1kISAmodel *classPtr = (or1kISAmodel *) instancePtr;
  return  classPtr->writeUpcall (addr, mask, wdata, dataLen);

}	// staticWriteUpcall()


// ----------------------------------------------------------------------------
//! Member function to handle read upcall from the underlying Or1ksim ISS

//! This is the entry point used by ::staticReadUpcall(). Constructs a Generic
//! transactional payload for the read, then passes it to ::doTrans() (also
//! used by ::writeUpcall()) for transport to the target.

//! @param[in]  addr         The address for the read
//! @param[in]  mask         The byte enable mask for the read
//! @param[out] rdata        Vector for the read data
//! @param[in]  dataLen      The number of bytes to read

//! @return  Zero on success. A return code otherwise.
// ----------------------------------------------------------------------------
int
or1kISAmodel::readUpcall (unsigned long int  addr,
		       unsigned char      mask[],
		       unsigned char      rdata[],
		       int                dataLen)
{
  // Set up the payload fields. Assume everything is 4 bytes.
  trans.set_read ();
  trans.set_address ((sc_dt::uint64) addr);

  trans.set_byte_enable_length ((const unsigned int) dataLen);
  trans.set_byte_enable_ptr ((unsigned char *) mask);

  trans.set_data_length ((const unsigned int) dataLen);
  trans.set_streaming_width ((const unsigned int) dataLen);
  trans.set_data_ptr ((unsigned char *) rdata);

  // Transport.
  doTrans (trans);

  /* For now just simple non-zero return code on error */
  return  trans.is_response_ok () ? 0 : -1;

}	// readUpcall()


// ----------------------------------------------------------------------------
//! Member function to handle write upcall from the underlying Or1ksim ISS

//! This is the entry point used by ::staticWriteUpcall(). Constructs a
//! Generic transactional payload for the write, then passes it to ::doTrans()
//! (also used by ::readUpcall()) for transport to the target.

//! @param[in] addr         The address for the write
//! @param[in] mask         The byte enable mask for the write
//! @param[in] wdata        Vector of data to write
//! @param[in] dataLen      The number of bytes to write

//! @return  Zero on success. A return code otherwise.
// ----------------------------------------------------------------------------
int
or1kISAmodel::writeUpcall (unsigned long int  addr,
			unsigned char      mask[],
			unsigned char      wdata[],
			int                dataLen)
{
  // Set up the payload fields. Assume everything is 4 bytes.
  trans.set_write ();
  trans.set_address ((sc_dt::uint64) addr);

  trans.set_byte_enable_length ((const unsigned int) dataLen);
  trans.set_byte_enable_ptr ((unsigned char *) mask);

  trans.set_data_length ((const unsigned int) dataLen);
  trans.set_data_ptr ((unsigned char *) wdata);

  // Transport.
  doTrans( trans );

  /* For now just simple non-zero return code on error */
  return  trans.is_response_ok () ? 0 : -1;

}	// writeUpcall()


// ----------------------------------------------------------------------------
//! TLM transport to the target

//! Calls the blocking transport routine for the initiator socket (@see
//! ::dataBus). Passes in a dummy time delay of zero.

//! @param trans  The transaction payload
// ----------------------------------------------------------------------------
void
or1kISAmodel::doTrans( tlm::tlm_generic_payload &trans )
{
  sc_core::sc_time  dummyDelay = sc_core::SC_ZERO_TIME;
  dataBus->b_transport( trans, dummyDelay );

}	// doTrans()
