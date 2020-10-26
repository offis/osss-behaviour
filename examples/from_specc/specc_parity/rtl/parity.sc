/****************************************************************************
*  Title: parity.sc
*  Author: Dongwan Shin
*  Date: 12/03/2000
*  Description: top behavior for parity checker
****************************************************************************/

import "ones";
import "even";

behavior parity(in event clk1, in event clk2, in unsigned bit[0:0] rst, 
	in unsigned bit[31:0] Inport, out unsigned bit[31:0] Outport, 
	in unsigned bit[0:0] Start, out unsigned bit[0:0] Done)
{
	unsigned bit[31:0] data, ocount;
	unsigned bit[0:0] istart, idone;
	unsigned bit[0:0] ack_istart, ack_idone;
	
	even U00(clk1, rst, Inport, Outport, Start, Done, data, ocount, istart,
		idone, ack_istart, ack_idone);
	ones U01(clk2, rst, data, ocount, istart, idone, ack_istart, ack_idone);

	void main (void)
	{
		par {
			U00.main();
			U01.main();
		}
	}
};
