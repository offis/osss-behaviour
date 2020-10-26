/****************************************************************************
*  Title: ones.sc
*  Author: Dongwan Shin
*  Date: 12/03/2000
*  Description: Communication model for one's counter
****************************************************************************/

import "bus";

behavior ones(iBus DataBus, iBus OcountBus)
{
	void main(void) {
		unsigned bit[31:0] data;
		unsigned bit[31:0] ocount;
		unsigned bit[31:0] mask;
		unsigned bit[31:0] temp;

		while (1) {
			data = DataBus.read();
			ocount = 0;
			mask = 1;

			while (data != 0) {
				temp = data & mask;
				ocount = ocount + temp;
				data = data >> 1;
			}

			OcountBus.write(ocount);
		}
	}
};
