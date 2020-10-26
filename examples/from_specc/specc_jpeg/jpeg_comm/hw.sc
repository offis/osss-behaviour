/************	JPEG encoder Hardware 	*******/
/************	hw.sc		*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/

#include "const.sh"
#include "bus.sh"

import "bus";
import "dct";


behavior BHDataRecv(IBusSlave bus, out int HData[BLOCKSIZE])
{
  void main(void)
  {
    bus.recvBlock(&HData[0], DCT_DATAIN);
  }
};


behavior BDDataSend(in int DData[BLOCKSIZE], IBusSlave bus)
{
  void main(void)
  {
    bus.sendBlock(DData, DCT_DATAOUT);
  }
};



#ifdef PIPED_HW  

// Hardware unit with separate, concurrent FSMDs for sending and receiving
behavior HW(    signal bit[31:0] DB,
	    in  signal bit[1]    DBOE,
	    in  signal bit[1]    TSB,
	    out signal bit[1]    TAB,
	    in  signal bit[31:0] MADDR,
	    in  signal bit[1]    MRWB,
	    out signal bit[1]    INTC
	    )
{
  // HW bus slave interface
  CHWBus bus(DB, DBOE, TSB, TAB, MADDR, MRWB, INTC);
  
  piped int HData[BLOCKSIZE];
  piped int DData[BLOCKSIZE];
  
  BHDataRecv recvHData(bus, HData);
  DCT	     dct(HData, DData);
  BDDataSend sendDData(DData, bus);

  void main(void) 
  {
    pipe {
      recvHData.main();	// receive hdata output
		
      dct.main();	// original behavior

      sendDData.main();	// send ddata output
    }
  }
};

#else


behavior DctHW(IBusSlave bus)
{
  int HData[BLOCKSIZE];
  int DData[BLOCKSIZE];
  
  BHDataRecv recvHData(bus, HData);
  DCT	     dct(HData, DData);
  BDDataSend sendDData(DData, bus);

  void main(void) 
  {
    recvHData.main();	// receive hdata output
		
    dct.main();		// original behavior

    sendDData.main();	// send ddata output
  }
};


behavior HW(    signal bit[31:0] DB,
	    in  signal bit[1]    DBOE,
	    in  signal bit[1]    TSB,
	    out signal bit[1]    TAB,
	    in  signal bit[31:0] MADDR,
	    in  signal bit[1]    MRWB,
	    out signal bit[1]    INTC
	    )
{
  // HW bus slave interface
  CHWBus bus(DB, DBOE, TSB, TAB, MADDR, MRWB, INTC);
  
  DctHW dctHW(bus);
  
  void main(void)
  {
    pipe {    // endless loop
      dctHW.main();
    }
  }
};

#endif
