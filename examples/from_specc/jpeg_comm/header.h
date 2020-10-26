#ifndef HEADER_INCLUDED
#define HEADER_INCLUDED

/************	Creation of header for JPEG encode 	*******/
/************	header.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/06/2001	*******/

#include "const.h"
#include "global.h"
#include "adapter.h"


static const int IZigzagIndex[] = {
        0,  1,  8, 16,  9,  2,  3, 10,
	17, 24, 32, 25, 18, 11,  4,  5,
	12, 19, 26, 33, 40, 48, 41, 34,
	27, 20, 13,  6,  7, 14, 21, 28,
	35, 42, 49, 56, 57, 50, 43, 36,
	29, 22, 15, 23, 30, 37, 44, 51,
	58, 59, 52, 45, 38, 31, 39, 46,
	53, 60, 61, 54, 47, 55, 62, 63
};


OSSS_BEHAVIOUR(JpegHeader)
{
 public:
  osss::osss_in<int>   ImageWidth, ImageHeight;
  osss::osss_in<XHuff> DCXhuff, ACXhuff;

  osss::osss_port<ISendBits> chData;

 protected:

  void WriteHuffman(XHuff* xhuff)
  {
    int i, accum;

    for(accum = 0, i = 1; i <= 16; i++)
    {
      chData->WriteByte(xhuff->bits[i]);
      accum += xhuff->bits[i];
    }
    for(i = 0; i < accum; i++)
    {
      chData->WriteByte(xhuff->huffval[i]);
    }
  }

  void WriteAPP0()
  {
    chData->WriteMarker(M_APP0);

    /* length */
    chData->WriteWord(16);

    /* identifier */
    chData->WriteByte('J');
    chData->WriteByte('F');
    chData->WriteByte('I');
    chData->WriteByte('F');
    chData->WriteByte(0);

    /* version */
    chData->WriteWord(0x0102);

    /* units */
    chData->WriteByte(2);

    /* Xdensity */
    chData->WriteWord(0x001d);

    /* Ydensity */
    chData->WriteWord(0x001d);

    /* Xthumbnail, Ythumbnail */
    chData->WriteWord(0x0000);
  }

  void WriteSOF()
  {
    chData->WriteMarker(M_SOF0);

    /* Lf: frame header length */
    chData->WriteWord(11);

    /* P: precision */
    chData->WriteByte(8);

    /* Y: # lines, X: # samples/line */
    chData->WriteWord(ImageHeight);
    chData->WriteWord(ImageWidth);

    /* Nf: number of component in frame */
    chData->WriteByte(1);

    /* Frame component specification */
    /* Ci: component identifier */
    chData->WriteByte(1);
    /* Hi: horizontal sampling factor, Vi: vertical sampling factor */
    chData->WriteByte(0x11);
    /* Tqi: quantization table destination selector */
    chData->WriteByte(0);
  }

  void WriteDQT()
  {
    int i;

    chData->WriteMarker(M_DQT);

    /* Lq */
    chData->WriteWord(67);

    /* define each quantization table */
    chData->WriteByte(0);
    
    for (i = 0; i < 64; i++) {
      chData->WriteByte(QuantizationMatrix[IZigzagIndex[i]]);
    }
  }
  
  void WriteDHT()
  {
    chData->WriteMarker(M_DHT);

    chData->WriteWord(0x4+0x20+0xc+0xa2);
	
    /* Write DC Huffman */
    /* Tc, Th */
    chData->WriteByte(0);
    // read from port
    XHuff tmp_dcxhuff = DCXhuff;
    WriteHuffman(&tmp_dcxhuff);

    /* Write AC Huffman */
    /* Tc, Th */
    chData->WriteByte(0x10);
    // read from port
    XHuff tmp_acxhuff = ACXhuff;
    WriteHuffman(&tmp_acxhuff);
  }

  void WriteSOS()
  {
    chData->WriteMarker(M_SOS);

    /* Ls: scan header length */
    chData->WriteWord(8);

    /* Ns: number of components in scan */
    chData->WriteByte(1);
    
    /* Csk: scan component selector */
    chData->WriteByte(1);
    
    /* Tdk, Tak: DC/AC entropy coding table selector */
    chData->WriteByte(0);
    
    /* Ss: start of spectral selection or predictor selection */
    chData->WriteByte(0);
    
    /* Se: end of spectral selection */
    chData->WriteByte(63);
    
    /* Ah, Al: successssive approximation bit position high/low */
    chData->WriteByte(0);
  }

 public:
  BEHAVIOUR_CTOR(JpegHeader) {}
  
  void main()
  {
#ifdef JPEG_DEBUG	
    msg("JpegHeader: Writing JPEG header...\n");
#endif
	
    chData->WriteMarker(M_SOI);
    WriteAPP0();
    WriteSOF();
    WriteDQT();
    WriteDHT();
    WriteSOS();
	
#ifdef JPEG_DEBUG	
    msg("JpegHeader: Wrote JPEG header\n");
#endif	
  }
};

#endif
