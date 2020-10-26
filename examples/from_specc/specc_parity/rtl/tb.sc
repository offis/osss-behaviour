/****************************************************************************
*  Title: tb.sc
*  Author: Dongwan Shin
*  Date: 12/03/2000
*  Description: testbench for partiy checker
****************************************************************************/

import "io";
import "clkgen";
import "ones";
import "parity";

behavior Main
{
	unsigned bit[31:0] inport, outport;
	unsigned bit[0:0] rst;
	event clk1, clk2;
	unsigned bit[0:0] start, done;

	clkgen U00(clk1, 20);
	clkgen U01(clk2, 4);
	IO U02(clk1, rst, inport, outport, start, done);
	parity U03(clk1, clk2, rst, inport, outport, start, done);

	int main (void)
	{
		par {
			U00.main();
			U01.main();
			U02.main();
			U03.main();
		}
		return 0;
	}
};
