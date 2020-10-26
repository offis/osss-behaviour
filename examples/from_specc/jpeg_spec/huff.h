#ifndef HUFF_INCLUDED
#define HUFF_INCLUDED

/************	JPEG encode 	*******/
/************	huff.sc		*******/
/************	Hanyu Yin	*******/
/************	05/11/2000	*******/

/*************************************************************
huffman.c

This file represents the core Huffman routines, most of them
implemented with the JPEG reference. These routines are not very fast
and can be improved, but comprise very little of software run-time.

*************************************************************/

#include "const.h"
#include "global.h"
#include "adapter.h"

static const int csize[] = {
0,
1,
2, 2,
3, 3, 3, 3,
4, 4, 4, 4, 4, 4, 4, 4,
5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8}; 

static const int ZigzagIndex[] =  /* Is zig-zag map for matrix -> scan array */
	{0,  1,  5,  6, 14, 15, 27, 28,
 	2,  4,  7, 13, 16, 26, 29, 42,
 	3,  8, 12, 17, 25, 30, 41, 43,
 	9, 11, 18, 24, 31, 40, 44, 53,
	10, 19, 23, 32, 39, 45, 52, 54,
	20, 22, 33, 38, 46, 51, 55, 60,
	21, 34, 37, 47, 50, 56, 59, 61,
	35, 36, 48, 49, 57, 58, 62, 63};

void PrintTable(int *table)
{
  int i,j;

  for(i=0;i<16;i++)
    {
      for(j=0;j<16;j++)
        {
          msg("%2x ",*(table++));
        }
      msg("\n");
    }
}

void PrintHuffman(EHuff* ehuff, XHuff *xhuff)
{
  int i;

  if (xhuff)
    {
      msg("Bits: [length:number]\n");
      for(i=1;i<9;i++)
        {
          msg("[%d:%d]",i,xhuff->bits[i]);
        }
      msg("\n");
      for(i=9;i<17;i++)
        {
          msg("[%d:%d]",i,xhuff->bits[i]);
        }
      msg("\n");

      msg("Huffval:\n");
      PrintTable(xhuff->huffval);
    }
  if (ehuff)
    {
      msg("Ehufco:\n");
      PrintTable(ehuff->ehufco);
      msg("Ehufsi:\n");
      PrintTable(ehuff->ehufsi);
    }
}

OSSS_BEHAVIOUR(ZigzagMatrix)
{
  osss::osss_in<osss::osss_array<int, BLOCKSIZE> >  imatrix; 
  osss::osss_out<osss::osss_array<int, BLOCKSIZE> > omatrix;

  BEHAVIOUR_CTOR(ZigzagMatrix) {}

  void main()
  {
    int i, z;

#ifdef JPEG_DEBUG
    msg("ZigzagMatrix: converting matrix to scan array...\n");
    //for(i = 0; i < 8; i++) {
    //  for(z = 0; z < 8; z++) {
    //    msg("%2x  ", imatrix[i]);
    //  }
    //  msg("\n");
    //}
#endif
    
    for (i = 0; i < BLOCKSIZE; i++) {
      z = ZigzagIndex[i];
      omatrix[z] = imatrix[i];
    }
  }
};


OSSS_BEHAVIOUR(EncodeDC)
{
 public:
  osss::osss_in<osss::osss_array<int, BLOCKSIZE> > matrix;
  osss::osss_in<EHuff> Ehuff;
  
  osss::osss_port<ISendBits> chData;

 protected:
  int LastDC;

 public:
  BEHAVIOUR_CTOR(EncodeDC) : LastDC(0) {}
  
  void main()
  {
    int s, diff, coef, cofac;

#ifdef JPEG_DEBUG
    msg("EncodeDC: Huffman encoding...\n");
    // PrintHuffman(&Ehuff, 0);
#endif
    
    coef = matrix[0];
    
    diff = coef - LastDC;
    LastDC = coef;
    cofac = abs(diff);
    if (cofac < 256) {
      s = csize[cofac];
    }
    else {
      cofac = cofac >> 8;
      s = csize[cofac] + 8;
    }

    // read from port
    EHuff tmp_ehuff = Ehuff;
    EncodeHuffman(&tmp_ehuff, s);
    if (diff < 0) {
      diff--;
    }

    chData->WriteBits(s, diff);
  }

 private:
  void EncodeHuffman(EHuff* ehuff, int value)
  {
    if (ehuff->ehufsi[value]) {
      chData->WriteBits(ehuff->ehufsi[value], ehuff->ehufco[value]);
    }
    else {
      msg("Null Code for [%d] Encountered:\n", value);
      msg("*** Dumping Huffman Table ***\n");
      PrintHuffman(ehuff, 0);
      msg("***\n");
      exit(-1);
    }
  }

};

  
OSSS_BEHAVIOUR(EncodeAC)
{
 public:
  osss::osss_in<osss::osss_array<int, BLOCKSIZE> > matrix;
  osss::osss_in<EHuff> Ehuff;
  
  osss::osss_port<ISendBits> chData;

  BEHAVIOUR_CTOR(EncodeAC) {}

  void main(void)
  {
    int i, k, r, ssss, cofac;

#ifdef JPEG_DEBUG
    msg("EncodeAC: Huffman encoding...\n");
#endif    
    
    for (k = r = 0; ++k < BLOCKSIZE; ) 
    {
      cofac = abs(matrix[k]);
      if (cofac < 256) {
	ssss = csize[cofac];
      }
      else {
	cofac = cofac >> 8;
	ssss = csize[cofac] + 8;
      }

      EHuff tmp_ehuff = Ehuff;
      if (matrix[k] == 0) {
	if (k == BLOCKSIZE-1) {
	  EncodeHuffman(&tmp_ehuff, 0);
	  break;
	}
	r++;
      }
      else {
	while (r > 15) {
	  EncodeHuffman(&tmp_ehuff, 240);
	  r -= 16;
	}
	i = 16 * r + ssss;
	r = 0;
	EncodeHuffman(&tmp_ehuff, i);
	if (matrix[k] < 0) 
	  chData->WriteBits(ssss, matrix[k]-1);
	else 
	  chData->WriteBits(ssss, matrix[k]);
      }
    }
  }

 private:
  void EncodeHuffman(EHuff* ehuff, int value)
  {
    if (ehuff->ehufsi[value]) {
      chData->WriteBits(ehuff->ehufsi[value], ehuff->ehufco[value]);
    }
    else {
      msg("Null Code for [%d] Encountered:\n", value);
      msg("*** Dumping Huffman Table ***\n");
      PrintHuffman(ehuff, 0);
      msg("***\n");
      exit(-1);
    }
  }

};

#ifdef PIPELINED_INNER
OSSS_PIPELINE_STAGE(HuffmanEncode)
#else
OSSS_BEHAVIOUR(HuffmanEncode)
#endif
{
 public:
  osss::osss_in<osss::osss_array<int, BLOCKSIZE> > QData;
  osss::osss_in<EHuff> DCEhuff, ACEhuff;
  
  osss::osss_port<ISendBits> chData;

 protected:
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE> > OData;
	
  ZigzagMatrix zigzagMatrix;
  EncodeDC     encodeDC;
  EncodeAC     encodeAC;

 public:
#ifdef PIPELINED_INNER
  PIPELINE_STAGE_CTOR(HuffmanEncode) :
#else
  BEHAVIOUR_CTOR(HuffmanEncode) :
#endif
    OData("OData"),
    zigzagMatrix("zigzagMatrix"),
    encodeDC("encodeDC"),
    encodeAC("encodeAC")
  {
    zigzagMatrix.imatrix(QData);
    zigzagMatrix.omatrix(OData);
    
    encodeDC.matrix(OData);
    encodeDC.Ehuff(DCEhuff);
    encodeDC.chData(chData);
   
    encodeAC.matrix(OData);
    encodeAC.Ehuff(ACEhuff);
    encodeAC.chData(chData);
    
    osss_seq( zigzagMatrix, encodeDC, encodeAC );
  }
  
};

#endif
