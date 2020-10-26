/************	JPEG encoder Software 	*******/
/************	sw.sc		*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/

#include "const.sh"
#include "bus.sh"


import "bus";
import "jpeg";


behavior SW(i_int_receiver chHeader, i_byte_receiver chPixel,
	    out signal bit[31:0] MWDATA,	// Signals to the Interface
	    in  signal bit[31:0] MRDATA,
	    out signal bit[1]    MTSB,
	    in  signal bit[1]    MTAB,
	    out signal bit[1]    MWDATAOE,
	    out signal bit[31:0] MADDR,	// Signals directly to the HW
	    out signal bit[1]    MRWB,
	    in  signal bit[1]    INTC,
	    i_byte_sender chData)
{
  // SW bus master driver
  CSWBus bus(MWDATA, MRDATA, MTSB, MTAB, MWDATAOE, MADDR, MRWB, INTC);
  
  JpegSW jpegSW(chHeader, chPixel, bus, chData);
  
  void main(void)
  {
    pipe {
      jpegSW.main();
    }
  }
};

