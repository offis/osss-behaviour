#include "StimuliGen.h"
#include <iostream>

using namespace std;

StimuliGen::StimuliGen(sc_module_name module_name) 
                     : sc_module(module_name)
{
#if defined(USE_CTHREAD)
  SC_CTHREAD(generateStimuli, clk.pos());
  reset_signal_is(rst, true);
#else
  SC_THREAD(generateStimuli_thread);
#endif
}

void
StimuliGen::generateStimuli()
{
  //Reset
  wait();

  a.write(0);
  b.write(0);
  wait();

  b.write(1);
  wait();

  a.write(-1);
  b.write(-2);
  wait();

  wait();

  wait();

  sc_core::sc_stop();
}

void
StimuliGen::generateStimuli_thread()
{
  //Reset
  wait(sc_core::SC_ZERO_TIME);

  a.write(0);
  b.write(0);
  wait(sc_core::SC_ZERO_TIME);

  b.write(1);
  wait(sc_core::SC_ZERO_TIME);

  a.write(-1);
  b.write(-2);
  wait(sc_core::SC_ZERO_TIME);

  wait(sc_core::SC_ZERO_TIME);

  wait(sc_core::SC_ZERO_TIME);

  sc_core::sc_stop();
}
