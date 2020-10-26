/************	External functions for JPEG encode 	*******/
/************	extern.cc  	*******/
/************	A. Gerstlauer	*******/
/************	06/08/2001	*******/

#include <stdarg.h>

extern "C" {
#include "extern.h"
}


/**************************************************************************
**		Error handling, debug output
**************************************************************************/


// Error messages
void error(const char *Format, ...)
{
  va_list lst;
  
  fprintf(stderr, "ERROR: ");
  va_start(lst, Format);
  vfprintf(stderr, Format, lst);
  va_end(lst);
  exit(1);
} 

// Normal (and debugging) messages
void msg(const char *Format, ...)
{
  va_list lst;
  
  va_start(lst, Format);
  vfprintf(stderr, Format, lst);
  va_end(lst);
} 

// Functions which return standard file descriptors
// ("stdout" and "stderr" are defines which get lost in binary import)
FILE* stdOut()
{
  return stdout;   
}

FILE* stdErr()
{
  return stderr;
}

