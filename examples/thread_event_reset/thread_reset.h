#ifndef THREAD_RESET_INCLUDED
#define THREAD_RESET_INCLUDED

#include "osss_behaviour.h"

OSSS_MODULE(Resetable)
{
  osss::osss_event_in rst_ev_0;
  osss::osss_event_in rst_ev_1;

  OSSS_CTOR(Resetable) :
    rst_ev_0("rst_ev_0"),
    rst_ev_1("rst_ev_1")
 {
   OSSS_THREAD(blub);
   reset_event_is(rst_ev_0);
   OSSS_THREAD(bla);
   reset_event_is(rst_ev_1);
 }
  
 void blub() 
 {
   // reset
   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "--> Reset" );
   wait();

   while(true)
   {
     OSSS_PRINT_TIME_STAMP_WITH_TEXT( "in main loop" );
     wait(12);
   }
 }

 void bla()
 {
   // reset
   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "--> Reset" );
   wait();

   while(true)
   {
     OSSS_PRINT_TIME_STAMP_WITH_TEXT( "in main loop" );
     wait(12);
   }
 }

};

OSSS_MODULE(Resetter)
{
  osss::osss_event_out rst_ev_0;
  osss::osss_event_out rst_ev_1;

  OSSS_CTOR(Resetter) : 
    rst_ev_0("rst_ev_0"),
    rst_ev_1("rst_ev_1")
  {
    OSSS_THREAD(rst_proc_0);
    OSSS_THREAD(rst_proc_1);
  }

  void rst_proc_0()
  {
    while(true)
    {
      wait(1);
      rst_ev_0.notify(sc_core::SC_ZERO_TIME);
      // problem with rst_ev_0.notify()
      wait(120);
      rst_ev_0.notify(sc_core::sc_time(120, sc_core::SC_NS));
      wait(121);
      rst_ev_0.notify(sc_core::sc_time(120, sc_core::SC_NS));
      wait(12000);
    }
  }

  void rst_proc_1()
  {
    while(true)
    {
      wait(1);
      rst_ev_1.notify(sc_core::SC_ZERO_TIME);
      // problem with rst_ev_1.notify()
      wait(120);
      rst_ev_1.notify(sc_core::sc_time(120, sc_core::SC_NS));
      wait(121);
      rst_ev_1.notify(sc_core::sc_time(120, sc_core::SC_NS));
      wait(12000);
    }
  }

};

#endif
