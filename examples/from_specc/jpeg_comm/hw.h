#ifndef HW_INCLUDED
#define HW_INCLUDED

/************	JPEG encoder Hardware 	*******/
/************	hw.sc		*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/

#include "const.h"
#include "bus.h"

#include "bus.h"
#include "dct.h"


OSSS_BEHAVIOUR(BHDataRecv)
{
  osss::osss_port<IBusSlave> bus;
  osss::osss_out<osss::osss_array<int, BLOCKSIZE> > HData;

  BEHAVIOUR_CTOR(BHDataRecv) {}

  void main()
  {
    osss::osss_array<int, BLOCKSIZE> tmp_data;
    bus->recvBlock(tmp_data, DCT_DATAIN);
    HData = tmp_data;
  }
};


OSSS_BEHAVIOUR(BDDataSend)
{
  osss::osss_in<osss::osss_array<int, BLOCKSIZE> > DData;
  osss::osss_port<IBusSlave> bus;

  BEHAVIOUR_CTOR(BDDataSend) {}

  void main()
  {
    bus->sendBlock(DData, DCT_DATAOUT);
  }
};


#ifdef PIPED_HW  

// Hardware unit with separate, concurrent FSMDs for sending and receiving
OSSS_BEHAVIOUR(HW)
{
  sc_core::sc_inout<sc_uint<32> > DB;
  sc_core::sc_in<bool>            DBOE;
  sc_core::sc_in<bool>            TSB;
  sc_core::sc_out<bool>           TAB;
  sc_core::sc_in<sc_uint<32> >    MADDR;
  sc_core::sc_in<bool>            MRWB;
  sc_core::sc_out<bool>           INTC;
	    
 protected:
  // HW bus slave interface
  CHWBus bus;
  
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE>, 1> HData;
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE>, 1> DData;
  
  BHDataRecv recvHData;
  DCT	     dct;
  BDDataSend sendDData;

 public:
  BEHAVIOUR_CTOR(HW) :
    CHWBus("CHWBus"),
    HData("HData"),
    DData("DData"),
    recvHData("recvHData"),
    dct("dct"),
    sendDData("sendDData")
  {
    CHWBus bus(DB, DBOE, TSB, TAB, MADDR, MRWB, INTC);

    recvHData.bus(bus);
    recvHData.HData(HData);

    dct.HData(HData);
    dct.DData(DData);

    sendDData.DData(DData);
    sendDData.bus(bus);
    
    osss_pipe(
      recvHData >> // receive hdata output
      dct >>       // original behavior
      sendDData    // send ddata output
    ); 
  }
};

#else


OSSS_PIPELINE_STAGE(DctHW)
{
  osss::osss_port<IBusSlave> bus;

 protected:
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE> > HData;
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE> > DData;
   
  BHDataRecv recvHData;
  DCT	     dct;
  BDDataSend sendDData;

 public:
  PIPELINE_STAGE_CTOR(DctHW) :
    HData("HData"),
    DData("DData"),
    recvHData("recvHData"),
    dct("dct"),
    sendDData("sendDData")
  {
    recvHData.bus(bus);
    recvHData.HData(HData);

    dct.HData(HData);
    dct.DData(DData);

    sendDData.DData(DData);
    sendDData.bus(bus);
    
    osss_seq(
      recvHData, // receive hdata output
      dct,       // original behavior
      sendDData  // send ddata output
    );
  }

  // TODO !!!
  void main()
  {
    recvHData.main(); // receive hdata output
    dct.main();       // original behavior
    sendDData.main();  // send ddata output
  }

};


OSSS_BEHAVIOUR(HW)
{
  sc_core::sc_inout<sc_uint<32> > DB;
  sc_core::sc_in<bool>            DBOE;
  sc_core::sc_in<bool>            TSB;
  sc_core::sc_out<bool>           TAB;
  sc_core::sc_in<sc_uint<32> >    MADDR;
  sc_core::sc_in<bool>            MRWB;
  sc_core::sc_out<bool>           INTC;
	    
 protected:
  // HW bus slave interface
  CHWBus bus;
  DctHW dctHW;
  
 public:
  BEHAVIOUR_CTOR(HW) :
    bus("bus"),
    dctHW("dctHW")
  {
    TAB.initialize(1);
    INTC.initialize(1);

    bus.DB(DB);
    bus.DBOE(DBOE);
    bus.TSB(TSB);
    bus.TAB(TAB);
    bus.MADDR(MADDR);
    bus.MRWB(MRWB);
    bus.INTC(INTC);
  
    dctHW.bus(bus);

    osss_pipe( dctHW ); // endless loop
  }
};

#endif

#endif
