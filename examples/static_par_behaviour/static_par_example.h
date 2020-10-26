#include "osss_behaviour.h"

#define USE_CTHREAD

OSSS_BEHAVIOUR(Behaviour1)
{
  
  BEHAVIOUR_CTOR(Behaviour1) {}

  virtual void main()
  {
    #if defined(USE_CTHREAD)
      OSSS_PRINT_TIME_STAMP_WITH_TEXT( "--> Reset" );
    #endif

    wait();

    OSSS_PRINT_TIME_STAMP();  
  }
    
};

OSSS_BEHAVIOUR(Behaviour2)
{

  BEHAVIOUR_CTOR(Behaviour2) {}

  virtual void main()
  {
    #if defined(USE_CTHREAD)
      OSSS_PRINT_TIME_STAMP_WITH_TEXT( "--> Reset" );
    #endif

    wait();

    OSSS_PRINT_TIME_STAMP();
  }
    
};

OSSS_BEHAVIOUR(Behaviour3)
{

  BEHAVIOUR_CTOR(Behaviour3) {}

  virtual void main()
  {
    #if defined(USE_CTHREAD)
      OSSS_PRINT_TIME_STAMP_WITH_TEXT( "--> Reset" );
    #endif

    wait();

    OSSS_PRINT_TIME_STAMP();
  }
    
};

//------------------------------------------------------------------------------

SC_MODULE(B_par)
{
 sc_in<bool> clk;
 sc_in<bool> rst;

 Behaviour1 b1; 
 Behaviour2 b2; 
 Behaviour3 b3;

 SC_CTOR(B_par) :
   clk("clk"),
   rst("rst"),
   b1("b1"),
   b2("b2"),
   b3("b3")
 {
#if defined(USE_CTHREAD)
   // this process declaration does not work because
   // b1.main() is not a member function of this class
   //SC_CTHREAD(b1.main, clk.pos());
   OSSS_STATIC_CTHREAD(Behaviour1, b1, clk.pos());
   reset_signal_is(rst, true);
   b1.set_sensitivity(clk.pos());
#else
   // this process declaration does not work because
   // b1.main() is not a member function of this class
   //SC_THREAD(b1.main);
   OSSS_STATIC_THREAD(Behaviour1, b1);
#endif

#if defined(USE_CTHREAD)
   //SC_CTHREAD(b2.main, clk.pos());
   OSSS_STATIC_CTHREAD(Behaviour2, b2, clk.pos());
   reset_signal_is(rst, true);
   b2.set_sensitivity(clk.pos());
#else
   //SC_THREAD(b2.main);
   OSSS_STATIC_THREAD(Behaviour2, b2);
#endif

#if defined(USE_CTHREAD)
   //SC_CTHREAD(b3.main, clk.pos());
   OSSS_STATIC_CTHREAD(Behaviour3, b3, clk.pos());
   reset_signal_is(rst, true);
   b3.set_sensitivity(clk.pos());
#else
   //SC_THREAD(b3.main);
   OSSS_STATIC_THREAD(Behaviour3, b3);
#endif

   osss::osss_global_port_registry::register_clock_port(clk);
   osss::osss_global_port_registry::register_reset_port(rst);

 }

};
