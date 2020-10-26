#include "thread_reset.h"

int sc_main(int, char**)
{
  Resetable resetable("resetable");
  Resetter  resetter("resetter");

  sc_signal<bool> rst_signal_0;
  sc_signal<bool> rst_signal_1;

  resetable.rst_0(rst_signal_0);
  resetable.rst_1(rst_signal_1);

  resetter.rst_0(rst_signal_0);
  resetter.rst_1(rst_signal_1);
  
  sc_start(sc_time(1200, SC_NS));

  return 0;
}
