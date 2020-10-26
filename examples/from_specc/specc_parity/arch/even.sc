/****************************************************************************
*  Title: even.sc
*  Author: Dongwan Shin
*  Date: 04/20/2001
*  Description: Architecture model for even parity checker
****************************************************************************/

import "chan";

behavior even(in unsigned int Inport, out unsigned int Outport, 
	in event Start, out event Done, iSend iData, iRecv iOcount)
{
	void main(void) {
		unsigned int mask;
		unsigned int ocount;

		while (1) {
			wait(Start);
 			mask = 0x0001;

			iData.send(Inport);

			ocount = iOcount.recv();			

			Outport = ocount & mask; // even parity checker

			notify(Done);
		}
	}
};
