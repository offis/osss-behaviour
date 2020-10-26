#ifndef ARRAY_COMMUNICATION_EXAMPLE_INCLUDED
#define ARRAY_COMMUNICATION_EXAMPLE_INCLUDED

#include "osss_behaviour.h"

#define USE_CTHREAD

OSSS_BEHAVIOUR(Behaviour1)
{
  osss::osss_out<osss::osss_array<unsigned int, 10> > array_out;
  
  BEHAVIOUR_CTOR(Behaviour1) {}

  virtual void main()
  {
    for(unsigned int i=0; i<10; ++i)
      array_out[i] = i;

    OSSS_PRINT_TIME_STAMP();

    wait();
  }
    
};

OSSS_BEHAVIOUR(Behaviour2)
{
  osss::osss_in<osss::osss_array<unsigned int, 10> > array_in; 

  BEHAVIOUR_CTOR(Behaviour2) {}

  virtual void main()
  {
    wait();

    for(unsigned int i=0; i<10; ++i)
      std::cout << array_in[i] << " ";
    std::cout << std::endl;

    OSSS_PRINT_TIME_STAMP();
  }
    
};

//--------------------------------------------------------------------

OSSS_BEHAVIOUR(B_par)
{
  Behaviour1 b1; 
  Behaviour2 b2;

  osss::osss_shared_variable<osss::osss_array<unsigned int, 10> > array;

  BEHAVIOUR_CTOR(B_par) :
    b1("b1"),
    b2("b2")
  {
    b1.array_out(array);
    b2.array_in(array);
    
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
