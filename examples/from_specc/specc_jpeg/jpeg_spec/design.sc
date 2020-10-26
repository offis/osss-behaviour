/************	JPEG encoder design 	*******/
/************	design.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/

#include "const.sh"

import "jpeg";


behavior Design(i_int_receiver  chHeader, 
                i_byte_receiver chPixel, 
                i_byte_sender   chData)
{
  Jpeg jpeg(chHeader, chPixel, chData);
  
  void main(void)
  {
    pipe {
      jpeg.main();
    }
  }
};


