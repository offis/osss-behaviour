#ifndef GLOBAL_INCLUDED
#define GLOBAL_INCLUDED

/************	Global Function for JPEG encode 	*******/
/************	global.sc  	*******/
/************	Hanyu Yin	*******/
/************	05/11/2000	*******/

#include <stdlib.h>

#include "extern.h"       // link against external library
#include "const.h"


/**************************************************************************
**		Constants
**************************************************************************/

const int QuantizationMatrix[BLOCKSIZE] = {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99 
};


/**************************************************************************
**		Typedefs
**************************************************************************/

struct huffman_standard_structure 
{
  int bits[36]; 
  int huffval[257];
};

typedef huffman_standard_structure XHuff;

bool operator == (const XHuff& hss1, const XHuff& hss2)
{
  for(unsigned int i=0; i<36; ++i)
  {
    if (hss1.bits[i] != hss2.bits[i])
      return false;
  }

  for(unsigned int i=0; i<257; ++i)
  {
    if(hss1.huffval[i] != hss2.huffval[i])
      return false;
  }
  return true;
}

void
sc_trace( sc_trace_file* tf, 
          const huffman_standard_structure& object,
          const ::std::string& str )
{}

::std::ostream &
operator << ( ::std::ostream & os, const huffman_standard_structure& object )
{ return os; }

//-----------------------------------------------------------------------------

struct huffman_encoder 
{
  int ehufco[257];
  int ehufsi[257];
};

typedef huffman_encoder EHuff;

bool operator == (const EHuff& he1, const EHuff& he2)
{
  for(unsigned int i=0; i<257; ++i)
  {
    if (he1.ehufco[i] != he2.ehufco[i])
      return false;
  }

  for(unsigned int i=0; i<257; ++i)
  {
    if(he1.ehufsi[i] != he2.ehufsi[i])
      return false;
  }
  return true;
}

void
sc_trace( sc_trace_file* tf, 
          const huffman_encoder& object,
          const ::std::string& str ) 
{}

::std::ostream &
operator << ( ::std::ostream & os, const huffman_encoder& object )
{ return os; }

/**************************************************************************
**		Error handling, debug output
**************************************************************************/

/* External: included from extern.h */

#endif
