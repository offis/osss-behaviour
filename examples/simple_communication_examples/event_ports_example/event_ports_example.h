#include "osss_behaviour.h"

//#define USE_CTHREAD

OSSS_BEHAVIOUR(Sender)
{
  osss::osss_out<float> data_out;
  osss::osss_event_out Req_out;
  osss::osss_event_in  Ack_in;

  BEHAVIOUR_CTOR(Sender) {}

  virtual void main()
  {
    float X = 0;
    
    while(true)
    {
      data_out = X;
      OSSS_PRINT_TIME_STAMP_WITH_TEXT( "Data = " << X );
      Req_out.notify(SC_ZERO_TIME);
      wait(Ack_in);
      X++;
    }
  }
    
};

OSSS_BEHAVIOUR(Receiver)
{
  osss::osss_in<float> data_in;
  osss::osss_event_in  Req_in;
  osss::osss_event_out Ack_out;

  BEHAVIOUR_CTOR(Receiver) {}

  virtual void main()
  {
    float Y;

    while(true)
    {
      wait(Req_in);
      Y = data_in;
      OSSS_PRINT_TIME_STAMP_WITH_TEXT( "Data = " << Y );
      if (Y == 10)
        sc_core::sc_stop();

      Ack_out.notify(SC_ZERO_TIME);
    }
  }
  
};

//------------------------------------------------------------------------------

OSSS_BEHAVIOUR(B)
{
  osss::osss_event_channel Req, Ack;
  osss::osss_shared_variable<float> Data;

  Sender   S; 
  Receiver R; 

  BEHAVIOUR_CTOR(B) :
    S("S"),
    R("R")
  {
    S.data_out(Data);
    S.Req_out(Req);
    S.Ack_in(Ack); 

    R.data_in(Data);
    R.Req_in(Req);
    R.Ack_out(Ack);

    osss_par(S | R);
  }
};

//------------------------------------------------------------------------------

SC_MODULE(Top)
{
 sc_in<bool> clk;
 sc_in<bool> rst;

 B b;

 SC_CTOR(Top) :
   clk("clk"),
   rst("rst"),
   b("b")
 {
#if defined(USE_CTHREAD)
   SC_CTHREAD(main, clk.pos());
   reset_signal_is(rst, true);
#else
   SC_THREAD(main);
#endif

   osss::osss_global_port_registry::register_clock_port(clk);
   osss::osss_global_port_registry::register_reset_port(rst);

 }

 void main() 
 {
#if defined(USE_CTHREAD)
   // reset
   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "--> Reset" );
   wait();
#endif

   b.main();
 }

};

