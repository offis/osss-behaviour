#ifndef PIPED_ARRAY_COMMUNICATION_EXAMPLE_INCLUDED
#define PIPED_ARRAY_COMMUNICATION_EXAMPLE_INCLUDED

#include "osss_behaviour.h"

#define USE_CTHREAD

OSSS_PIPELINE_STAGE(Behaviour1)
{
  osss::osss_out<osss::osss_array<unsigned int, 10> > array_out;
  
  PIPELINE_STAGE_CTOR(Behaviour1) {}

  virtual void main()
  {
    wait(sc_core::SC_ZERO_TIME);

    OSSS_PRINT_TIME_STAMP(); 
    for(unsigned int i=0; i<10; ++i)
      array_out[i] = i;
  }
    
};

OSSS_PIPELINE_STAGE(Behaviour2)
{
  osss::osss_in<osss::osss_array<unsigned int, 10> > array_in; 

  PIPELINE_STAGE_CTOR(Behaviour2) {}

  virtual void main()
  {
    wait(sc_core::SC_ZERO_TIME);

    OSSS_PRINT_TIME_STAMP();
    for(unsigned int i=0; i<10; ++i)
      std::cout << array_in[i] << " ";
    std::cout << std::endl;
  }
    
};

OSSS_PIPELINE_STAGE(Behaviour3)
{

  PIPELINE_STAGE_CTOR(Behaviour3) {}

  virtual void main()
  {
    wait(sc_core::SC_ZERO_TIME);

    OSSS_PRINT_TIME_STAMP();
  }
    
};


//--------------------------------------------------------------------

OSSS_BEHAVIOUR(B_par)
{
 public:
  Behaviour1 b1; 
  Behaviour2 b2;
  Behaviour3 b3;

 protected:
  osss::osss_shared_variable<osss::osss_array<unsigned int, 10>, 1> piped_array;
  unsigned int m_count;

 public:
  BEHAVIOUR_CTOR(B_par) :
    b1("b1"),
    b2("b2"),
    b3("b3"),
    m_count(0)
  {
    b1.array_out(piped_array);
    b2.array_in(piped_array);
    
    //osss_pipe(3,  b1 >> b2 >> b3 );
    // or (with the stop_condition() below)
    osss_pipe( b1 >> b2 >> b3 );
  }

  virtual void init() { OSSS_PRINT_TIME_STAMP(); }

  virtual void pipe_post()
  {
    m_count++;
  }

  virtual bool pipe_stop_condition() const
  {
    return (m_count == 2);
  }

  virtual void final() { OSSS_PRINT_TIME_STAMP(); }

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
