/************	JPEG encode 	*******/
/************	jpeg.sc		*******/
/************	A. Gerstlauer	*******/
/************	06/06/2001	*******/

#include <sim.sh>

#include "const.sh"


import "default";
import "header";
import "encode";



behavior JpegStart(i_int_receiver chHeader, 
		   out int ImageWidth, out int ImageHeight)
{
  void main(void)
  {
    int width, height;
    
#ifdef DEBUG	
    msg("HandleData: Receiving header with image size...\n");
#endif
    
    chHeader.receive(&width);
    chHeader.receive(&height);
    ImageWidth  = width;
    ImageHeight = height;    

#ifdef DEBUG	
    msg("HandleData: Got header width=%d, height=%d\n", width, height);
#endif
    
#if defined(VERBOSE) || defined (DEBUG)
    msg("****************************************************\n");
    msg("Time %s: JPEG Encoder Begin...\n", time2str(now()));
    msg("****************************************************\n");
#endif
  }
};


behavior JpegInit(i_int_receiver chHeader, ISendBits chData,
		  out EHuff DCEhuff, out EHuff ACEhuff,
		  int ImageWidth, int ImageHeight)
{
  XHuff DCXhuff, ACXhuff;

  JpegStart start(chHeader, ImageWidth, ImageHeight);
  JpegDefaultHuffman huffman(DCXhuff, DCEhuff, ACXhuff, ACEhuff);
  JpegHeader header(ImageWidth, ImageHeight, DCXhuff, ACXhuff, chData);
  
  void main(void)
  {
    start.main();
    huffman.main();
    header.main();
  }
};



behavior JpegEnd(ISendBits chData)
{
  void main(void)
  {        
    chData.WriteBits(-1, 0);
    chData.WriteMarker(M_EOI);

#if defined(VERBOSE) || defined (DEBUG)
    msg("****************************************************\n");
    msg("Time %s: JPEG Encoder End...\n", time2str(now()));
    msg("****************************************************\n");
#endif
  }
};



behavior JpegSW(i_int_receiver chHeader, i_byte_receiver chPixel,
		i_block_sender chHData, i_block_receiver chDData,
		i_byte_sender chData)
{
  int imageWidth, imageHeight;  
  EHuff DCEhuff, ACEhuff;
  
  CBitAdapter bitAdapter(chData);
  
  JpegInit   init(chHeader, bitAdapter, DCEhuff, ACEhuff, 
		  imageWidth, imageHeight);
  JpegEncode encode(chPixel, imageWidth, imageHeight, DCEhuff, ACEhuff, 
		    chHData, chDData, bitAdapter);
  JpegEnd    end(bitAdapter);
  
  void main(void)
  {
    init.main();
    encode.main();
    end.main();
  }
};


