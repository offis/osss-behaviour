#ifndef SW_INCLUDED
#define SW_INCLUDED

/************	JPEG encoder Software 	*******/
/************	sw.sc		*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/

#include "const.h"
#include "jpeg.h"

OSSS_BEHAVIOUR(SW)
{
  osss::osss_port<osss::osss_receiver_if<int> >   chHeader;
  osss::osss_port<osss::osss_receiver_if<char> >  chPixel;
  osss::osss_port<osss::osss_sender_if<block> >   chHData;
  osss::osss_port<osss::osss_receiver_if<block> > chDData;
  osss::osss_port<osss::osss_sender_if<char> >    chData;

 protected:
  JpegSW jpegSW;
  
 public:
  BEHAVIOUR_CTOR(SW) :
    jpegSW("jpegSW")
  {
    jpegSW.chHeader(chHeader);
    jpegSW.chPixel(chPixel);
    jpegSW.chHData(chHData);
    jpegSW.chDData(chDData);
    jpegSW.chData(chData);

    osss_pipe( jpegSW );
  }
};

#endif
