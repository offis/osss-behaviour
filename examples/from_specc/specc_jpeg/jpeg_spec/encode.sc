/************	JPEG encode 	*******/
/************	encode.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/06/2001	*******/

#include "const.sh"

import "handle";
import "dct";
import "quant";
import "huff";


behavior JpegEncodeStripe(in int mduHigh, 
			  in unsigned char* stripe, in int MDUWide,
			  in EHuff DCEhuff, in EHuff ACEhuff,
			  ISendBits chData)
{
  int mduWide;
  
  piped int hdata[BLOCKSIZE];
  piped int ddata[BLOCKSIZE];
  piped int qdata[BLOCKSIZE];
  
  HandleData	handleData(mduWide, mduHigh, stripe, MDUWide, hdata);
  DCT		dct(hdata, ddata);
  Quantization	quantization(ddata, qdata);
  HuffmanEncode	huffmanEncode(qdata, DCEhuff, ACEhuff, chData);

  void main(void) 
  {
    pipe(mduWide = 0; mduWide < MDUWide; mduWide++)
    {
      handleData.main();
      dct.main();
      quantization.main();
      huffmanEncode.main();
    }
  }
};



behavior JpegEncode(i_byte_receiver chPixel,
		    in int ImageWidth, in int ImageHeight,
		    in EHuff DCEhuff,  in EHuff ACEhuff,
		    ISendBits chData)
{
  int mduHigh;
  int MDUHigh;
  
  piped int MDUWide;
  piped unsigned char* stripe;
  
  ReceiveData receiveData(chPixel, mduHigh, ImageWidth, ImageHeight,
			  MDUWide, stripe);
  JpegEncodeStripe encodeStripe(mduHigh, stripe, MDUWide, DCEhuff, ACEhuff, 
				chData);
  
  void main(void) 
  {
    pipe(mduHigh=0, MDUHigh=(ImageHeight+7)>>3; mduHigh < MDUHigh; mduHigh++)
    {
      receiveData.main();
      encodeStripe.main();
    }
  }
};

