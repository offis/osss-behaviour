#ifndef HANDSHAKE_EXAMPLE_INCLUDED
#define HANDSHAKE_EXAMPLE_INCLUDED

#include "osss_behaviour.h"

#define USE_CTHREAD

OSSS_BEHAVIOUR(Behaviour1)
{
  osss::osss_port<osss::osss_send_if> ch_out;
  
  BEHAVIOUR_CTOR(Behaviour1) {}

  virtual void main()
  {
    for(unsigned int i=0; i<10; ++i)
    {
      OSSS_PRINT_TIME_STAMP_WITH_TEXT("at index: " << i);
      ch_out->send();
      wait();
    }
  }
    
};

OSSS_BEHAVIOUR(Behaviour2)
{
  osss::osss_port<osss::osss_receive_if> ch_in; 

  BEHAVIOUR_CTOR(Behaviour2) {}

  virtual void main()
  {
    for(unsigned int i=0; i<10; ++i)
    {
      ch_in->receive();
      wait();
      OSSS_PRINT_TIME_STAMP_WITH_TEXT("at index: " << i);
    }
  }
    
};

//--------------------------------------------------------------------

OSSS_BEHAVIOUR(B_par)
{
  Behaviour1 b1; 
  Behaviour2 b2;

  osss::osss_handshake_channel ch;

  BEHAVIOUR_CTOR(B_par) :
    b1("b1"),
    b2("b2")
  {
    b1.ch_out(ch);
    b2.ch_in(ch);
    
    osss_par( b1 | b2);
  }
};

//--------------------------------------------------------------------

SC_MODULE(Top)
{
  sc_in<bool> clk;
  sc_in<bool> rst;

  B_par b_par;

  SC_CTOR(Top) :
   clk("clk"),
   rst("rst"),
   b_par("b_par")
 {
#if defined(USE_CTHREAD)
   SC_CTHREAD(main, clk.pos());
   reset_signal_is(rst, true);

   b_par.set_sensitivity(clk.pos());
#else
   SC_THREAD(main);
#endif

   // register clock and reset globally
   osss::osss_global_port_registry::register_clock_port(clk);
   osss::osss_global_port_registry::register_reset_port(rst);
 }

  virtual void end_of_elaboration()
  {
    b_par.print_graph(std::string("graph.dot"), true);
  }

 void main() 
 {
#if defined(USE_CTHREAD)
   // reset
   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "--> Reset" );
  
   wait();
#endif

   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "before parallel behaviour" );
  
   // call parallel behaviour
   b_par.main();

   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "after parallel behaviour" ); 
 }
};

#endif
