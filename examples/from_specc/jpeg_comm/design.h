#ifndef DESIGN_INCLUDED
#define DESIGN_INCLUDED

/************	JPEG encoder Design  	*******/
/************	design.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/

#include "const.h"

#include "sw.h"
#include "hw.h"
#include "transducer.h"


OSSS_BEHAVIOUR(Design)
{
  osss::osss_port<osss::osss_receiver_if<int> >  chHeader;
  osss::osss_port<osss::osss_receiver_if<char> > chPixel;
  osss::osss_port<osss::osss_sender_if<char> >   chData;

  //protected:
 public: // for tracing
  sc_core::sc_signal<sc_uint<32> >          MWDATA; // coldfire write data bus
  sc_core::sc_signal<sc_uint<32> >          MRDATA; // coldfire read data bus
  sc_core::sc_signal<sc_uint<32> >          MADDR;  // address
  osss::osss_unchecked_signal<sc_uint<32> > DB;	    // dct data bus
  sc_core::sc_signal<bool>                  MTSB;   // coldfire-transducer control
  sc_core::sc_signal<bool>                  MTAB;
  sc_core::sc_signal<bool>                  MWDATAOE;
  sc_core::sc_signal<bool>                  TSB;    // transducer-dct control
  sc_core::sc_signal<bool>                  TAB;
  sc_core::sc_signal<bool>                  DBOE;
  sc_core::sc_signal<bool>                  MRWB;
  sc_core::sc_signal<bool>                  INTC;   // coldfire-dct control
  
 protected:
  SW sw;
  HW hw;
  Transducer transducer;
  
 public:
  BEHAVIOUR_CTOR(Design) :
    MWDATA("MWDATA"),
    MRDATA("MRDATA"),
    MADDR("MADDR"),
    DB("DB"),
    MTSB("MTSB"),
    MTAB("MTAB"),
    MWDATAOE("MWDATAOE"),
    TSB("TSB"),
    TAB("TAB"),
    DBOE("DBOE"),
    MRWB("MRWB"),
    INTC("INTC"),
    sw("sw"),
    hw("hw"),
    transducer("transducer")
  {
    sw.chHeader(chHeader);
    sw.chPixel(chPixel);
    sw.MWDATA(MWDATA);
    sw.MRDATA(MRDATA);
    sw.MTSB(MTSB);
    sw.MTAB(MTAB);
    sw.MWDATAOE(MWDATAOE);
    sw.MADDR(MADDR);
    sw.MRWB(MRWB);
    sw.INTC(INTC);
    sw.chData(chData);

    hw.DB(DB);
    hw.DBOE(DBOE);
    hw.TSB(TSB);
    hw.TAB(TAB);
    hw.MADDR(MADDR);
    hw.MRWB(MRWB);
    hw.INTC(INTC);

    transducer.MWDATA(MWDATA);
    transducer.MRDATA(MRDATA);
    transducer.MTSB(MTSB);
    transducer.MTAB(MTAB);
    transducer.MWDATAOE(MWDATAOE);
    transducer.DB(DB);
    transducer.DBOE(DBOE);
    transducer.TSB(TSB);
    transducer.TAB(TAB);
    transducer.MRWB(MRWB);

    osss_par( sw | transducer | hw );
  }

};

#endif
