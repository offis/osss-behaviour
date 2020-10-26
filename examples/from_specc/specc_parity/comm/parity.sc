/****************************************************************************
*  Title: parity.sc
*  Author: Dongwan Shin
*  Date: 12/03/2000
*  Description: top behavior for parity checker
****************************************************************************/

import "ones";
import "even";

behavior parity(in unsigned bit[31:0] Inport, out unsigned bit[31:0] Outport, 
	iISignal Start, iOSignal Done)
{
	cBus cDataBus;
	cBus cOcountBus;

	even U00(Inport, Outport, Start, Done, cDataBus, cOcountBus);
	ones U01(cDataBus, cOcountBus);

	void main (void)
	{
		par {
			U00.main();
			U01.main();
		}
	}
};
