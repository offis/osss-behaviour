#ifndef THREAD_RESET_INCLUDED
#define THREAD_RESET_INCLUDED

#include "osss_behaviour.h"

OSSS_MODULE(Resetable)
{
  sc_in<bool> rst_0;
  sc_in<bool> rst_1;

  OSSS_CTOR(Resetable) :
    rst_0("rst_0"),
    rst_1("rst_1")
 {
   OSSS_THREAD(blub);
   reset_signal_is(rst_0, true);
   OSSS_THREAD(bla);
   reset_signal_is(rst_1, true);
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
  sc_out<bool> rst_0;
  sc_out<bool> rst_1;

  OSSS_CTOR(Resetter) : 
    rst_0("rst_0"),
    rst_1("rst_1")
  {
    OSSS_THREAD(rst_proc_0);
    OSSS_THREAD(rst_proc_1);
  }

  void rst_proc_0()
  {
    rst_0 = 0;
    wait();

    while(true)
    {
      rst_0 = 1;
      wait();
      rst_0 = 0;
      wait(120);
      rst_0 = 1;
      wait();
      rst_0 = 0;
      wait(121);
      rst_0 = 1;
      wait();
      rst_0 = 0;
      wait(12000);
    }
  }

  void rst_proc_1()
  {
    rst_1 = 0;
    wait();

    while(true)
    {
      rst_1 = 1;
      wait();
      rst_1 = 0;
      wait(120);
      rst_1 = 1;
      wait();
      rst_1 = 0;
      wait(121);
      rst_1 = 1;
      wait();
      rst_1 = 0;
      wait(12000);
    }
  }

};

#endif
