#include "osss_behaviour.h"

#define USE_CTHREAD

//------------------------------------------------------------------------------
// definition of leaf behaviours
//------------------------------------------------------------------------------

OSSS_BEHAVIOUR(B1_1)
{

  BEHAVIOUR_CTOR(B1_1) {}

  virtual void main() { OSSS_PRINT_TIME_STAMP(); }
    
};

//--------------------------------------------------------------------

OSSS_INITIAL_STATE(C1)
{

  INITIAL_STATE_CTOR(C1) {}

  virtual void enter_action() { OSSS_PRINT_TIME_STAMP(); }

  virtual void main() { OSSS_PRINT_TIME_STAMP(); }

  virtual void exit_action() { OSSS_PRINT_TIME_STAMP(); }
    
};

OSSS_STATE(C3)
{

  STATE_CTOR(C3) {}

  virtual void enter_action() { OSSS_PRINT_TIME_STAMP(); }
 
  virtual void main() { OSSS_PRINT_TIME_STAMP(); }
 
  virtual void exit_action() { OSSS_PRINT_TIME_STAMP(); }
    
};

OSSS_END_STATE(C4)
{

  END_STATE_CTOR(C4) {}

  virtual void enter_action() { OSSS_PRINT_TIME_STAMP(); }
 
  virtual void main() { OSSS_PRINT_TIME_STAMP(); }
 
  virtual void exit_action() { OSSS_PRINT_TIME_STAMP(); }
    
};

//--------------------------------------------------------------------

OSSS_BEHAVIOUR(E1)
{

  BEHAVIOUR_CTOR(E1) {}

  virtual void main() { OSSS_PRINT_TIME_STAMP(); }
    
};

OSSS_BEHAVIOUR(E2)
{

  BEHAVIOUR_CTOR(E2) {}

  virtual void main() { OSSS_PRINT_TIME_STAMP(); }
    
};

//--------------------------------------------------------------------

OSSS_BEHAVIOUR(D2)
{

  BEHAVIOUR_CTOR(D2) {}

  virtual void main() { OSSS_PRINT_TIME_STAMP(); wait(); }
    
};

//--------------------------------------------------------------------

OSSS_PIPELINE_STAGE(B1_2)
{

  PIPELINE_STAGE_CTOR(B1_2) {}

  virtual void main() { OSSS_PRINT_TIME_STAMP(); wait(); }
    
};

OSSS_PIPELINE_STAGE(B2_2)
{

  PIPELINE_STAGE_CTOR(B2_2) {}

  virtual void main() { OSSS_PRINT_TIME_STAMP(); wait(); }
    
};

OSSS_PIPELINE_STAGE(B3_2)
{

  PIPELINE_STAGE_CTOR(B3_2) {}

  virtual void main() { OSSS_PRINT_TIME_STAMP(); wait(); }
    
};

//------------------------------------------------------------------------------
// definition of hierarchical behaviours
//------------------------------------------------------------------------------

//--------------------------------
// D1 (OSSS Behaviour)
//--------------------------------

OSSS_BEHAVIOUR(D1)
{
  E1 e1; 
  E2 e2; 

  D1(sc_core::sc_module_name name) :
    osss::osss_behaviour(name),
    e1("e1"),
    e2("e2")
  {
    osss_seq(e1, e2);
  }
};

//--------------------------------
// C2 (OSSS Behaviour)
//--------------------------------

OSSS_STATE(C2)
{
  D1 d1; 
  D2 d2; 

  C2(sc_core::sc_module_name name) :
    osss::osss_state(name),
    d1("d1"),
    d2("d2")
  {
    osss_par(d1 | d2);
  }

  virtual void enter_action() { OSSS_PRINT_TIME_STAMP(); }
  virtual void exit_action() { OSSS_PRINT_TIME_STAMP(); }
};

//-----------------------------------------------------
// A1 (OSSS FSM Behaviour)
//
// TODO: put state machine definition in constructor
//-----------------------------------------------------

OSSS_BEHAVIOUR(B2)
{
  sc_in<int> a;

  C1 c1;
  C2 c2;
  C3 c3;
  C4 c4;

  B2(sc_core::sc_module_name name) :
    osss::osss_behaviour(name),
    a("a"),
    c1("c1"),
    c2("c2"),
    c3("c3"),
    c4("c4")
  {
    osss_fsm( (c1 >> c2, osss_cond(B2::cond1)) &&
              (c2 >> c3, osss_cond(B2::cond2)) &&
              (c3 >> c4, osss_cond(B2::cond3)) &&
              (c3 >> c1, osss_cond(B2::cond4)) );
  }

  bool cond1() const { return (a > 0); }
  bool cond2() const { return (a < 0); }
  bool cond3() const { return (a != 0); }
  bool cond4() const { return (a == 0); }

  /*
  virtual void main() 
  {
    //OSSS_PRINT_TIME_STAMP();

    OSSS_FSM
    {
#if defined(USE_CTHREAD)
      wait(clock.posedge_event());
      //wait();
#else
      wait(sc_core::SC_ZERO_TIME);
#endif

      fsm_case(c1)
      { 
        if (a > 0)  { enter_state(c2); } 
      }
      fsm_case(c2)
      { 
        if (a < 0) { enter_state(c3); } 
      }
      fsm_case(c3) 
      { 
        if (a != 0)  { enter_state(c4); }
        if (a == 0) { enter_state(c1); }
      }
      fsm_case(c4) { enter_end_state(); }
    }
  }
  */

};

//--------------------------------
// A1 (OSSS Sequential Behaviour)
//--------------------------------

OSSS_BEHAVIOUR(A1)
{
  sc_in<int> a;

  B1_1 b1_1;
  B2   b2;
  
  BEHAVIOUR_CTOR(A1) : 
    a("a"),
    b1_1("b1_1"),
    b2("b2")
  {
    //port to port binding
    b2.a(a);

    osss_seq(b1_1, b2);
  }
};

//--------------------------------
// A2 (OSSS Pipeline Behaviour)
//--------------------------------

OSSS_BEHAVIOUR(A2)
{
  B1_2 b1_2;
  B2_2 b2_2;
  B3_2 b3_2;
  
  BEHAVIOUR_CTOR(A2) : 
    b1_2("b1_2"),
    b2_2("b2_2"),
    b3_2("b3_2")
  {
    osss_pipe(b1_2 >> b2_2 >> b3_2);
  }
};

//--------------------------------
// Base (OSSS Parallel Behaviour)
//--------------------------------

OSSS_BEHAVIOUR(Base)
{
  sc_in<int> a;
  
  A1 a1; 
  A2 a2; 

  BEHAVIOUR_CTOR(Base) :
    a("a"),
    a1("a1"),
    a2("a2")
  {
    a1.a(a);
    
    osss_par(a1 | a2);
  }
};

//--------------------------------
// Top (SC Module)
//--------------------------------

SC_MODULE(Top)
{
  sc_in<bool> clk;
  sc_in<bool> rst;

  sc_in<int> a;

  Base b;

  SC_CTOR(Top) :
    clk("clk"),
    rst("rst"),
    b("b")
  {
#if defined(USE_CTHREAD)
    SC_CTHREAD(main, clk.pos());
    reset_signal_is(rst, true);

    b.set_sensitivity(clk.pos());

    SC_CTHREAD(showStimuli, clk.pos());
    reset_signal_is(rst, true);
#else
    SC_THREAD(main);
    SC_THREAD(showStimuli);
#endif

    // register clock and reset globally
    osss::osss_global_port_registry::register_clock_port(clk);
    osss::osss_global_port_registry::register_reset_port(rst);

    // port to port binding
    b.a(a);
  }

  virtual void end_of_elaboration()
  {
    b.print_graph("graph.dot", true);
  } 

  void main() 
  {
#if defined(USE_CTHREAD)
    // reset
    OSSS_PRINT_TIME_STAMP_WITH_TEXT( "--> Reset");
   
    wait();
#endif

    OSSS_PRINT_TIME_STAMP_WITH_TEXT( "before OSSS_PAR(...)" );
   
    b.main();

    OSSS_PRINT_TIME_STAMP_WITH_TEXT( "after OSSS_PAR(...)" );
  }

  void showStimuli()
  {
#if defined(USE_CTHREAD)
    //Reset
    wait();
#endif

    while(true)
    {
      OSSS_PRINT_TIME_STAMP_WITH_TEXT(  "a: " << a.read() );
#if defined(USE_CTHREAD)
      wait();
#else
      wait(sc_core::SC_ZERO_TIME);
#endif
    }
  }

};

