#ifndef HIERARCHICAL_PIPELINE_EXAMPLE_INCLUDED
#define HIERARCHICAL_PIPELINE_EXAMPLE_INCLUDED

#include "osss_behaviour.h"

#define USE_CTHREAD

OSSS_PIPELINE_STAGE(InnerBehaviour1)
{
  osss::osss_in<unsigned int> in_port; 

  PIPELINE_STAGE_CTOR(InnerBehaviour1) {}

  virtual void main()
  {
    wait(sc_core::SC_ZERO_TIME);

    std::cout << "InnerBehaviour1 in_port: " << in_port.read() << std::endl;

    OSSS_PRINT_TIME_STAMP();
  }
    
};

OSSS_PIPELINE_STAGE(InnerBehaviour2)
{

  PIPELINE_STAGE_CTOR(InnerBehaviour2) {}

  virtual void main()
  {
    wait(sc_core::SC_ZERO_TIME);

    OSSS_PRINT_TIME_STAMP();
  }
    
};

OSSS_PIPELINE_STAGE(InnerBehaviour3)
{

  PIPELINE_STAGE_CTOR(InnerBehaviour3) {}

  virtual void main()
  {
    wait(sc_core::SC_ZERO_TIME);

    OSSS_PRINT_TIME_STAMP();
  }
    
};


OSSS_PIPELINE_STAGE(Behaviour1)
{
  osss::osss_out<unsigned int> out_port;
  
  unsigned int i;

  PIPELINE_STAGE_CTOR(Behaviour1) :
    i(0)
  {}

  virtual void main()
  {
    wait(sc_core::SC_ZERO_TIME);

    out_port = i;
    std::cout << "Behaviour1 out_port: " << i << std::endl;

    OSSS_PRINT_TIME_STAMP(); 
    
    i++;
  }
    
};

OSSS_PIPELINE_STAGE(Behaviour2)
{
  osss::osss_in<unsigned int> in_port; 
  
  InnerBehaviour1 ib1;
  InnerBehaviour2 ib2;
  InnerBehaviour3 ib3;

  unsigned int m_count;

  PIPELINE_STAGE_CTOR(Behaviour2) :
    ib1("ib1"),
    ib2("ib2"),
    ib3("ib3")
  {
    ib1.in_port(in_port);

    osss_pipe( ib1 >> ib2 >> ib3 );
  }

  virtual void init() 
  { 
    OSSS_PRINT_TIME_STAMP(); 
    m_count = 0;
  }

  virtual void pipe_post()
  {
    m_count++;
  }

  virtual bool pipe_stop_condition() const
  {
    return (m_count == 3);
  }

  virtual void final() { OSSS_PRINT_TIME_STAMP(); }   
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
  osss::osss_shared_variable<unsigned int, 1> piped_var;
  unsigned int m_count;

 public:
  BEHAVIOUR_CTOR(B_par) :
    b1("b1"),
    b2("b2"),
    b3("b3")
  {
    b1.out_port(piped_var);
    b2.in_port(piped_var);
    
    //osss_pipe(3,  b1 >> b2 >> b3 );
    // or (with the stop_condition() below)
    osss_pipe( b1 >> b2 >> b3 );
  }

  virtual void init() 
  { 
    OSSS_PRINT_TIME_STAMP(); 
    m_count = 0;
  }

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
