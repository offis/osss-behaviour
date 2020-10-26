#ifndef TB_INCLUDED
#define TB_INCLUDED

//
// This has been converted from SpecC parity example
//

/****************************************************************************
*  Title: tb.sc
*  Author: Dongwan Shin
*  Date: 04/20/2001
*  Description: testbench for partiy checker
****************************************************************************/

#include "io.h"
#include "parity.h"

OSSS_BEHAVIOUR(Main)
{
 protected:
  osss::osss_shared_variable<unsigned int> data_in, data_out;
  osss::osss_event_channel start, done;

  IO io;
  Parity parity;

 public:
  BEHAVIOUR_CTOR(Main) :
    data_in("data_in"),
    data_out("data_out"),
    io("io"),
    parity("parity")
  {
    io.outport(data_in);
    io.inport(data_out);
    io.start(start);
    io.done(done);
    
    parity.inport(data_in);
    parity.outport(data_out);
    parity.start(start);
    parity.done(done);

    osss_par( io | parity );
  }
};

#endif
