#include "static_par_example.h"
#include "Testbench.h"

int sc_main(int, char**)
{
  B_par b_par("B_par");

  Testbench testbench("testbench");
  sc_signal<bool> s_clock;
  sc_signal<bool> s_reset;
 
  b_par.clk(s_clock);
  b_par.rst(s_reset);
  
  testbench.po_bClk(s_clock);
  testbench.po_bReset(s_reset);

  sc_start(100*testbench.GetClockCyclePeriod());

  return 0;
}
