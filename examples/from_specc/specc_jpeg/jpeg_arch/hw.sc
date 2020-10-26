/************	JPEG encoder Hardware 	*******/
/************	hw.sc		*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/

#include "const.sh"

import "chann";
import "dct";


behavior BHDataRecv(i_block_receiver chHData, out int HData[BLOCKSIZE])
{
  void main(void)
  {
    chHData.receive(&HData);
  }
};


behavior BDDataSend(in int DData[BLOCKSIZE], i_block_sender chDData)
{
  void main(void)
  {
    chDData.send(DData);
  }
};



#ifdef PIPED_HW  

// Hardware unit with separate, concurrent FSMDs for sending and receiving
behavior HW(i_block_receiver chHData, i_block_sender chDData)
{
  piped int HData[BLOCKSIZE];
  piped int DData[BLOCKSIZE];
  
  BHDataRecv recvHData(chHData, HData);
  DCT	     dct(HData, DData);
  BDDataSend sendDData(DData, chDData);

  void main(void) 
  {
    pipe {
      recvHData.main();	// receive hdata output
		
      dct.main();	// original behavior

      sendDData.main();	// send ddata output
    }
  }
};

#else


behavior DctHW(i_block_receiver chHData, i_block_sender chDData)
{
  int HData[BLOCKSIZE];
  int DData[BLOCKSIZE];
  
  BHDataRecv recvHData(chHData, HData);
  DCT	     dct(HData, DData);
  BDDataSend sendDData(DData, chDData);

  void main(void) 
  {
    recvHData.main();	// receive hdata output
		
    dct.main();		// original behavior

    sendDData.main();	// send ddata output
  }
};


behavior HW(i_block_receiver chHData, i_block_sender chDData)
{
  DctHW dctHW(chHData, chDData);
  
  void main(void)
  {
    pipe {    // endless loop
      dctHW.main();
    }
  }
};

#endif
