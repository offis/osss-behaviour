#include "event_ports_example.h"
#include "Testbench.h"

int sc_main(int, char**)
{
  Top top("top");

  Testbench testbench("testbench");
  sc_signal<bool> s_clock;
  sc_signal<bool> s_reset;
 
  top.clk(s_clock);
  top.rst(s_reset);
  
  testbench.po_bClk(s_clock);
  testbench.po_bReset(s_reset);

  sc_start(1000*testbench.GetClockCyclePeriod());

  return 0;
}
