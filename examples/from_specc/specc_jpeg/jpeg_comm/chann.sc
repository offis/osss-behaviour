/************	Message-passing channels for JPEG encode 	*******/
/************	chann.sc  	*******/
/************	A. Gerstlauer	*******/
/************	06/06/2001	*******/

#include "const.sh"


/* --------------------- Channels ------------------------ */

import "i_sender";
import "i_receiver";

#include <c_typed_double_handshake.sh>


/**************************************************************************
**		Byte Channel
**************************************************************************/

typedef	char	byte;

DEFINE_I_TYPED_SENDER(byte, byte)
DEFINE_I_TYPED_RECEIVER(byte, byte)
DEFINE_I_TYPED_TRANCEIVER(byte, byte)
DEFINE_C_TYPED_DOUBLE_HANDSHAKE(byte, byte)


/**************************************************************************
**		Integer channel
**************************************************************************/

DEFINE_I_TYPED_SENDER(int, int)
DEFINE_I_TYPED_RECEIVER(int, int)
DEFINE_I_TYPED_TRANCEIVER(int, int)
DEFINE_C_TYPED_DOUBLE_HANDSHAKE(int, int)


/**************************************************************************
**		Block channel
**************************************************************************/

typedef int   block[BLOCKSIZE];

DEFINE_I_TYPED_SENDER(block, block)
DEFINE_I_TYPED_RECEIVER(block, block)
DEFINE_I_TYPED_TRANCEIVER(block, block)
DEFINE_C_TYPED_DOUBLE_HANDSHAKE(block, block)
                
