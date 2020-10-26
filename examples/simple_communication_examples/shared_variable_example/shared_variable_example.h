#include "osss_behaviour.h"

//#define USE_CTHREAD

OSSS_BEHAVIOUR(Sender)
{
  osss::osss_out<float> data_out;

  Sender(sc_core::sc_module_name name,
         osss::osss_event& req,
         const osss::osss_event& ack) : 
    osss::osss_behaviour(name),
    Req(req),
    Ack(ack)
    {}

  virtual void main()
  {
    float X = 0;
    
    while(true)
    {
      data_out = X;
      OSSS_PRINT_TIME_STAMP_WITH_TEXT( "Data = " << X );
      Req.notify(SC_ZERO_TIME);
      wait(Ack);
      X++;
    }  
  }

 protected:
  osss::osss_event& Req;
  const osss::osss_event& Ack;    
};

OSSS_BEHAVIOUR(Receiver)
{
  osss::osss_in<float> data_in;

  Receiver(sc_core::sc_module_name name,
           const osss::osss_event& req,
           osss::osss_event& ack) : 
    osss::osss_behaviour(name),
    Req(req),
    Ack(ack)
    {}

  virtual void main()
  {
    float Y;

    while(true)
    {
      wait(Req);
      Y = data_in;
      OSSS_PRINT_TIME_STAMP_WITH_TEXT( "Data = " << Y );
      if (Y == 10)
        sc_core::sc_stop();

      Ack.notify(SC_ZERO_TIME);
    }
  }

 protected:
  const osss::osss_event& Req;
  osss::osss_event& Ack;  
};

//------------------------------------------------------------------------------

OSSS_BEHAVIOUR(B)
{
  osss::osss_event Req, Ack;
  osss::osss_shared_variable<float> Data;

  Sender   S; 
  Receiver R;

  BEHAVIOUR_CTOR(B) :
    S("S", Req, Ack),
    R("R", Req, Ack)
  {
    S.data_out(Data);
    R.data_in(Data);
    
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

