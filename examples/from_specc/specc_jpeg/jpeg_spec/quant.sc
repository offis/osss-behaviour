/************	JPEG encode 	*******/
/************	quant.sc	*******/
/************	Hanyu Yin	*******/
/************	05/11/2000	*******/

#include "const.sh"

import "global";



behavior Quantization(in int DData[BLOCKSIZE], out int QData[BLOCKSIZE])
{
  void main(void)
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

