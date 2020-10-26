#ifndef HANDLE_INCLUDED
#define HANDLE_INCLUDED

/************	JPEG encode 	*******/
/************	handle.sc  	*******/
/************	Hanyu Yin	*******/
/************	05/11/2000	*******/

#include "const.h"
#include "global.h"

OSSS_BEHAVIOUR(ReceiveData)
{
 public:
  osss::osss_port<osss::osss_receiver_if<char> > chPixel;

  osss::osss_in<int> mduHigh;
  osss::osss_in<int> ImageWidth, ImageHeight;
  osss::osss_out<int> MDUWide;
  osss::osss_out<unsigned char*> stripe;

  BEHAVIOUR_CTOR(ReceiveData) {}

  void main()
  {
    int MDUs;
    int i, j;
    char temp;
    unsigned char *buf;
    
    // Compute number of MDUs in one row
    MDUs = (ImageWidth + 7) >> 3;    
    
    // Allocate memory for stripe data
    buf = (unsigned char *)calloc(BLOCKSIZE*MDUs, sizeof(char));

    wait(71000 * MDUs);

    // Read a stripe from testbench 
    for (i = 0; i < 8; i++) {
      for (j = 0; j < MDUs * 8; j++) {
	if ((j < ImageWidth) && (mduHigh*8+i < ImageHeight)) {
#ifdef JPEG_DEBUG	
	  msg("HandleData: Receiving pixel(%d,%d)...\n", i, j);
#endif	
	  chPixel->receive(temp);
#ifdef JPEG_DEBUG	
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

OSSS_BEHAVIOUR(HandleData)
{
 public:
  osss::osss_in<int> mduWide, mduHigh;
  osss::osss_in<unsigned char*> stripe;
  osss::osss_in<int> MDUWide;
  osss::osss_out<osss::osss_array<int, BLOCKSIZE> > HData;

 protected:
  int block;

 public:
  BEHAVIOUR_CTOR(HandleData) : block(0) {}

  void main() 
  {
    int i, j;

    //read from input port
    unsigned char* stripe_ptr = stripe;
    
    // Output message
#if defined(JPEG_VERBOSE) || defined(JPEG_DEBUG)
    std::cout << "@ " << sc_core::sc_time_stamp() 
              << " (" << sc_core::sc_delta_count() << "): ";
    msg("Processing Block %d (%d,%d)...\n", ++block, mduWide.read(), mduHigh.read());
#endif
        
    wait(71000); 

    // fetch a block and send it to DCT 
    for(i = 0; i < 8; i++) {
      for(j = 0; j < 8; j++) {
	HData[i*8+j] = stripe_ptr[i*MDUWide*8+mduWide*8+j];
#ifdef JPEG_DEBUG
	msg("%2x  ", stripe_ptr[i*MDUWide*8+mduWide*8+j]);
      }
      msg("\n");
#else
      }
#endif    
    }
    
    // Delete stripe if last MDU
    if(mduWide == (MDUWide-1)) free(stripe_ptr);
  }
};

#endif
