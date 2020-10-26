#include "osss_behaviour.h"

#define USE_CTHREAD

OSSS_PIPELINE_STAGE(Stage1)
{
  PIPELINE_STAGE_CTOR(Stage1) {}

  virtual void main()
  {
    OSSS_PRINT_TIME_STAMP();
    
    wait();
  }
    
};

OSSS_PIPELINE_STAGE(Stage2)
{
  PIPELINE_STAGE_CTOR(Stage2) {}

  virtual void main()
  {
    OSSS_PRINT_TIME_STAMP();
   
    wait(2);
  }
    
};

OSSS_PIPELINE_STAGE(Stage3)
{
  PIPELINE_STAGE_CTOR(Stage3) {}

  virtual void main()
  {
    OSSS_PRINT_TIME_STAMP();
  
    wait(3);
  }
    
};

//--------------------------------------------------------------------

OSSS_BEHAVIOUR(Pipeline)
{
 protected:
  Stage1 b1; 
  Stage2 b2; 
  Stage3 b3;

 public:
  BEHAVIOUR_CTOR(Pipeline) :
    b1("b1"),
    b2("b2"),
    b3("b3")
  {
    // pipeline runs until stop condition is reached,
    // when no stop condition is specified ir runs only once
    osss_pipe(b1 >> b2 >> b3);

    // pipeline runs 10 times
    //osss_pipe(10, b1 >> b2 >> b3);
  }
};

//--------------------------------------------------------------------

SC_MODULE(Top)
{
  sc_in<bool> clk;
  sc_in<bool> rst;

  Pipeline p;

  SC_CTOR(Top) :
   clk("clk"),
   rst("rst"),
   p("p")
 {
#if defined(USE_CTHREAD)
   SC_CTHREAD(main, clk.pos());
   reset_signal_is(rst, true);

   p.set_sensitivity(clk.pos());
#else
   SC_THREAD(main);
#endif

   // register clock and reset globally
   osss::osss_global_port_registry::register_clock_port(clk);
   osss::osss_global_port_registry::register_reset_port(rst); 
 }

 virtual void end_of_elaboration()
 {
   p.print_graph("graph.dot", true);
 }

 void main() 
 {
#if defined(USE_CTHREAD)
   // reset
   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "--> Reset" );
  
   wait();
#endif

   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "before pipeline behaviour" );
   
   // call pipeline behaviour
   p.main();

   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "after pipeline behaviour" );
  
 }
};

