/****************************************************************************
*  Title: parity.sc
*  Author: Dongwan Shin
*  Date: 04/20/2001
*  Description: Specification model for even parity checker
****************************************************************************/

import "ones";
import "even";

behavior parity(in unsigned int Inport, out unsigned int Outport, 
	in event Start, out event Done)
{
	ChMP cData;
	ChMP cOcount;

	even U00(Inport, Outport, Start, Done, cData, cOcount);
	ones U01(cData, cOcount);

	void main(void) {
		par {
			U00.main();
			U01.main();
		}
	}
};
