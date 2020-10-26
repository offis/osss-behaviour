#ifndef DCT_INCLUDED
#define DCT_INCLUDED

/************	JPEG encode 	*******/
/************	DCT.sc  	*******/
/************	Hanyu Yin	*******/
/************	05/11/2000	*******/

#include "const.h"
#include "global.h"

/**************************************************************************
* DCT
**************************************************************************/

#define NO_MULTIPLY

#ifdef NO_MULTIPLY
#define LS(r,s) ((r) << (s))
#define RS(r,s) ((r) >> (s))       /* Caution with rounding... */
#else
#define LS(r,s) ((r) * (1 << (s)))
#define RS(r,s) ((r) / (1 << (s))) /* Correct rounding */
#endif

/* Cos constants */
#define c1d4 362L
#define c1d8 473L
#define c3d8 196L
#define c1d16 502L
#define c3d16 426L
#define c5d16 284L
#define c7d16 100L
#define MSCALE(expr)  RS((expr),9)

/*
  VECTOR_DEFINITION makes the temporary variables vectors.
  Useful for machines with small register spaces.
*/
#ifdef VECTOR_DEFINITION
#define a0 a[0]
#define a1 a[1]
#define a2 a[2]
#define a3 a[3]
#define b0 b[0]
#define b1 b[1]
#define b2 b[2]
#define b3 b[3]
#define c0 c[0]
#define c1 c[1]
#define c2 c[2]
#define c3 c[3]
#endif

/*START*/
/*BFUNC
ChenDCT() implements the Chen forward dct. Note that there are two
input vectors that represent x=input, and y=output, and must be
defined (and storage allocated) before this routine is called
EFUNC*/

OSSS_BEHAVIOUR(ChenDct)
{
 public:
  osss::osss_in<osss::osss_array<int, BLOCKSIZE> >  x;
  osss::osss_out<osss::osss_array<int, BLOCKSIZE> > y;

  BEHAVIOUR_CTOR(ChenDct) {}

  void main()
  {
    register int i;
    register int *aptr,*bptr;
#ifdef VECTOR_DEFINITION
    register int a[4];
    register int b[4];
    register int c[4];
#else  
    register int a0,a1,a2,a3;
    register int b0,b1,b2,b3;
    register int c0,c1,c2,c3;
#endif
    
    int local_x[BLOCKSIZE];
    // copy x into local_x
    for(unsigned int j=0; j<BLOCKSIZE; ++j)
      local_x[j] = x[j];

    int local_y[BLOCKSIZE];

    /* Loop over columns */
    //
    // TODO: This needs to be rewritten without pointer-arithmetics
    //
    for(i=0;i<8;i++)
    {
      // CAUTION: This is an ugly hack !!!
      aptr = local_x + i;
      bptr = aptr+56;

      a0 = LS((*aptr+*bptr),2);
      c3 = LS((*aptr-*bptr),2);
      aptr += 8;
      bptr -= 8;
      a1 = LS((*aptr+*bptr),2);
      c2 = LS((*aptr-*bptr),2);
      aptr += 8;
      bptr -= 8;
      a2 = LS((*aptr+*bptr),2);
      c1 = LS((*aptr-*bptr),2);
      aptr += 8;
      bptr -= 8;
      a3 = LS((*aptr+*bptr),2);
      c0 = LS((*aptr-*bptr),2);
      
      b0 = a0+a3;
      b1 = a1+a2;
      b2 = a1-a2;
      b3 = a0-a3;

      aptr = (&local_y[0]) + i;

      *aptr = MSCALE(c1d4*(b0+b1));
      aptr[32] = MSCALE(c1d4*(b0-b1));

      aptr[16] = MSCALE((c3d8*b2)+(c1d8*b3));
      aptr[48] = MSCALE((c3d8*b3)-(c1d8*b2));

      b0 = MSCALE(c1d4*(c2-c1));
      b1 = MSCALE(c1d4*(c2+c1));

      a0 = c0+b0;
      a1 = c0-b0;
      a2 = c3-b1;
      a3 = c3+b1;

      aptr[8] = MSCALE((c7d16*a0)+(c1d16*a3));
      aptr[24] = MSCALE((c3d16*a2)-(c5d16*a1));
      aptr[40] = MSCALE((c3d16*a1)+(c5d16*a2));
      aptr[56] = MSCALE((c7d16*a3)-(c1d16*a0));
    }
  
    for(i = 0; i < 8; i++)
    {       /* Loop over rows */
      aptr = (&local_y[0]) + LS(i,3);
      bptr = aptr+7;

      c3 = RS((*(aptr)-*(bptr)),1);
      a0 = RS((*(aptr++)+*(bptr--)),1);
      c2 = RS((*(aptr)-*(bptr)),1);
      a1 = RS((*(aptr++)+*(bptr--)),1);
      c1 = RS((*(aptr)-*(bptr)),1);
      a2 = RS((*(aptr++)+*(bptr--)),1);
      c0 = RS((*(aptr)-*(bptr)),1);
      a3 = RS((*(aptr)+*(bptr)),1);

      b0 = a0+a3;
      b1 = a1+a2;
      b2 = a1-a2;
      b3 = a0-a3;

      aptr = (&local_y[0]) + LS(i,3);
      
      *aptr = MSCALE(c1d4*(b0+b1));
      aptr[4] = MSCALE(c1d4*(b0-b1));
      aptr[2] = MSCALE((c3d8*b2)+(c1d8*b3));
      aptr[6] = MSCALE((c3d8*b3)-(c1d8*b2));

      b0 = MSCALE(c1d4*(c2-c1));
      b1 = MSCALE(c1d4*(c2+c1));

      a0 = c0+b0;
      a1 = c0-b0;
      a2 = c3-b1;
      a3 = c3+b1;

      aptr[1] = MSCALE((c7d16*a0)+(c1d16*a3));
      aptr[3] = MSCALE((c3d16*a2)-(c5d16*a1));
      aptr[5] = MSCALE((c3d16*a1)+(c5d16*a2));
      aptr[7] = MSCALE((c7d16*a3)-(c1d16*a0));
    }

    /* We have an additional factor of 8 in the Chen algorithm. */

    for(i = 0, aptr = (&local_y[0]); i < 64; i++, aptr++)
      *aptr = (((*aptr<0) ? (*aptr-4) : (*aptr+4))/8);

    // write to output port
    for(unsigned int j=0; j<BLOCKSIZE; ++j)
      y[j] = local_y[j];
  }
};


OSSS_BEHAVIOUR(PreshiftDctMatrix)
{
 public:
  osss::osss_in<osss::osss_array<int, BLOCKSIZE> >  mi; 
  osss::osss_out<osss::osss_array<int, BLOCKSIZE> > mo;

 protected:
  int shift;

 public:
  PreshiftDctMatrix(osss::osss_behaviour_name name, int shift_in) :
    osss::osss_behaviour(name),
    shift(shift_in)
  {}

  void main()
  {
    int i;
    for(i = 0; i<BLOCKSIZE; i++)
      mo[i] = mi[i] - shift;
  }
};


OSSS_BEHAVIOUR(BoundDctMatrix)
{
 public:
  osss::osss_in<osss::osss_array<int, BLOCKSIZE> >  mi;
  osss::osss_out<osss::osss_array<int, BLOCKSIZE> > mo;

 protected:
  int bound;

 public:
  BoundDctMatrix(osss::osss_behaviour_name name, int bound_in) :
    osss::osss_behaviour(name),
    bound(bound_in)
  {}

  void main()
  {
    int i;
    for (i = 0; i < BLOCKSIZE; i++) {
      if (mi[i] < -bound)	
	mo[i] = -bound;
      else if (mi[i] > bound)
	mo[i] = bound;
      else
	mo[i] = mi[i];
    }
  }
};

#ifdef PIPELINED_INNER
OSSS_PIPELINE_STAGE(DCT)
#else
OSSS_BEHAVIOUR(DCT)
#endif
{
 public:
  osss::osss_in<osss::osss_array<int, BLOCKSIZE> >  HData;
  osss::osss_out<osss::osss_array<int, BLOCKSIZE> > DData;

 protected:
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE> > HDataDct;
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE> > DDataDct;

  PreshiftDctMatrix preshiftDctMatrix;
  ChenDct           chenDct;
  BoundDctMatrix    boundDctMatrix;

 public:
#ifdef PIPELINED_INNER
  PIPELINE_STAGE_CTOR(DCT) :
#else
  BEHAVIOUR_CTOR(DCT) :
#endif
    HDataDct("HDataDct"),
    DDataDct("DDataDct"),
    preshiftDctMatrix("preshiftDctMatrix", 128),
    chenDct("chenDct"),
    boundDctMatrix("boundDctMatrix", 1023)
  {
    preshiftDctMatrix.mi(HData);
    preshiftDctMatrix.mo(HDataDct);
   
    chenDct.x(HDataDct);
    chenDct.y(DDataDct);

    boundDctMatrix.mi(DDataDct);
    boundDctMatrix.mo(DData);
      
    osss_seq( preshiftDctMatrix, chenDct, boundDctMatrix );
  }
  
};

#endif
