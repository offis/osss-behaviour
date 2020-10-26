/************	JPEG encoder Software 	*******/
/************	sw.sc		*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/

#include "const.sh"


import "jpeg";


behavior SW(i_int_receiver chHeader, i_byte_receiver chPixel,
	    i_block_sender chHData, i_block_receiver chDData,
	    i_byte_sender chData)
{
  JpegSW jpegSW(chHeader, chPixel, chHData, chDData, chData);
  
  void main(void)
  {
    pipe {
      jpegSW.main();
    }
  }
};


