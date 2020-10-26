/************	External functions for JPEG encode 	*******/
/************	extern.h  	*******/
/************	A. Gerstlauer	*******/
/************	06/08/2001	*******/

#include <stdio.h>
#include <stdlib.h>


/**************************************************************************
**		Error handling, debug output
**************************************************************************/


// Error messages
extern void error(const char *Format, ...);

// Normal (and debugging) messages
extern void msg(const char *Format, ...);

// Functions which return standard file descriptors
// ("stdout" and "stderr" are defines which get lost in binary import)
extern FILE* stdOut();
extern FILE* stdErr();

