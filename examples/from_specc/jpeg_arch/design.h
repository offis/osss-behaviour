#ifndef DESIGN_INCLUDED
#define DESIGN_INCLUDED

/************	JPEG encoder Design  	*******/
/************	design.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/

#include "const.h"

#include "sw.h"
#include "hw.h"
  
  
OSSS_BEHAVIOUR(Design)
{
  osss::osss_port<osss::osss_receiver_if<int> >  chHeader;
  osss::osss_port<osss::osss_receiver_if<char> > chPixel;
  osss::osss_port<osss::osss_sender_if<char> >   chData;
	
 protected:
  osss::osss_double_handshake_channel<block> chHData, chDData;

  SW sw;
  HW hw;

 public:
  BEHAVIOUR_CTOR(Design) :
    chHData("chHData"),
    chDData("chDData"),
    sw("sw"),
    hw("hw")
  {
    sw.chHeader(chHeader);
    sw.chPixel(chPixel);
    sw.chHData(chHData);
    sw.chDData(chDData);
    sw.chData(chData);

    hw.chHData(chHData);
    hw.chDData(chDData);
    
    osss_par( sw | hw );
  }
};

#endif
