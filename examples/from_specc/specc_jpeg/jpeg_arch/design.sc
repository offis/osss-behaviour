/************	JPEG encoder Design  	*******/
/************	design.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/

#include "const.sh"

import "sw";
import "hw";
  
  
behavior Design(i_int_receiver  chHeader, 
                i_byte_receiver chPixel, 
                i_byte_sender   chData)
{
  c_block_double_handshake chHData, chDData;
	
  SW sw(chHeader, chPixel, chHData, chDData, chData);
  HW hw(chHData, chDData);

  void main(void) 
  {
    par
    {	
      sw.main();
      hw.main();
    }
  }
};
