/************	JPEG encode 	*******/
/************	encode.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/06/2001	*******/

#include "const.sh"
#include "bus.sh"

import "bus";
import "handle";
import "dct";
import "quant";
import "huff";


behavior BHDataSend(in int HData[BLOCKSIZE], IBusMaster bus)
{
  void main(void)
  {
    bus.sendBlock(HData, DCT_DATAIN);
  }
};


behavior BDDataRecv(IBusMaster bus, out int DData[BLOCKSIZE])
{
  void main(void)
  {
    bus.recvBlock(&DData[0], DCT_DATAOUT);
  }
};



behavior JpegEncodeStripe(in int mduHigh, 
			  in unsigned char* stripe, in int MDUWide,
			  in EHuff DCEhuff, in EHuff ACEhuff,
			  IBusMaster bus,
			  ISendBits chData)
{
  int mduWide;
  
  int hdata[BLOCKSIZE];
  int ddata[BLOCKSIZE];
  int qdata[BLOCKSIZE];
  
  HandleData	handleData(mduWide, mduHigh, stripe, MDUWide, hdata);
  BHDataSend    sendHData(hdata, bus);
  BDDataRecv    recvDData(bus, ddata);
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
		    IBusMaster bus,
		    ISendBits chData)
{
  int mduHigh;
  int MDUWide, MDUHigh;
  unsigned char* stripe;
  
  ReceiveData receiveData(chPixel, mduHigh, ImageWidth, ImageHeight,
			  MDUWide, stripe);
  JpegEncodeStripe encodeStripe(mduHigh, stripe, MDUWide, DCEhuff, ACEhuff, 
				bus, chData);
  
  void main(void) 
  {
    for(mduHigh=0, MDUHigh=(ImageHeight+7)>>3; mduHigh < MDUHigh; mduHigh++)
    {
      receiveData.main();
      encodeStripe.main();
    }
  }
};

