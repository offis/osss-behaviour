#ifndef QUANT_INCLUDED
#define QUANT_INCLUDED

/************	JPEG encode 	*******/
/************	quant.sc	*******/
/************	Hanyu Yin	*******/
/************	05/11/2000	*******/

#include "const.h"
#include "global.h"

OSSS_BEHAVIOUR(Quantization)
{
 public:
  osss::osss_in<osss::osss_array<int, BLOCKSIZE> >  DData;
  osss::osss_out<osss::osss_array<int, BLOCKSIZE> > QData;

  BEHAVIOUR_CTOR(Quantization) {}

  void main()
  {
    int i, m, q;
    
    wait(93000);

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
