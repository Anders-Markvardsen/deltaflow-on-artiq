Hardware Abstraction Layer
==========================

The Hardware Abstraction Layer (HAL) marks the interface between 
hardware and software in a quantum computer. 

For Deltaflow, this concept is central: in order to make use of 
local control, real-time control elements such as FPGAs must be 
accessible to internal and external programmers and so must lie 
above the HAL. 

We differentiate between two types of HALs that afford local control:
 
1. To implement applications like `noise aware compiling <https://arxiv.org/abs/1901.11054>`_,  
   `randomised compiling <https://arxiv.org/abs/2010.00215>`_, or 
   `accelerated VQE <https://journals.aps.org/prl/abstract/10.1103/PhysRevLett.122.140504>`_, 
   we need to be able to react to the results of small batches of circuits. 
   The latency requirement for reacting to what’s going on in the 
   QPU is on the order of *1x* the decoherence time.

2. To implement algorithms and applications like 
   `holoVQE <https://arxiv.org/abs/2005.03023>`_, 
   `QNN dropout <https://arxiv.org/abs/1911.00352>`_,  
   or simple error correction, we need to react to the results of qubits 
   measurements within a quantum circuit. This is a “measure and control” 
   type operation, and we need to react very quickly to what’s happening 
   in the QPU, on the order of *0.01x* decoherence time.
 
What exactly the decoherence time is depends on the qubit technology in use -
and on how good the qubits are that all those wonderful hardware companies
in the world are making.
 
Most HALs that are currently in use to access quantum computers 
do not afford this level of control and only allow the user to run large 
batch sizes and react to what’s happening in the QPU on the order of *1000x*
the decoherence time.

For the first release of Deltaflow we have implemented a simple and powerful
HAL:
:download:`HAL <https://github.com/riverlane/deltalanguage/blob/main/deltalanguage/lib/hal/_commands.py>`

The driver for this first implementation was the desire to write a
Rabi-oscillation on real hardware. 
The way to run the example is described in :doc:`examples`.

This simple HAL provides the following:

1. A combination of commands to request Quantum operation (e.g. :code:`SQRT_X`)
   as well as ARTIQ specific ones (e.g. :code:`SEND_TO_HOST`)

2. Constant-size commands (32 bits) that can easily handled by the hardware
   logic and parsed in an extremely quick way

3. A way of indexing qubits with your command as well optional arguments that
   can be provided to your command

The following table provide a visual representation of the current implementation with few examples:

+----------------+------------------+--------------------+---------------------+
| Opcode [31-26] | Argument [25-16] | Qubit Index [15-0] | Full representation |
+================+==================+====================+=====================+
| 0xB  (SQRT_X)  | 0xF  (15*pi/256) | 0x2 (second qubit) |        0x02C0_F000  |
+----------------+------------------+--------------------+---------------------+
| 0x4  (STOP)    |   (ignored)      |   (ignored)        |        0x0100_0000  |
+----------------+------------------+--------------------+---------------------+

We are currently working with UK hardware companies to kick off a
standardisation of the HAL.
You can find more information
`here <https://www.riverlane.com/news/2020/05/uk-companies-to-build-radically-new-operating-system-for-quantum-computers/>`_.

