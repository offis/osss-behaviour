#include "osss_behaviour.h"
#include "tb.h"

SC_MODULE(Top)
{
  sc_in<bool> clk;
  sc_in<bool> rst;

  Main m_main;

  SC_CTOR(Top) :
    m_main("m_main")
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


int sc_main(int, char**)
{ 
  sc_signal<bool> dummy;

  Top top("top");
  top.clk(dummy);
  top.rst(dummy);

  sc_start();

  return 0;
}
