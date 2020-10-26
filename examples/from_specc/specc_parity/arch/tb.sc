/****************************************************************************
*  Title: tb.sc
*  Author: Dongwan Shin
*  Date: 04/20/2001
*  Description: testbench for partiy checker
****************************************************************************/

import "io";
import "parity";

behavior Main
{
	unsigned int inport, outport;
	event start, done;

	IO U00(inport, outport, start, done);
	parity U01(inport, outport, start, done);

	int main (void)
	{
		par {
			U00.main();
			U01.main();
		}

		return 0;
	}
};
