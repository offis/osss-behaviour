#include "osss_behaviour.h"

#include "Testbench.h"
#include "tb.h"

SC_MODULE(Top)
{
  sc_in<bool> clk;
  sc_in<bool> rst;

  Main m_main;

  SC_HAS_PROCESS(Top);

  Top(sc_core::sc_module_name name,
      const std::string& in_file_name, 
      const std::string& out_file_name) :
    m_main("m_main", in_file_name, out_file_name)
  {
    SC_THREAD(main);
#ifdef USE_CLOCK
    // make top-level behaviour m_main sensitive to clock
    m_main.set_sensitivity(clk.pos());
#endif

    osss::osss_global_port_registry::register_clock_port(clk);
    osss::osss_global_port_registry::register_reset_port(rst);
  }

  virtual void end_of_elaboration()
  {
    m_main.print_graph("graph.dot", true);
  }

  void main()
  {
    m_main.main();
  }

};

int sc_main (int argc, char** argv)
{ 
  std::string ifname;
  std::string ofname;

  // Command line arguments
  if (argc < 2) {
    error("Usage: %s infile [ outfile ]\n", argv[0]);
  }
  ifname += std::string(argv[1]);
  if (argc >= 3) {
    ofname += std::string(argv[2]);
  } else {
    ofname += ifname + std::string("_out.jpg");
  }

  sc_signal<bool> clk_signal;
  sc_signal<bool> rst_signal;

#ifdef USE_CLOCK
  // clock generator
  Testbench testbench("testbench");
  testbench.po_bClk(clk_signal);
  testbench.po_bReset(rst_signal);
#endif

  Top top("top", ifname, ofname);
  top.clk(clk_signal);
  top.rst(rst_signal);

  // bus signal tracing
#ifdef JPEG_BUS_TRACE
  sc_trace_file* tf = sc_create_vcd_trace_file("bus_trace");
#ifdef USE_CLOCK
  sc_trace(tf, clk_signal, "clock");
  sc_trace(tf, rst_signal, "reset");
#endif

  sc_trace(tf, top.m_main.design.MWDATA, "MWDATA"); // coldfire write data bus
  sc_trace(tf, top.m_main.design.MRDATA, "MRDATA"); // coldfire read data bus
  sc_trace(tf, top.m_main.design.MADDR, "MADDR");   // address
  sc_trace(tf, top.m_main.design.DB, "DB");	    // dct data bus
  sc_trace(tf, top.m_main.design.MTSB, "MTSB");     // coldfire-transducer control
  sc_trace(tf, top.m_main.design.MTAB, "MTAB");
  sc_trace(tf, top.m_main.design.MWDATAOE, "MWDATAOE");
  sc_trace(tf, top.m_main.design.TSB, "TSB");       // transducer-dct control
  sc_trace(tf, top.m_main.design.TAB, "TAB");
  sc_trace(tf, top.m_main.design.DBOE, "DBOE");
  sc_trace(tf, top.m_main.design.MRWB, "MRWB");
  sc_trace(tf, top.m_main.design.INTC, "INTC");     // coldfire-dct control
#endif
  
  // start the simulation
  sc_start();
  
#ifdef JPEG_BUS_TRACE
  sc_close_vcd_trace_file(tf);
#endif
  
  return 0;
}
