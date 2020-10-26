/****************************************************************************
*  Title: ones.sc
*  Author: Dongwan Shin
*  Date: 04/20/2001
*  Description: Architecture model for one's counter
****************************************************************************/

import "chan";

behavior ones(iRecv iData, iSend iOcount)
{
	void main(void) {
		unsigned int data;
		unsigned int ocount;
		unsigned int mask;
		unsigned int temp;

		while (1) {
			// waitfor(0);
			data = iData.recv();
			ocount = 0;
			mask = 1;

			while (data != 0) {
				temp = data & mask;
				ocount = ocount + temp;
				data = data >> 1;
			}
//			waitfor(0);
			iOcount.send(ocount);
		}
	}
};
