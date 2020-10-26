/************	JPEG encode 	*******/
/************	encode.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/06/2001	*******/

#include "const.sh"

import "handle";
import "dct";
import "quant";
import "huff";


behavior BHDataSend(in int HData[BLOCKSIZE], i_block_sender chHData)
{
  void main(void)
  {
    chHData.send(HData);
  }
};


behavior BDDataRecv(i_block_receiver chDData, out int DData[BLOCKSIZE])
{
  void main(void)
  {
    chDData.receive(&DData);
  }
};



behavior JpegEncodeStripe(in int mduHigh, 
			  in unsigned char* stripe, in int MDUWide,
			  in EHuff DCEhuff, in EHuff ACEhuff,
			  i_block_sender chHData, i_block_receiver chDData,
			  ISendBits chData)
{
  int mduWide;
  
  int hdata[BLOCKSIZE];
  int ddata[BLOCKSIZE];
  int qdata[BLOCKSIZE];
  
  HandleData	handleData(mduWide, mduHigh, stripe, MDUWide, hdata);
  BHDataSend    sendHData(hdata, chHData);
  BDDataRecv    recvDData(chDData, ddata);
  Quantization	quantization(ddata, qdata);
  HuffmanEncode	huffmanEncode(qdata, DCEhuff, ACEhuff, chData);

  void main(void) 
  {
#ifdef PIPED_SW    
    mduWide = 0;
    
    handleData.main();
    sendHData.main();

    // Run software in parallel with hardware in pipelined fashion
    for(mduWide = 1; mduWide < MDUWide; mduWide++)
    {
      handleData.main();
      recvDData.main();
      sendHData.main();
      quantization.main();
      huffmanEncode.main();
    };
    
    recvDData.main();
    quantization.main();
    huffmanEncode.main();
  }
#else
    for(mduWide = 0; mduWide < MDUWide; mduWide++)
    {
      handleData.main();
      sendHData.main();
      recvDData.main();
      quantization.main();
      huffmanEncode.main();
    }
  }
#endif  
};



behavior JpegEncode(i_byte_receiver chPixel,
		    in int ImageWidth, in int ImageHeight,
		    in EHuff DCEhuff,  in EHuff ACEhuff,
		    i_block_sender chHData, i_block_receiver chDData,
		    ISendBits chData)
{
  int mduHigh;
  int MDUWide, MDUHigh;
  unsigned char* stripe;
  
  ReceiveData receiveData(chPixel, mduHigh, ImageWidth, ImageHeight,
			  MDUWide, stripe);
  JpegEncodeStripe encodeStripe(mduHigh, stripe, MDUWide, DCEhuff, ACEhuff, 
				chHData, chDData, chData);
  
  void main(void) 
  {
    for(mduHigh=0, MDUHigh=(ImageHeight+7)>>3; mduHigh < MDUHigh; mduHigh++)
    {
      receiveData.main();
      encodeStripe.main();
    }
  }
};

