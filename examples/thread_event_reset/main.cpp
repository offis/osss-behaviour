#include "thread_reset.h"

int sc_main(int, char**)
{
  Resetable resetable("resetable");
  Resetter  resetter("resetter");

  osss::osss_event_channel rst_ev_0_ch;
  osss::osss_event_channel rst_ev_1_ch;

  resetable.rst_ev_0(rst_ev_0_ch);
  resetable.rst_ev_1(rst_ev_1_ch);

  resetter.rst_ev_0(rst_ev_0_ch);
  resetter.rst_ev_1(rst_ev_1_ch);
  
  sc_start(sc_time(1200, SC_NS));

  return 0;
}
