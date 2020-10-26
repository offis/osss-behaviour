/****************************************************************************
*  Title: io.sc
*  Author: Dongwan Shin
*  Date: 11/15/2000
*  Description: input/output for testbench
****************************************************************************/

// I/O for testbench
#include <stdio.h>
#include <stdlib.h>

behavior IO(in event clk, out unsigned bit[0:0] rst, 
	out unsigned bit[31:0] Inport, out unsigned bit[31:0] Outport, 
	out unsigned bit[0:0] Start, in unsigned bit[0:0] Done)
{
	void main(void) {
		char buf[16];

		rst = 1b;
		Start = 0b;
		wait(clk);
		wait(clk);

		rst = 0b;		// deassign reset

		while (1) {
			printf("Input for one's counter: ");
			gets(buf);
			Inport = atoi(buf);
			Start = 1b;
			wait(clk);
			while (Done != 1b) {
				wait(clk);
			}
			printf("parity checker output(%s) = %u\n", buf, 
				(unsigned int)Outport);
			Start = 0b;
			waitfor(100);

		}
	}
};
