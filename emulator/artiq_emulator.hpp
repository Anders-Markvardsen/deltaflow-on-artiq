#ifndef __ARTIQ_EMULATOR__
#define __ARTIQ_EMULATOR__

#include "or1kISAmodel.hpp"
#include <iostream>
#include <systemc>
#include <limits>
#include <sstream>

#include "tlm.h"
#include "deltamodels/commons/testbench_utils.hpp"
#include "misoc_models/tlm_gateware_version.hpp"
#include "misoc_models/tlm_identifier.hpp"
#include "misoc_models/tlm_dbg.hpp"
#include "misoc_models/tlm_uart.hpp"
#include "misoc_models/tlm_eth_phy.hpp"
#include "misoc_models/tlm_timer.hpp"
#include "deltamodels/tlms/tlm_router/tlm_router.hpp"
#include "deltamodels/tlms/tlm_adapters/tlm2wishbone.hpp"
#include "deltamodels/tlms/commons/logging.hpp"
#include "deltamodels/tlms/tlm_memories/tlm_rom.hpp"
#include "deltamodels/models/basic_blocks/tristate.hpp"
#include "deltamodels/models/memories/flash/N25QX.hpp"
#include "deltamodels/models/memories/sdram/generic_sdram.hpp"
#include "deltamodels/models/wishbone/or1k/or1k_data_tracer.hpp"
#include "deltamodels/models/wishbone/or1k/or1k_instruction_tracer.hpp"

#include "build/verilated/Vtop.h"

// CSR <-> Address of the peripherals
#include "build/artiq_kasli/software/include/generated/csr.h"
// MEM <-> Address of the memories
#include "build/artiq_kasli/software/include/generated/mem.h"

// Quantum interfacing
#include "quantum_interface.hpp"

// For tracing and debugging
#include "verilated_vcd_sc.h"

using namespace std;


struct ARTIQ_EMULATOR: sc_module
{
    sc_in_clk clk;
    sc_in<bool> rst;
    // Riverlane HAL interface
    sc_in<sc_bv<32>> hal_cmd;
    sc_in<sc_bv<1>> hal_cmd_valid;
    sc_out<sc_bv<1>> hal_cmd_ready;
    sc_out<sc_bv<32>> hal_readout;
    sc_out<sc_bv<1>> hal_readout_valid;
    sc_in<sc_bv<1>> hal_readout_ready;

     // Modules
    Clk2BV clk_to_bv;
    TLMRouter<1, 5> router;
    or1kISAmodel cpu;
    TLM_ROM reset_rom;
    Vtop SoC;
    BvToBool flash_clk_bv2bool;
    N25QX<4> flash_model;
    TLM_TIMER timer;
    TLM_IDENTIFIER version;
    TLM_UART console;
    TLM2WB<30, sc_bv<1>> tlm_to_SoC;
    TriState<32> sdram_adapter;
    GENERIC_SDRAM<12, 2> sdram;
    Or1kInstructionTracer<30,32> kernel_cpu_ins_tracer;
    Or1kDataTracer<30,32> kernel_cpu_data_tracer;
    QUANTUM_INTERFACE quantum_interface;
    // Constants
    const uint32_t DEFAULT_CSR_SIZE = 0x800;

    // Trace handlers
    sc_trace_file *Tf;
    VerilatedVcdSc* tfp;
    bool tracing;

    static constexpr int SDRAM_WIDTH = 32;
    // SoC signals
    sc_signal<sc_bv<1>> soc_sysclk;
    sc_signal<sc_bv<1>> soc_rst;
    sc_signal<sc_bv<1>> soc_serial_rx;
    sc_signal<sc_bv<1>> soc_serial_tx;
    sc_signal<sc_bv<32>> soc_irq;
    sc_signal<sc_bv<30>> soc_wishbone_adr;
    sc_signal<sc_bv<32>> soc_wishbone_dat_r;
    sc_signal<sc_bv<32>> soc_wishbone_dat_w;
    sc_signal<sc_bv<4>> soc_wishbone_sel;
    sc_signal<sc_bv<1>> soc_wishbone_cyc;
    sc_signal<sc_bv<1>> soc_wishbone_stb;
    sc_signal<sc_bv<1>> soc_wishbone_ack;
    sc_signal<sc_bv<1>> soc_wishbone_we;
    sc_signal<sc_bv<3>> soc_wishbone_cti;
    sc_signal<sc_bv<2>> soc_wishbone_bte;
    sc_signal<sc_bv<1>> soc_wishbone_err;
    sc_signal<sc_bv<1>>  soc_spiflash_clk;
    sc_signal<sc_bv<1>>  soc_spiflash_cs_n;
    sc_signal<sc_bv<4>, sc_core::SC_MANY_WRITERS>  soc_spiflash_dq;
    sc_signal<sc_bv<1>> soc_ddr_cs_n;
    sc_signal<sc_bv<12>> soc_ddr_a;
    sc_signal<sc_bv<2>>  soc_ddr_ba;
    sc_signal<sc_bv<SDRAM_WIDTH>, sc_core::SC_MANY_WRITERS> soc_ddr_dq;
    sc_signal<sc_bv<SDRAM_WIDTH>> soc_ddr_dq_i;
    sc_signal<sc_bv<SDRAM_WIDTH>> soc_ddr_dq_o;
    sc_signal<sc_bv<1>> soc_ddr_dq_oe;
    sc_signal<sc_bv<SDRAM_WIDTH/8>>  soc_ddr_dm;
    sc_signal<sc_bv<1>>  soc_ddr_cke;
    sc_signal<sc_bv<1>>  soc_ddr_ras_n;
    sc_signal<sc_bv<1>>  soc_ddr_cas_n;
    sc_signal<sc_bv<1>>  soc_ddr_we_n;
    sc_signal<sc_bv<1>> soc_model_eth_source_valid;
    sc_signal<sc_bv<8>> soc_model_eth_source_data;
    sc_signal<sc_bv<1>> soc_model_eth_sink_valid;
    sc_signal<sc_bv<8>> soc_model_eth_sink_data;
    // backdoor
    sc_signal<sc_bv<32>> sdram_backdoor_copy_from;
    sc_signal<sc_bv<32>> sdram_backdoor_copy_to;
    sc_signal<sc_bv<32>> sdram_backdoor_copy_size;
    sc_signal<sc_bv<1>> sdram_backdoor_copy;
    sc_signal<sc_bv<32>> sdram_backdoor_clear_from;
    sc_signal<sc_bv<32>> sdram_backdoor_clear_to;
    sc_signal<sc_bv<1>> sdram_backdoor_clear;
    // Probes
    // ibus
    sc_signal<sc_bv<30>> wishbone_kernel_ibus_adr;
    sc_signal<sc_bv<32>> wishbone_kernel_ibus_dat_r;
    sc_signal<sc_bv<1>> wishbone_kernel_ibus_ack;
    // dbus
    sc_signal<sc_bv<30>> wishbone_kernel_dbus_adr;
    sc_signal<sc_bv<32>> wishbone_kernel_dbus_dat_r;
    sc_signal<sc_bv<32>> wishbone_kernel_dbus_dat_w;
    sc_signal<sc_bv<1>> wishbone_kernel_dbus_ack;
    sc_signal<sc_bv<1>> wishbone_kernel_dbus_we;
    // Spi Flash
    sc_signal<bool> soc_spiflash_clk_b;
    // Quantum interface - we should connect here a quantum emulator
    sc_signal<sc_bv<1>>  soc_quantum_cmd;
    sc_signal<sc_bv<1>>  soc_quantum_valid;
    sc_signal<sc_bv<32>> backend_quantum_cmd;
    sc_signal<sc_bv<1>> backend_quantum_valid;


    void setup_trace() {
        Tf = sc_create_vcd_trace_file("/workdir/trace_kasli_emulator_top");
        sc_trace(Tf, clk, "clk");
        sc_trace(Tf, rst, "rst");
        sc_trace(Tf, soc_ddr_we_n, "soc_ddr_we_n");
        sc_trace(Tf, soc_ddr_cs_n, "soc_ddr_cs_n");
        sc_trace(Tf, soc_ddr_a, "soc_ddr_a");
        sc_trace(Tf, soc_ddr_ba, "soc_ddr_ba");
        sc_trace(Tf, soc_ddr_dq, "soc_ddr_dq");
        sc_trace(Tf, soc_ddr_dm, "soc_ddr_dm");
        sc_trace(Tf, soc_ddr_cke, "soc_ddr_cke");
        sc_trace(Tf, soc_ddr_ras_n, "soc_ddr_ras_n");
        sc_trace(Tf, soc_ddr_cas_n, "soc_ddr_cas_n");

        sc_trace(Tf, soc_ddr_dq_i, "soc_ddr_dq_i");
        sc_trace(Tf, soc_ddr_dq_i, "soc_ddr_dq_o");
        sc_trace(Tf, soc_ddr_dq_oe, "soc_ddr_dq_oe");
        sc_trace(Tf, soc_spiflash_cs_n, "soc_spiflash_cs_n");
        sc_trace(Tf, soc_spiflash_dq, "soc_spiflash_dq");
        sc_trace(Tf, soc_spiflash_clk_b, "soc_spiflash_clk_b");

        // Quantum interface
        sc_trace(Tf, soc_quantum_cmd, "soc_quantum_cmd");
        sc_trace(Tf, soc_quantum_valid, "soc_quantum_valid");
        sc_trace(Tf, backend_quantum_cmd, "backend_quantum_cmd");
        sc_trace(Tf, backend_quantum_valid, "backend_quantum_valid");
     }

    void start_deep_trace() {
        if (this->tracing)
            return;
        std::cout << "Starting deep-trace" << std::endl;
        this->tracing = true;
        tfp->open("/workdir/build/SoC.vcd");
    }

    void init() {
        // Clocks
        clk_to_bv.clk(clk);
        clk_to_bv.clkout(soc_sysclk);

        // CPU
        cpu.clk.bind(clk);
        cpu.dataBus.bind(*(router.target_socket[0]));

        // ROM
        router.initiator_socket[0]->bind(reset_rom.dataBus);
        const InitiatorConfig romConfig = {0x0, 0x10000, 0xFFFFFFFF};
        router.setInitiatorProperties(0, romConfig);

        // ARTIQ_SOC
        // Enabling deep signal trace
        Verilated::traceEverOn(true);
        tfp = new VerilatedVcdSc;
        SoC.trace(tfp, 3);  // Trace 3 levels of hierarchy
        
        // begin bind every port
        SoC.irq.bind(soc_irq);
        SoC.wishbone_adr.bind(soc_wishbone_adr);
        SoC.wishbone_dat_r.bind(soc_wishbone_dat_r);
        SoC.wishbone_dat_w.bind(soc_wishbone_dat_w);
        SoC.wishbone_sel.bind(soc_wishbone_sel);
        SoC.wishbone_cyc.bind(soc_wishbone_cyc);
        SoC.wishbone_stb.bind(soc_wishbone_stb);
        SoC.wishbone_ack.bind(soc_wishbone_ack);
        SoC.wishbone_we.bind(soc_wishbone_we);
        SoC.wishbone_cti.bind(soc_wishbone_cti);
        SoC.wishbone_bte.bind(soc_wishbone_bte);
        SoC.wishbone_err.bind(soc_wishbone_err);
        SoC.serial_rx.bind(soc_serial_rx);
        SoC.serial_tx.bind(soc_serial_tx);
        SoC.sysclk.bind(soc_sysclk);
        //SoC.reset.bind(soc_rst);

        SoC.model_eth_source_valid.bind(soc_model_eth_source_valid);
        SoC.model_eth_source_data.bind(soc_model_eth_source_data);
        SoC.model_eth_sink_valid.bind(soc_model_eth_sink_valid);
        SoC.model_eth_sink_data.bind(soc_model_eth_sink_data);

        SoC.virt_spiflash_clk.bind(soc_spiflash_clk) ;
        SoC.virt_spiflash_cs_n.bind(soc_spiflash_cs_n) ;
        SoC.virt_spiflash_dq.bind(soc_spiflash_dq);

        SoC.virt_ddr_a.bind(soc_ddr_a);
        SoC.virt_ddr_ba.bind(soc_ddr_ba);
        SoC.virt_ddr_dq_i.bind(soc_ddr_dq_i);
        SoC.virt_ddr_dq_o.bind(soc_ddr_dq_o);
        SoC.virt_ddr_dq_oe.bind(soc_ddr_dq_oe);

        SoC.virt_ddr_dm.bind(soc_ddr_dm);
        SoC.virt_ddr_cke.bind(soc_ddr_cke);
        SoC.virt_ddr_ras_n.bind(soc_ddr_ras_n);
        SoC.virt_ddr_cas_n.bind(soc_ddr_cas_n);
        SoC.virt_ddr_we_n.bind(soc_ddr_we_n);
        SoC.virt_ddr_cs_n.bind(soc_ddr_cs_n);
        SoC.quantum_cmd.bind(soc_quantum_cmd);
        SoC.quantum_valid.bind(soc_quantum_valid);


        // Backdoor for SDRAM
        SoC.sdram_backdoor_copy_from.bind(sdram_backdoor_copy_from);
        SoC.sdram_backdoor_copy_to.bind(sdram_backdoor_copy_to);
        SoC.sdram_backdoor_copy_size.bind(sdram_backdoor_copy_size);
        SoC.sdram_backdoor_copy.bind(sdram_backdoor_copy);
        SoC.sdram_backdoor_clear_from.bind(sdram_backdoor_clear_from);
        SoC.sdram_backdoor_clear_to.bind(sdram_backdoor_clear_to);
        SoC.sdram_backdoor_clear.bind(sdram_backdoor_clear);

        // Probe of the kernel_cpu ibus for instruction decoding
        SoC.wishbone_kernel_ibus_adr.bind(wishbone_kernel_ibus_adr);
        SoC.wishbone_kernel_ibus_dat_r.bind(wishbone_kernel_ibus_dat_r);
        SoC.wishbone_kernel_ibus_ack.bind(wishbone_kernel_ibus_ack);
       
        // Probe of the kernel_cpu dbus for access tracing
        SoC.wishbone_kernel_dbus_adr.bind(wishbone_kernel_dbus_adr);
        SoC.wishbone_kernel_dbus_dat_r.bind(wishbone_kernel_dbus_dat_r);
        SoC.wishbone_kernel_dbus_dat_w.bind(wishbone_kernel_dbus_dat_w);
        SoC.wishbone_kernel_dbus_ack.bind(wishbone_kernel_dbus_ack);
        SoC.wishbone_kernel_dbus_we.bind(wishbone_kernel_dbus_we);
         
        // Bind external flash
        flash_clk_bv2bool.in(soc_spiflash_clk);
        flash_clk_bv2bool.out(soc_spiflash_clk_b);

        //flash_model.enable_logger();
        flash_model.cs_n.bind(soc_spiflash_cs_n);
        flash_model.dq.bind(soc_spiflash_dq);
        flash_model.clk.bind(soc_spiflash_clk_b);
        flash_model.configure_region("/workdir/build/flash_storage.img",
                                     FLASH_BOOT_ADDRESS - CONFIG_SPIFLASH_SECTOR_SIZE);
        flash_model.configure_region("/workdir/build/artiq_kasli/software/runtime/runtime.fbi",
                                     FLASH_BOOT_ADDRESS);
            
        // Currently having issues with verilated tristate for the DDR.
        sdram_adapter.din.bind(soc_ddr_dq_o);
        sdram_adapter.dout.bind(soc_ddr_dq_i);
        sdram_adapter.oe.bind(soc_ddr_dq_oe);
        sdram_adapter.dinout.bind(soc_ddr_dq);

        // TODO: generate the clock as an output of top.v
        sdram.ck.bind(soc_sysclk);
        sdram.cs_n.bind(soc_ddr_cs_n);
        sdram.a.bind(soc_ddr_a);
        sdram.ba.bind(soc_ddr_ba);
        sdram.dq.bind(soc_ddr_dq);
        sdram.dm.bind(soc_ddr_dm);
        sdram.cke.bind(soc_ddr_cke);
        sdram.ras_n.bind(soc_ddr_ras_n);
        sdram.cas_n.bind(soc_ddr_cas_n);
        sdram.we_n.bind(soc_ddr_we_n);

        sdram.backdoor_copy_from.bind(sdram_backdoor_copy_from);
        sdram.backdoor_copy_to.bind(sdram_backdoor_copy_to);
        sdram.backdoor_copy_size.bind(sdram_backdoor_copy_size);
        sdram.backdoor_copy.bind(sdram_backdoor_copy);
        sdram.backdoor_clear_from.bind(sdram_backdoor_clear_from);
        sdram.backdoor_clear_to.bind(sdram_backdoor_clear_to);
        sdram.backdoor_clear.bind(sdram_backdoor_clear);
        
        // For tracing the instruction bus
        kernel_cpu_ins_tracer.clk.bind(soc_sysclk);
        kernel_cpu_ins_tracer.ibus_adr.bind(wishbone_kernel_ibus_adr);
        kernel_cpu_ins_tracer.ibus_dat.bind(wishbone_kernel_ibus_dat_r);
        kernel_cpu_ins_tracer.ibus_ack.bind(wishbone_kernel_ibus_ack);

        // For tracing the data bus
        kernel_cpu_data_tracer.clk.bind(soc_sysclk);
        kernel_cpu_data_tracer.dbus_adr.bind(wishbone_kernel_dbus_adr);
        kernel_cpu_data_tracer.dbus_dat_w.bind(wishbone_kernel_dbus_dat_w);
        kernel_cpu_data_tracer.dbus_dat_r.bind(wishbone_kernel_dbus_dat_r);
        kernel_cpu_data_tracer.dbus_ack.bind(wishbone_kernel_dbus_ack);
        kernel_cpu_data_tracer.dbus_we.bind(wishbone_kernel_dbus_we);

        // We are preconfiguring the SRAM with the firmware to save time on the loading. 
        sdram.configure_region("/workdir/build/artiq_kasli/software/runtime/runtime.bin", 0x0, 0);

        // TLM peripherals
        router.initiator_socket[1]->bind(timer.dataBus);
        router.setInitiatorProperties(1, InitiatorConfig{CSR_TIMER0_BASE, CSR_TIMER0_BASE+DEFAULT_CSR_SIZE-4, 0xFFFFFFFF});

        router.initiator_socket[2]->bind(version.dataBus);
        router.setInitiatorProperties(2, InitiatorConfig{CSR_IDENTIFIER_BASE, CSR_IDENTIFIER_BASE+DEFAULT_CSR_SIZE-4, 0xFFFFFFFF});

        router.initiator_socket[3]->bind(console.dataBus);
        router.setInitiatorProperties(3, InitiatorConfig{CSR_UART_BASE, CSR_UART_BASE+DEFAULT_CSR_SIZE-4, 0xFFFFFFFF});

        router.initiator_socket[4]->bind(tlm_to_SoC.tlm_socket);
        tlm_to_SoC.clk(clk);
        tlm_to_SoC.rst(rst);
        tlm_to_SoC.adr_o(soc_wishbone_adr);
        tlm_to_SoC.dat_o(soc_wishbone_dat_w);
        tlm_to_SoC.we_o(soc_wishbone_we) ;
        tlm_to_SoC.sel_o(soc_wishbone_sel);
        tlm_to_SoC.stb_o(soc_wishbone_stb);
        tlm_to_SoC.cyc_o(soc_wishbone_cyc);
        tlm_to_SoC.ack_i(soc_wishbone_ack);
        tlm_to_SoC.dat_i(soc_wishbone_dat_r);

        // This will catch all the other accesses. It needs to be the last initiator in the list.
        router.setInitiatorProperties(4, InitiatorConfig{(uint32_t)0, (uint32_t)0xFFFFFFFF, 0xFFFFFFFF});

        soc_rst.write(0);
        // Binding HAL interface
        SoC.cmd.bind(hal_cmd);
        SoC.cmd_valid.bind(hal_cmd_valid);
        SoC.cmd_ready.bind(hal_cmd_ready);
        SoC.readout.bind(hal_readout);
        SoC.readout_valid.bind(hal_readout_valid) ;
        SoC.readout_ready.bind(hal_readout_ready) ;

        // Binding quantum interface
        quantum_interface.quantum_in_cmd.bind(soc_quantum_cmd);
        quantum_interface.quantum_in_valid.bind(soc_quantum_valid);
        quantum_interface.quantum_out_cmd.bind(backend_quantum_cmd);
        quantum_interface.quantum_out_valid.bind(backend_quantum_valid);
    }

    ARTIQ_EMULATOR(sc_module_name name, float CLK_PERIOD)
        : sc_module(name),
          clk_to_bv("clk_to_bv"),
          router("router", "/workdir/build/logs/router_tlm.csv"), 
          cpu("cpu", "/workdir/emulator/or1ksim-kasli.cfg",
                    "/workdir/build/artiq_kasli/software/bootloader/bootloader.elf",
                    sc_time(CLK_PERIOD, SC_NS), sc_time{100, SC_NS}),
          reset_rom("bootRom", 0x100, "/workdir/build/textrst.bin"),
          SoC("ARTIQ_SoC"),
          flash_clk_bv2bool("flash_clk_to_bool"),
          flash_model("flash_model", CONFIG_SPIFLASH_DUMMY_CLKS, "/workdir/build/logs/flash.csv"),
          timer("timer", CSR_TIMER0_BASE, CONFIG_CLOCK_FREQUENCY),
          version("version"),
          console("console", CSR_UART_BASE),
          tlm_to_SoC("tlm_to_SoC"),
          sdram_adapter("sdram_tristate"),
          sdram("sdram", SDRAM_GEOM{.row_bits=12, .bank_bits=2, .col_bits=8}, "/workdir/build/logs/sdram.csv"),
          kernel_cpu_ins_tracer("kernel_cpu_ins_tracer", "/workdir/build/logs/kernel_cpu_instruction.log"),
          kernel_cpu_data_tracer("kernel_cpu_data_tracer", "/workdir/build/logs/kernel_cpu_data.csv"),
          quantum_interface("quantum_interface")
    {
        init();
        setup_trace();
    }

    ~ARTIQ_EMULATOR() {
        sdram.hexdump("/workdir/build/logs/sdram.dump");
        sc_close_vcd_trace_file(Tf);
        if (tfp) {
            tfp->close();
        }
    }
};


#endif
