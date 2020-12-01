Examples
========

In the current release we are providing three demos that represents the standard 
steps that an application developer can take. From experience we know that testing
on FPGAs directly in the lab is time consuming, for this reason we come up with these
three levels of verification of your code.

Let's use the Rabi Oscillation as an example of we think we can help in designing 
better Quantum Software.

Step 1: Fast Design
-------------------

In the first phase you decide that you want to implement a Rabi Oscillation in your system.
Instead of writing FPGA code you sketch a graph. In these graph you identify the main nodes:
- A node that will decide what rotation to apply
- A node that converts this information into an Hardware Abstraction Layer friendly format
- A node that receives these values and "pretends"

The jupyter notebook describe this process in more detail:

Equivalently to run the jupyter notebook, you can execute the command

    .. code-block:: console

      $ make run-deltagraph

this will call the graph that defines the rabi experiment, run it and return some statistics. 

In this stage you make changes, test them quickly and verify your general assumption. 
You don't have to worry yet about the timing of your control and you can focuse on the algorithm.

Step 2: Add the notion of Time
------------------------------

In particular with FPGA code it is initially difficult to enter into the "hardware" mentality. 

Multiple blocks execute can be execute at the same time, communicate and a small change can alter
the behaviour of the whole system. 

Traditional simulation of these behaviour strongly relies on commercial simulators that require 
licenses, have relatively steep learning curves will look "slow" to most software developers. 

To tackle the first and second issues we have decided to rely on a C++ based language, 
SystemC, that represents the hardware behaviour in a relatively easy way to understand.
SystemC is developed by Accelera and is free-to-use. The European Space Agency has an interesting
description of it at `ESA-SystemC <https://www.esa.int/Enabling_Support/Space_Engineering_Technology/Microelectronics/System-Level_Modeling_in_SystemC>`_

Finally to tackle the speed issue we come up with the idea of converting our verilog code into SystemC via 
Verilator, an open-source Linux-foundation project that provides massive speedups when compared to traditional simulators:
`Verilator <https://github.com/verilator/verilator>`_

Back to our Rabi Demo, we wanted to verify that all the nodes were receiving the data at the right time and that 
our backend (ProjectQ) was happy with it.

By running:

.. code-block:: console
    
    $ make run-deltasimulator

We: 

1. Load from the same python files the Rabi Graph

2. Convert it to a transportable representation (serialization phase)

3. Extract the nodes and:

    (A) If they are Python based (*DeltaBlock*, *DeltaMethodBlock*, etc): create a SystemC wrapper that uses C-Python bind to allow SystemC signals to reach the PythonNodes and vice-versa

    (B) If they are Migen based (*MigenNode*): generate the verilog code for them and pass it to verilator to convert them to SystemC (not-synthetizable)

4. Auto-wire the nodes to create a SystemC representation of the graph: SystemC blocks are now all wired together

5. Compile and link to generate an executable.

When running the executable, all the signals of interested are logged into a Value Change Dump (.vcd) file - a convenient (and open-source) way to represent changes of status of a system with timing considerations.
Various tools can be used to visualize this format: at Riverlane we normally use Gtkwave (open-source).


Step 3: Add the latency/behaviour of a real control stack
---------------------------------------------------------

We believe that control latency can make or break a significant number of future Quantum applications. Also, we believe that with an emulator more developers can work in parallel on the same problem with no need to use precious quantum resources.

Building on the previous step, we can connect an emulator of an ARTIQ system to show how data move and allow designers to make the best possible decisions. 

Via a simple Hardware Abstraction Layer that we have implemented graphs can send and request commands to and from the ARTIQ emulator - with a clear representation of time.

By running:

.. code-block:: console
    
    $ make run-kasli-emulator

Quite few things happen (assuming a clean environment, some steps will be skipped if their output is already available):

1. We call the ARTIQ toolchain to generate the FPGA code (verilog) and to compile the firmware (Rust). Both outputs are "driven" by the description of our system in :code:`emulator/SoCBuilder/virtual_kasli.py`. Following the real "kasli" target, we have made few adjustments to make it fast in terms of emulation, with limited impact on the accuracy of the results. (Note: you can force a recompilation by removing the :code:`build/ARTIQ` file)

2. We compile the CPU model `Or1kSim <https://github.com/openrisc/or1ksim>`_ with its SystemC wrapper

3. We compile two flash images that ARTIQ can use to load experiments, startup kernel and idle kernel. We implement a simple test of the I/Os in the former, showing at the same time how a simple ARTIQ kernel can be written.

4. We compile our Rabi Graph, convert it to SystemC

5. We convert via Verilator the output of step 1 (the verilog portion) into a SystemC representation. 

6. We connect the output of 5 to our generic simulation models (defined in |Deltamodels|_) and to some ARTIQ specific ones.

7. We compile and link together all the blocks.

8. Finally we execute the demo! This takes a bit of time (3-5 mins on our machines) but it shows all the boot sequence of the ARTIQ system, with two stages bootloading, readout and execution from both SPI Flash and DDR memories.

9. We start to fetch HAL commands from the HAL bridge via the startup kernel and schedule them to be execute by the RTIO: 

.. code-block:: console
    
    357750 ns : ARTIQ_CONSOLE: [0.003300s]  INFO(kernel): Retrieving 10 Rabi commands 
    and sending them to the I/Os

10. Just before the completion of the run you will notice messages like: 

.. code-block:: console
   
   3642191 ns : QUANTUM_INTERFACE: <-- 1c000000 

that's our graph sending requests to the quantum world via simple commands!!

Eventhough the 3-5 minutes might seem long, to spot some of type of errors on a real system might take days if not weeks! Instead, by providing loads of information about the internal state of the system (feel free to look into :code:`build/logs/` for outputs of the CPUs, memories and general transactions) we believe a significant portion of bugs can be investigated outside the lab. Even better, they can be prevented with Continous Integration and tests. 


.. |Deltamodels| replace:: **Deltamodels**
.. _Deltamodels: https://riverlane.github.io/deltamodels


