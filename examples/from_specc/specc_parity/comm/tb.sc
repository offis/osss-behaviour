/****************************************************************************
*  Title: tb.sc
*  Author: Dongwan Shin
*  Date: 12/03/2000
*  Description: testbench for partiy checker
****************************************************************************/

import "io";
import "ones";
import "parity";

behavior Main
{
	unsigned bit[31:0] inport, outport;
	cSignal start, done;

	IO io(inport, outport, start, done);
	parity U00(inport, outport, start, done);

	int main (void)
	{
		par {
			io.main();
			U00.main();
		}
		return 0;
	}
};
