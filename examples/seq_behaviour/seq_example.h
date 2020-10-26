#include "osss_behaviour.h"

#define USE_CTHREAD

OSSS_BEHAVIOUR(Behaviour1)
{
  BEHAVIOUR_CTOR(Behaviour1) {}

  virtual void main()
  {
    OSSS_PRINT_TIME_STAMP();
    
    wait();
  }
    
};

OSSS_BEHAVIOUR(Behaviour2)
{
  BEHAVIOUR_CTOR(Behaviour2) {}

  virtual void main()
  {
    OSSS_PRINT_TIME_STAMP();
    
    wait();
  }
    
};

OSSS_BEHAVIOUR(Behaviour3)
{
  BEHAVIOUR_CTOR(Behaviour3) {}

  virtual void main()
  {
    OSSS_PRINT_TIME_STAMP();
    
    wait();
  }
    
};

//--------------------------------------------------------------------

OSSS_BEHAVIOUR(B_seq)
{
  Behaviour1 b1; 
  Behaviour2 b2; 
  Behaviour3 b3;

  BEHAVIOUR_CTOR(B_seq) :
    b1("b1"),
    b2("b2"),
    b3("b3")
  {
    osss_seq(b1, b2, b3);
  }
};

//--------------------------------------------------------------------

SC_MODULE(Top)
{
  sc_in<bool> clk;
  sc_in<bool> rst;

  B_seq b_seq;

  SC_CTOR(Top) :
   clk("clk"),
   rst("rst"),
   b_seq("b_seq")
 {
#if defined(USE_CTHREAD)
   SC_CTHREAD(main, clk.pos());
   reset_signal_is(rst, true);

   b_seq.set_sensitivity(clk.pos());
#else
   SC_THREAD(main);
#endif
   // register clock and reset globally
   osss::osss_global_port_registry::register_clock_port(clk);
   osss::osss_global_port_registry::register_reset_port(rst);
 }

 virtual void end_of_elaboration()
 {
   b_seq.print_graph(std::string("graph.dot"), true);
 }

 void main() 
 {
#if defined(USE_CTHREAD)
   // reset
   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "--> Reset" );
  
   wait();
#endif

   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "before sequential behaviour" );

   // call sequential behaviour
   b_seq.main();
  
   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "after sequential behaviour" );
 }
};

