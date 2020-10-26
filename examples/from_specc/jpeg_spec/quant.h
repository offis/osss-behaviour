#ifndef QUANT_INCLUDED
#define QUANT_INCLUDED

/************	JPEG encode 	*******/
/************	quant.sc	*******/
/************	Hanyu Yin	*******/
/************	05/11/2000	*******/

#include "const.h"
#include "global.h"

#ifdef PIPELINED_INNER
OSSS_PIPELINE_STAGE(Quantization)
#else
OSSS_BEHAVIOUR(Quantization)
#endif
{
 public:
  osss::osss_in<osss::osss_array<int, BLOCKSIZE> >  DData;
  osss::osss_out<osss::osss_array<int, BLOCKSIZE> > QData;

#ifdef PIPELINED_INNER
  PIPELINE_STAGE_CTOR(Quantization) {}
#else
  BEHAVIOUR_CTOR(Quantization) {}
#endif

  void main()
  {
    int i, m, q;
    for (i = 0; i < BLOCKSIZE; i++) {
      m = DData[i];
      q = QuantizationMatrix[i];
      if (m > 0) {
	QData[i] = (m + q/2) / q;
      }
      else {
	QData[i] = (m - q/2) / q;
      }
    }
  }

};

#endif
