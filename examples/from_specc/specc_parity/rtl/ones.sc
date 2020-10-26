/****************************************************************************
*  Title: ones.sc
*  Author: Dongwan Shin
*  Date: 11/15/2000
*  Description: one's counter at behavioral RTL
****************************************************************************/

behavior ones(in event clk, in unsigned bit[0:0] rst, 
	in unsigned bit[31:0] inport, out unsigned bit[31:0] outport, 
	in unsigned bit[0:0] start, out unsigned bit[0:0] done, 
	out unsigned bit[0:0] ack_istart, in unsigned bit[0:0] ack_idone)
{
	void main(void) {
		unsigned bit[31:0] data;
		unsigned bit[31:0] ocount;
		unsigned bit[31:0] mask;
		unsigned bit[31:0] temp;

		enum state { S0, S1, S2, S3, S4, S5, S6, S7 } state;

		state = S0;
		
		while (1) {
			wait(clk);
			if (rst == 1b) {
				state = S0;
			}
			switch (state) {
				case S0 :
					done = 0b;
					ack_istart = 0b;
					if (start == 1b)
						state = S1;
					else 
						state = S0;
					break;
				case S1:
					ack_istart = 1b;
					data = inport;
					state = S2;
					break;
				case S2:
					ocount = 0;
					state = S3;
					break;
				case S3:
					mask = 1;
					state = S4;
					break;
				case S4:
					temp = data & mask;
					state = S5;
					break;
				case S5:
					ocount = ocount + temp;
					state = S6;
					break;
				case S6:
					data = data >> 1;
					if (data == 0)
						state = S7;
					else 
						state = S4;
					break;
				case S7:
					outport = ocount;
					done = 1b;
					if (ack_idone == 1b)
						state = S0;
					else
						state = S7;
					break;
			}
		}
	}
};
