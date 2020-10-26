#ifndef ONES_INCLUDED
#define ONES_INCLUDED

//
// This has been converted from SpecC parity example
//

/****************************************************************************
*  Title: ones.sc
*  Author: Dongwan Shin
*  Date: 04/20/2001
*  Description: Specification model for one's counter
****************************************************************************/

OSSS_BEHAVIOUR(Ones)
{
  osss::osss_in<unsigned int>  inport;
  osss::osss_out<unsigned int> outport;
  osss::osss_event_in          start;
  osss::osss_event_out         done;

  BEHAVIOUR_CTOR(Ones) {}

  void main() 
  {
    unsigned int data;
    unsigned int ocount;
    unsigned int mask;
    unsigned int temp;
    
    while (true) 
    {
      wait(start);
      data = inport;
      ocount = 0;
      mask = 1;

      while (data != 0) 
      {
        temp = data & mask;
        ocount = ocount + temp;
        data = data >> 1;
      }
      outport = ocount;
      done.notify(sc_core::SC_ZERO_TIME);
    }
  }
};

#endif
