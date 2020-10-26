#ifndef TRANSDUCER_INCLUDED
#define TRANSDUCER_INCLUDED

/************	JPEG encoder Transducer 	*******/
/************	transducer.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/


#include "global.h"
#include "bus.h"

OSSS_BEHAVIOUR(Transducer)
{
  sc_core::sc_in<sc_uint<32> >    MWDATA;
  sc_core::sc_out<sc_uint<32> >   MRDATA;
  sc_core::sc_in<bool>            MTSB;
  sc_core::sc_out<bool>           MTAB;
  sc_core::sc_in<bool>            MWDATAOE;
  sc_core::sc_inout<sc_uint<32> > DB;
  sc_core::sc_out<bool>           DBOE;
  sc_core::sc_out<bool>           TSB;
  sc_core::sc_in<bool>            TAB;
  sc_core::sc_in<bool>            MRWB;
		    
  BEHAVIOUR_CTOR(Transducer) 
  {
    MTAB.initialize(1);
    TSB.initialize(1);
  }

  void main() 
  {
    sc_uint<32> data;

    while(true)
    {
      // T0
      MTAB = 1;
      TSB = 1;

      wait(MTSB.negedge_event());
	
      if(MRWB)
      {
        // TR1
        MTAB = 1;
        TSB = 0;
        wait(4);

        // TR2
        MTAB = 1;
        TSB = 1;
        wait(4);

        // TR3
        wait(TAB.negedge_event());

        MTAB = 1;
        data = DB;
        wait(4);

        // TR4
        MRDATA = data;
        MTAB = 0;
        wait(4);

        // TR5
        MRDATA = data;
        MTAB = 0;
        wait(4);
      }
      else
      {
        // TW1
        MTAB = 1;
        wait(4);
	
        // TW2
        while(!MWDATAOE)
        {	
          MTAB = 1;
          wait(4);
        }
		
        // TW3
        TSB = 0;
        MTAB = 1;
        DBOE = 0;
        data = MWDATA;
        wait(4);
		
        // TW4
        TSB = 1;	
        MTAB = 1;
        DB = data;
        DBOE = 1;
        wait(4);
		
        // WS5
        MTAB = 0;
        wait(4);
	  
        // WS6
        MTAB = 0;
        wait(4);
      }
    }
  }
};

#endif
