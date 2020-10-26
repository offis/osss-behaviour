#include "fsm_example.h"
#include "Testbench.h"
#include "StimuliGen.h"

int sc_main(int, char**)
{
 
  Top top("top");

  Testbench testbench("testbench");
  StimuliGen stimuligen("StimuliGen");
  sc_signal<bool> s_clock;
  sc_signal<bool> s_reset;

  sc_signal<int> s_a, s_b;
 
  top.clk(s_clock);
  top.rst(s_reset);
  top.a(s_a);
  top.b(s_b);
  
  testbench.po_bClk(s_clock);
  testbench.po_bReset(s_reset);

  stimuligen.clk(s_clock);
  stimuligen.rst(s_reset);
  stimuligen.a(s_a);
  stimuligen.b(s_b);

  //sc_start(18*testbench.GetClockCyclePeriod());
  sc_start();

  return 0;
}
