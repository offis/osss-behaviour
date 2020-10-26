/****************************************************************************
*  Title: ones.sc
*  Author: Dongwan Shin
*  Date: 04/20/2001
*  Description: Specification model for one's counter
****************************************************************************/

behavior ones(in unsigned int inport, out unsigned int outport, 
	in event start, out event done)
{
	void main(void) {
		unsigned int data;
		unsigned int ocount;
		unsigned int mask;
		unsigned int temp;

		while (1) {
			wait(start);
			data = inport;
			ocount = 0;
			mask = 1;

			while (data != 0) {
				temp = data & mask;
				ocount = ocount + temp;
				data = data >> 1;
			}

			outport = ocount;
			notify(done);
		}
	}
};
