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


#include "const.sh"

import "global";
import "adapter";


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

void PrintHuffman(EHuff *ehuff, XHuff *xhuff)
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


void EncodeHuffman(EHuff* ehuff, int value, ISendBits chData)
{
  if (ehuff->ehufsi[value]) {
    chData.WriteBits(ehuff->ehufsi[value], ehuff->ehufco[value]);
  }
  else {
    msg("Null Code for [%d] Encountered:\n", value);
    msg("*** Dumping Huffman Table ***\n");
    PrintHuffman(ehuff, 0);
    msg("***\n");
    exit(-1);
  }
}


behavior ZigzagMatrix(in int imatrix[BLOCKSIZE], int omatrix[BLOCKSIZE])
{
  void main(void)
  {
    int i, z;

#ifdef DEBUG
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




behavior EncodeDC(in int matrix[BLOCKSIZE], in EHuff Ehuff,
		  ISendBits chData)
{
  int LastDC = 0;
  
  void main(void)
  {
    int s, diff, coef, cofac;

#ifdef DEBUG
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

    EncodeHuffman(&Ehuff, s, chData);
    if (diff < 0) {
      diff--;
    }

    chData.WriteBits(s, diff);
  }
};

  
behavior EncodeAC(in int matrix[BLOCKSIZE], in EHuff Ehuff, 
		  ISendBits chData)
{
  void main(void)
  {
    int i, k, r, ssss, cofac;

#ifdef DEBUG
    msg("EncodeAC: Huffman encoding...\n");
    // PrintHuffman(&Ehuff, 0);
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

      if (matrix[k] == 0) {
	if (k == BLOCKSIZE-1) {
	  EncodeHuffman(&Ehuff, 0, chData);
	  break;
	}
	r++;
      }
      else {
	while (r > 15) {
	  EncodeHuffman(&Ehuff, 240, chData);
	  r -= 16;
	}
	i = 16 * r + ssss;
	r = 0;
	EncodeHuffman(&Ehuff, i, chData);
	if (matrix[k] < 0) 
	  chData.WriteBits(ssss, matrix[k]-1);
	else 
	  chData.WriteBits(ssss, matrix[k]);
      }
    }
  }
};

  
behavior HuffmanEncode(in int QData[BLOCKSIZE],
		       in EHuff DCEhuff, in EHuff ACEhuff,
		       ISendBits chData)
{
  int OData[BLOCKSIZE];
	
  ZigzagMatrix zigzagMatrix(QData, OData);
  EncodeDC     encodeDC(OData, DCEhuff, chData);
  EncodeAC     encodeAC(OData, ACEhuff, chData);
  
  void main(void) {
    zigzagMatrix.main();
    encodeDC.main();
    encodeAC.main();
  }
};

