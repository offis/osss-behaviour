#ifndef STIMULIGEN_INCLUDED
#define STIMULIGEN_INCLUDED

#include <systemc.h>

#define USE_CTHREAD

// Non-synthesisable testbench code
class StimuliGen : public sc_module
{
  public:

    sc_in<bool>  clk;
    sc_in<bool>  rst;

    sc_out<int> a, b;
     
    SC_HAS_PROCESS(StimuliGen);

    StimuliGen(sc_module_name moduleName);

  private:

    void generateStimuli();
    void generateStimuli_thread();
};

#endif
