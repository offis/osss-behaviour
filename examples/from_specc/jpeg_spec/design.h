#ifndef DESIGN_INCLUDED
#define DESIGN_INCLUDED

/************	JPEG encoder design 	*******/
/************	design.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/

#include "const.h"
#include "jpeg.h"

OSSS_BEHAVIOUR(Design)
{
 public:
  osss::osss_port<osss::osss_receiver_if<int> >  chHeader;
  osss::osss_port<osss::osss_receiver_if<char> > chPixel;
  osss::osss_port<osss::osss_sender_if<char> >   chData;

 protected:
  Jpeg jpeg;

 public:

  BEHAVIOUR_CTOR(Design) :
    jpeg("jpeg")
  {
    jpeg.chHeader(chHeader);
    jpeg.chPixel(chPixel);
    jpeg.chData(chData);
    
    osss_seq( jpeg );
  }

};

#endif
