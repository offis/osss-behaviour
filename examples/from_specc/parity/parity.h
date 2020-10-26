#ifndef PARITY_INCLUDED
#define PARITY_INCLUDED

//
// This has been converted from SpecC parity example
//

/****************************************************************************
*  Title: parity.sc
*  Author: Dongwan Shin
*  Date: 04/20/2001
*  Description: Specification model for even parity checker
****************************************************************************/

#include "ones.h"
#include "even.h"

OSSS_BEHAVIOUR(Parity)
{
  osss::osss_in<unsigned int>  inport;
  osss::osss_out<unsigned int> outport;
  osss::osss_event_in          start;
  osss::osss_event_out         done;

 protected:
  osss::osss_shared_variable<unsigned int> data, ocount;
  osss::osss_event_channel istart, idone;

  Even even;
  Ones ones;

 public:
  BEHAVIOUR_CTOR(Parity) :
    data("data"),
    ocount("ocount"),
    even("even"),
    ones("ones")
  {
    even.inport(inport);
    even.outport(outport);
    even.start(start);
    even.done(done);
    even.data(data);
    even.ocount(ocount);
    even.istart(istart);
    even.idone(idone);

    ones.inport(data);
    ones.outport(ocount);
    ones.start(istart);
    ones.done(idone);

    osss_par( even | ones );
  }
};

#endif
