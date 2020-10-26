/****************************************************************************
*  Title: io.sc
*  Author: Dongwan Shin
*  Date: 04/20/2001
*  Description: testbench
****************************************************************************/

import "chan";

// get interger from stdin
behavior IO(out unsigned int inport, in unsigned int outport, 
	out event start, in event done)
{
	void main(void) {
		char buf[16];

		while (1) {
			printf("Input for parity checker: ");
			gets(buf);
			inport = atoi(buf);
			notify(start);	// start parity checker

			wait(done);	// check result of parity checker
			printf("parity checker output(%s) = %u\n", buf, outport);
		}
	}
};
