#include "osss_behaviour.h"

//#define USE_CTHREAD

OSSS_BEHAVIOUR(Sender)
{

  Sender(sc_core::sc_module_name name,
         osss::osss_event& req,
         float& data,
         const osss::osss_event& ack) : 
    osss::osss_behaviour(name),
    Req(req),
    Data(data),
    Ack(ack)
    {}

  virtual void main()
  {
    float X = 0;
    
    while(true)
    {
      Data = X;
      OSSS_PRINT_TIME_STAMP_WITH_TEXT( "Data = " << X );
      Req.notify(SC_ZERO_TIME);
      wait(Ack);
      X++;
    }
  }

 protected:

  osss::osss_event& Req;
  float& Data;
  const osss::osss_event& Ack;
    
};

OSSS_BEHAVIOUR(Receiver)
{

  Receiver(sc_core::sc_module_name name,
           const osss::osss_event& req,
           const float& data,
           osss::osss_event& ack) : 
    osss::osss_behaviour(name),
    Req(req),
    Data(data),
    Ack(ack)
    {}

  virtual void main()
  {
    float Y;

    while(true)
    {
      wait(Req);
      Y = Data;
      OSSS_PRINT_TIME_STAMP_WITH_TEXT( "Data = " << Y );
      if (Y == 10)
        sc_core::sc_stop();

      Ack.notify(SC_ZERO_TIME);
    }
  }

 protected:

  const osss::osss_event& Req;
  const float& Data;
  osss::osss_event& Ack;
  
};

//------------------------------------------------------------------------------

OSSS_BEHAVIOUR(B)
{
  
 osss::osss_event Req, Ack;
 float Data;

 Sender   S; 
 Receiver R;

 BEHAVIOUR_CTOR(B) :
   S("S", Req, Data, Ack),
   R("R", Req, Data, Ack)
 {
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

