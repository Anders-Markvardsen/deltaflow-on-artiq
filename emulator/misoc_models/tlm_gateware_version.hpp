/*
This file contains a generic TLM read-only memory.
This is useful for mapping a binary file into the address space of a virtual cpu.

2020 Thomas Parks, Riverlane

V1: basic ROM functionality
*/
#include <locale>
#include <systemc>
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
using namespace std;
