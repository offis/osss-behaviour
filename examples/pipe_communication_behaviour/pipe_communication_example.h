#include "osss_behaviour.h"

#define USE_CTHREAD

OSSS_PIPELINE_STAGE(Stage1)
{
  osss::osss_out<int> v1_out;
  osss::osss_out<int> v2_out;

  PIPELINE_STAGE_CTOR(Stage1) :
    m_data1(0),
    m_data2(0)
  {}

  virtual void main()
  {
    OSSS_PRINT_TIME_STAMP();
  
    // write to v1_out
    v1_out = m_data1;
    m_data1++;

    // write to v2_out
    v2_out = m_data2;
    m_data2++;

    wait();
  }

 protected:
  int m_data1;
  int m_data2;
    
};

OSSS_PIPELINE_STAGE(Stage2)
{
  osss::osss_in<int>  v2_in;
  osss::osss_out<int> v3_out;

  PIPELINE_STAGE_CTOR(Stage2) {}

  virtual void main()
  {
    OSSS_PRINT_TIME_STAMP();
  
    //read from v2_in
    int tmp_v2 = v2_in;
    OSSS_PRINT_TIME_STAMP_WITH_TEXT( "v2_in = " << tmp_v2 );
   
    //write to v3_out
    v3_out = tmp_v2;

    wait();
  }
    
};

OSSS_PIPELINE_STAGE(Stage3)
{
  osss::osss_in<int> v3_in;
  osss::osss_in<int> v1_in;

  PIPELINE_STAGE_CTOR(Stage3) {}

  virtual void main()
  {
    OSSS_PRINT_TIME_STAMP();
   
    //read from v3_in
    int tmp_v3 = v3_in;
    OSSS_PRINT_TIME_STAMP_WITH_TEXT( "v3_in = " << tmp_v3 );
   
    //read from v1_in
    int tmp_v1 = v1_in;
    OSSS_PRINT_TIME_STAMP_WITH_TEXT( "v1_in = " << tmp_v1 );
   
    wait();
  }
    
};

//------------------------------------------------------------------------------

OSSS_BEHAVIOUR(Pipeline)
{
  osss::osss_shared_variable<int, 2> v1;
  osss::osss_shared_variable<int, 1> v2;
  osss::osss_shared_variable<int, 1> v3;

  Stage1 b1; 
  Stage2 b2; 
  Stage3 b3;

  BEHAVIOUR_CTOR(Pipeline) :
    v1("v1"),
    v2("v2"),
    v3("v3"),
    b1("b1"),
    b2("b2"),
    b3("b3")
  {
    b1.v1_out(v1);
    b1.v2_out(v2);
   
    b2.v2_in(v2);
    b2.v3_out(v3);
   
    b3.v3_in(v3);
    b3.v1_in(v1);

    // pipeline runs until stop condition is reached,
    // when no stop condition is specified ir runs only once
    //osss_pipe(b1 >> b2 >> b3);

    // pipeline runs 10 times
    osss_pipe(10, b1 >> b2 >> b3); 
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
