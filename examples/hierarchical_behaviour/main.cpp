#include "hierarchy_example.h"
#include "Testbench.h"
#include "StimuliGen.h"

int sc_main(int, char**)
{
 
  Top* top = new Top("top");

  Testbench testbench("testbench");
  StimuliGen stimuligen("StimuliGen");
  sc_signal<bool> s_clock;
  sc_signal<bool> s_reset;

  sc_signal<int> s_a;
 
  top->clk(s_clock);
  top->rst(s_reset);
  top->a(s_a);
   
  testbench.po_bClk(s_clock);
  testbench.po_bReset(s_reset);

  stimuligen.clk(s_clock);
  stimuligen.rst(s_reset);
  stimuligen.a(s_a);

  //sc_start(18*testbench.GetClockCyclePeriod());
  sc_start();

  return 0;
}
