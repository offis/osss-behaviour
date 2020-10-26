#include "osss_behaviour.h"

#define USE_CTHREAD

OSSS_INITIAL_STATE(State1)
{
  INITIAL_STATE_CTOR(State1) {}

  virtual void enter_action() { OSSS_PRINT_TIME_STAMP(); }
  virtual void main() { OSSS_PRINT_TIME_STAMP(); }
  virtual void exit_action() { OSSS_PRINT_TIME_STAMP(); }    
};

OSSS_STATE(State2)
{
public:

  STATE_CTOR(State2) {}

  virtual void enter_action() { OSSS_PRINT_TIME_STAMP(); }
  virtual void main() { OSSS_PRINT_TIME_STAMP(); }
  virtual void exit_action() { OSSS_PRINT_TIME_STAMP(); }   
};

class State2_prime : public State2
{
public:

  State2_prime(sc_core::sc_module_name name) : State2(name) {}

  virtual void main() { OSSS_PRINT_TIME_STAMP(); }
};

OSSS_STATE(State3)
{
  STATE_CTOR(State3) {}

  virtual void enter_action() { OSSS_PRINT_TIME_STAMP(); }
  virtual void main() { OSSS_PRINT_TIME_STAMP(); }
  virtual void exit_action() { OSSS_PRINT_TIME_STAMP(); }    
};

OSSS_END_STATE(State4)
{
  END_STATE_CTOR(State4) {}

  virtual void enter_action() { OSSS_PRINT_TIME_STAMP(); }
  virtual void main() { OSSS_PRINT_TIME_STAMP(); }
  virtual void exit_action() { OSSS_PRINT_TIME_STAMP(); }    
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

OSSS_BEHAVIOUR(B_FSM)
{
  sc_in<int> a, b;

  State1* st1; 
  State2* st2; 
  State3* st3;
  State4* st4;

  BEHAVIOUR_CTOR(B_FSM) :
    a("a"),
    b("b")
  {
    OSSS_DECLARE_STATE(State1, st1);
    OSSS_DECLARE_STATE(State2, st2);
    OSSS_DECLARE_STATE(State3, st3);
    OSSS_DECLARE_STATE(State4, st4);

    osss_fsm( (*st1 >> *st4, osss_cond(B_FSM::cond1)) &&
              (*st1 >> *st1, osss_cond(B_FSM::cond2)) &&
              (*st1 >> *st2, osss_cond(B_FSM::cond3)) &&
              (*st2 >> *st1, osss_cond(B_FSM::cond4)) &&
              (*st2 >> *st3, osss_cond(B_FSM::cond5)) &&
              (*st3 >> *st1, osss_cond(B_FSM::cond6)) &&
              (*st3 >> *st4, osss_cond(B_FSM::cond7)) );
  }

  bool cond1() const { return (b < 0); }
  bool cond2() const { return (b == 0); }
  bool cond3() const { return (b > 0); }
  bool cond4() const { return (a > 0); }
  bool cond5() const { return (a <= 0); }
  bool cond6() const { return (a > b); }
  bool cond7() const { return (a <= b); }

/*
  virtual void main() 
  {
    OSSS_FSM
    {
      wait();

      fsm_case(st1)
      { if (b < 0)  { enter_state(st4); }
        if (b == 0) { enter_state(st1); }
        if (b > 0)  { enter_state(st2); }
      }
      fsm_case(st2)
      { if (a > 0)  { enter_state(st1); }
        if (a <= 0) { enter_state(st3); }
      }
      fsm_case(st3) 
      { if (a > b)  { enter_state(st1); }
        if (a <= b) { enter_state(st4); }
      }
      fsm_case(st4) { enter_end_state(); }
    }
  }
*/

};

//------------------------------------------------------------------------------

class C_FSM : public B_FSM
{
 public:

  C_FSM(sc_core::sc_module_name name) : B_FSM(name) {}

 virtual void main() 
 {
   OSSS_FSM
   {
     wait();

     fsm_case(st1)
     { if (b < 0)  { enter_state(st4); }
       if (b == 0) { enter_state(st1); }
       if (b > 0)  { enter_state(st2); }
     }
     fsm_case(st2)
     { if (a > 0)  { enter_state(st1); }
       if (a <= 0) { enter_state(st4); }
     }
     fsm_case(st4) { enter_end_state(); }
   }
 }

};

//------------------------------------------------------------------------------

class D_FSM : public C_FSM
{
 public:

  State2_prime* st2_prime;

  D_FSM(sc_core::sc_module_name name) : 
    C_FSM(name)
  {
    OSSS_OVERWRITE_STATE(State2_prime, st2);
  }

};

//------------------------------------------------------------------------------

SC_MODULE(Top)
{
 sc_in<bool> clk;
 sc_in<bool> rst;

 sc_in<int> a, b;

 B_FSM m_fsm;
// C_FSM m_fsm;
// D_FSM m_fsm;

 SC_CTOR(Top) :
   clk("clk"),
   rst("rst"),
   a("a"),
   b("b"),
   m_fsm("m_fsm")
 {
#if defined(USE_CTHREAD)
   SC_CTHREAD(main, clk.pos());
   reset_signal_is(rst, true);

   m_fsm.set_sensitivity(clk.pos());

   SC_CTHREAD(showStimuli, clk.pos());
   reset_signal_is(rst, true);
#else
   SC_THREAD(main);
   SC_THREAD(showStimuli);
#endif

   // register clock and reset globally
   osss::osss_global_port_registry::register_clock_port(clk);
   osss::osss_global_port_registry::register_reset_port(rst);
   
   m_fsm.a(a);
   m_fsm.b(b);
 }

 virtual void end_of_elaboration()
 {
   m_fsm.print_graph("graph.dot", true);
 } 

 virtual void main() 
 {
#if defined(USE_CTHREAD)
   // reset
   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "--> Reset" );
   
   wait();
#endif

   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "before OSSS_FSM" );
   
   m_fsm.main();

   OSSS_PRINT_TIME_STAMP_WITH_TEXT( "after OSSS_FSM" );
 }

 void showStimuli()
 {
#if defined(USE_CTHREAD)
   //Reset
   wait();
#endif

   while(true)
   {
     OSSS_PRINT_TIME_STAMP_WITH_TEXT( "a: " << a.read() << " b: " << b.read() );
#if defined(USE_CTHREAD)
     wait();
#else
     wait(sc_core::SC_ZERO_TIME);
#endif
   }
 }

};
