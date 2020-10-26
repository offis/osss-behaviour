/************	JPEG encoder Design  	*******/
/************	design.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/

#include "const.sh"

import "sw";
import "hw";
import "transducer";  

  
behavior Design(i_int_receiver  chHeader, 
                i_byte_receiver chPixel, 
                i_byte_sender   chData)
{
  signal bit[31:0] MWDATA;			// coldfire write data bus
  signal bit[31:0] MRDATA;			// coldfire read data bus
  signal bit[31:0] MADDR;			// address
  signal bit[31:0] DB;				// dct data bus
  signal bit[1]    MTSB = 1; 	                // coldfire-transducer control
  signal bit[1]    MTAB = 1;
  signal bit[1]    MWDATAOE;   
  signal bit[1]    TSB = 1;  			// transducer-dct control
  signal bit[1]    TAB = 1;
  signal bit[1]    DBOE;
  signal bit[1]    MRWB;
  signal bit[1]    INTC = 1;			// coldfire-dct control
	
  SW sw(chHeader, chPixel,
	MWDATA, MRDATA, MTSB, MTAB, MWDATAOE, MADDR, MRWB, INTC,
	chData);
  HW hw(DB, DBOE, TSB, TAB, MADDR, MRWB, INTC);
  Transducer t(MWDATA, MRDATA, MTSB, MTAB, MWDATAOE, DB, DBOE, TSB, TAB, MRWB);

  void main(void) 
  {
    par
    {	
      sw.main();
      hw.main();
      t.main();
    }
  }
};
