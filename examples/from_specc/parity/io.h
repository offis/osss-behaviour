#ifndef IO_INCLUDED
#define IO_INCLUDED

//
// This has been converted from SpecC parity example
//

/****************************************************************************
*  Title: io.sc
*  Author: Dongwan Shin
*  Date: 04/20/2001
*  Description: testbench
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define PARTITY_USE_COUNTER

// get unsigned interger from stdin
// or by counting up until 25
OSSS_BEHAVIOUR(IO)
{
  osss::osss_out<unsigned int> outport;
  osss::osss_in<unsigned int>  inport;
  osss::osss_event_out         start;
  osss::osss_event_in          done;

  BEHAVIOUR_CTOR(IO) {}

  void main() 
  {
#ifndef PARTITY_USE_COUNTER
    char buf[16];
#else
    unsigned int counter = 0;
#endif

    while (true) 
    {
#ifndef PARTITY_USE_COUNTER
      printf("Input for parity checker: ");
      fgets(buf, sizeof(buf), stdin);
      // strip trailing newline
      for (unsigned int i = 0; i < strlen(buf); i++)
      {
        if ( buf[i] == '\n' || buf[i] == '\r' )
          buf[i] = '\0';
      }
      outport = atoi(buf);
#else
      outport = counter;
#endif

      start.notify(sc_core::SC_ZERO_TIME); // start parity checker
        
      wait(done);	// check result of parity checker
#ifndef PARTITY_USE_COUNTER
      printf("parity checker output(%s) = %u\n", buf, inport.read());
#else
      printf("parity checker output(%i) = %u\n", counter, inport.read());
      if (counter == 25)
        sc_core::sc_stop();
      counter++;
#endif 
    }
  }
};

#endif
