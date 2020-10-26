/************	JPEG encode 	*******/
/************	handle.sc  	*******/
/************	Hanyu Yin	*******/
/************	05/11/2000	*******/

#include <sim.sh>

#include "const.sh"

import "global";
import "chann";


behavior ReceiveData(i_byte_receiver chPixel, in int mduHigh,
		     in  int ImageWidth, in int ImageHeight,
		     out int MDUWide, out unsigned char* stripe)
{
  void main(void)
  {
    int MDUs;
    int i, j;
    byte temp;
    unsigned char *buf;
    
    // Compute number of MDUs in one row
    MDUs = (ImageWidth + 7) >> 3;    
    
    // Allocate memory for stripe data
    buf = (unsigned char *)calloc(BLOCKSIZE*MDUs, sizeof(char));

    waitfor(71000 * MDUs);
    
    // Read a stripe from testbench 
    for (i = 0; i < 8; i++) {
      for (j = 0; j < MDUs * 8; j++) {
	if ((j < ImageWidth) && (mduHigh*8+i < ImageHeight)) {
#ifdef DEBUG	
	  msg("HandleData: Receiving pixel(%d,%d)...\n", i, j);
#endif	
	  chPixel.receive(&temp);
#ifdef DEBUG	
	  msg("HandleData: Got pixel(%d,%d) = %x\n", i, j, temp);
#endif	
	  buf[i*MDUs*8+j] = temp;
	}
	else {
	  buf[i*MDUs*8+j] = buf[i*MDUs*8+ImageWidth-1];
	}
      }
    }
    
    // Write to outputs
    MDUWide = MDUs;
    stripe  = buf;
  }
};



behavior HandleData(in int mduWide, in int mduHigh,
		    in unsigned char* stripe, in int MDUWide,
		    out int HData[BLOCKSIZE])
{
  int block = 0;
  
  void main(void) 
  {
    int i, j;
    
    // Output message
    msg("%8s: Processing Block %d (%d,%d)...\n", time2str(now()),
	++block, mduWide, mduHigh);

    waitfor(71000);
    
    // fetch a block and send it to DCT 
    for(i = 0; i < 8; i++) {
      for(j = 0; j < 8; j++) {
	HData[i*8+j] = stripe[i*MDUWide*8+mduWide*8+j];
#ifdef DEBUG
	msg("%2x  ", stripe[i*MDUWide*8+mduWide*8+j]);
      }
      msg("\n");
#else
      }
#endif    
    }
    
    // Delete stripe if last MDU
    if(mduWide == (MDUWide-1)) free(stripe);
  }
};

