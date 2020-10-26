/****************************************************************************
*  Title: even.sc
*  Author: Dongwan Shin
*  Date: 12/03/2000
*  Description: Communication model for even parity checker
****************************************************************************/

import "bus";

behavior even(in unsigned bit[31:0] Inport, out unsigned bit[31:0] Outport, 
	iISignal Start, iOSignal Done, iBus DataBus, iBus OcountBus)
{
	void main(void) {
		unsigned bit[31:0] ocount;
		unsigned bit[31:0] mask;

		while (1) {
			Done.assign(0);
			mask = 0x0001;

			Start.waitval(1);

			DataBus.write(Inport);	

			ocount = OcountBus.read();

			Outport = ocount & mask;

			Done.assign(1);

			waitfor(1);
		}
	}
};
