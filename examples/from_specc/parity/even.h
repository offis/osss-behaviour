#ifndef EVEN_INCLUDED
#define EVEN_INCLUDED

//
// This has been converted from SpecC parity example
//

/****************************************************************************
*  Title: even.sc
*  Author: Dongwan Shin
*  Date: 04/20/2001
*  Description: Specification model for even parity checker
****************************************************************************/

OSSS_BEHAVIOUR(Even)
{
  osss::osss_in<unsigned int>  inport;
  osss::osss_out<unsigned int> outport;
  osss::osss_event_in          start;
  osss::osss_event_out         done; 
  osss::osss_out<unsigned int> data;
  osss::osss_in<unsigned int>  ocount;
  osss::osss_event_out         istart;
  osss::osss_event_in          idone;

  BEHAVIOUR_CTOR(Even) {}

  void main() 
  {
    unsigned int mask;

    while (true) 
    {
      wait(start);
      
      data = inport;
      mask = 0x0001;
			
      istart.notify(sc_core::SC_ZERO_TIME); // start one's counter

      wait(idone); 	// wait for the result of one's counter
			
      outport = ocount & mask; // even parity checker

      done.notify(sc_core::SC_ZERO_TIME);
    }
  }
};

#endif
