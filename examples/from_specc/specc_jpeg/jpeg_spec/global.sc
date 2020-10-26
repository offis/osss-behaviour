/************	Global Function for JPEG encode 	*******/
/************	global.sc  	*******/
/************	Hanyu Yin	*******/
/************	05/11/2000	*******/

#include <stdlib.h>

#include "extern.h"       // link against external library

#include "const.sh"


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


typedef struct huffman_standard_structure {
  int bits[36]; 
  int huffval[257];
} XHuff;

typedef struct huffman_encoder {
  int ehufco[257];
  int ehufsi[257];
} EHuff;


/**************************************************************************
**		Error handling, debug output
**************************************************************************/

/* External: included from extern.h */

