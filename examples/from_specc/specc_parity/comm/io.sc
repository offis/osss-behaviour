/****************************************************************************
*  Title: io.sc
*  Author: Dongwan Shin
*  Date: 12/03/2000
*  Description: input/outport for testbench
****************************************************************************/

import "bus";

// get interger from stdin
behavior IO(out unsigned bit[31:0] inport, in unsigned bit[31:0] outport, 
	iOSignal start, iISignal done)
{
	void main(void) {
		char buf[32];

		while (1) {
			start.assign(0); // maintain start signal low
			printf("Input for parity checker: ");
			gets(buf);
			inport = atoi(buf);

			done.waitval(0);

			start.assign(1);	// start parity checker

			done.waitval(1);
			printf("parity checker output(%s) = %u\n", buf, 
				(unsigned int)outport);
		}
	}
};
