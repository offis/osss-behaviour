#ifndef SW_INCLUDED
#define SW_INCLUDED

/************	JPEG encoder Software 	*******/
/************	sw.sc		*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/

#include "const.h"
#include "bus.h"
#include "jpeg.h"


OSSS_BEHAVIOUR(SW)
{
  osss::osss_port<osss::osss_receiver_if<int> >  chHeader; 
  osss::osss_port<osss::osss_receiver_if<char> > chPixel;

  sc_core::sc_out<sc_uint<32> > MWDATA; // Signals to the Interface
  sc_core::sc_in<sc_uint<32> >  MRDATA;
  sc_core::sc_out<bool>         MTSB;
  sc_core::sc_in<bool>          MTAB;
  sc_core::sc_out<bool>         MWDATAOE;
  sc_core::sc_out<sc_uint<32> > MADDR;  // Signals directly to the HW
  sc_core::sc_out<bool>         MRWB;
  sc_core::sc_in<bool>          INTC;
  
  osss::osss_port<osss::osss_sender_if<char> > chData;

 protected:
  // SW bus master driver
  CSWBus bus;
  JpegSW jpegSW;

 public:
  BEHAVIOUR_CTOR(SW) :
    bus("bus"),
    jpegSW("jpegSW")
  {
    MTSB.initialize(1);

    bus.MWDATA(MWDATA);
    bus.MRDATA(MRDATA);
    bus.MTSB(MTSB);
    bus.MTAB(MTAB);
    bus.MWDATAOE(MWDATAOE);
    bus.MADDR(MADDR);
    bus.MRWB(MRWB);
    bus.INTC(INTC);
    
    jpegSW.chHeader(chHeader);
    jpegSW.chPixel(chPixel);
    jpegSW.bus(bus);
    jpegSW.chData(chData);
    
    osss_pipe( jpegSW );
  }
};

#endif
