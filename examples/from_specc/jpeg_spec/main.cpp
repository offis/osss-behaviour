#include "osss_behaviour.h"

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


int sc_main(int argc, char** argv)
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

  sc_signal<bool> dummy;

  Top top("top", ifname, ofname);
  top.clk(dummy);
  top.rst(dummy);

  sc_start();

  return 0;
}
