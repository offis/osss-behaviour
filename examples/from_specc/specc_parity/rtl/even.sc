/****************************************************************************
*  Title: even.sc
*  Author: Dongwan Shin
*  Date: 12/09/2000
*  Description: Behavioral RTL model for even parity checker
****************************************************************************/

behavior even(in event clk, in unsigned bit[0:0] rst, 
	in unsigned bit[31:0] Inport, out unsigned bit[31:0] Outport, 
	in unsigned bit[0:0] Start, out unsigned bit[0:0] Done, 
	out unsigned bit[31:0] idata, in unsigned bit[31:0] iocount, 
	out unsigned bit[0:0] istart, in unsigned bit[0:0] idone, 
	in unsigned bit[0:0] ack_istart, out unsigned bit[0:0] ack_idone)
{
	void main(void) {
		unsigned bit[31:0] ocount;
		unsigned bit[31:0] mask;
		enum state { S0, S1, S2, S3 } state;

		state = S0;

		while (1) {
			wait(clk);
			if (rst == 1b) {
				state = S0;
			}
			switch (state) {
				case S0: 
					Done = 0b;
					istart = 0b;
					ack_idone = 0b;
					if (Start == 1b)
						state = S1;
					else
						state = S0;
					break;
				case S1:
					mask = 0x0001;
					idata = Inport;
					istart = 1b;
					if (ack_istart == 1b)
						state = S2;
					else
						state = S1;
					break;
				case S2:
					istart = 0b;
					ocount = iocount;
					if (idone == 1b)	
						state = S3;
					else
						state = S2;
					break;
				case S3:
					Outport = ocount & mask;	// even parity checker
					ack_idone = 1b;
					Done = 1b;
					if (idone == 0)
						state = S0;
					else
						state = S3;
					break;
			}
		}
	}
};
