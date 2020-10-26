/****************************************************************************
*  Title: even.sc
*  Author: Dongwan Shin
*  Date: 04/20/2001
*  Description: Specification model for even parity checker
****************************************************************************/

// #include <stdio.h>

behavior even(in unsigned int Inport, out unsigned int Outport, 
	in event Start, out event Done, out unsigned int data, 
	in unsigned int ocount, out event istart, in event idone)
{
	void main(void) {
		unsigned int mask;

		while (1) {
			wait(Start);

			data = Inport;
			mask = 0x0001;
			
			notify(istart);	// start one's counter

			wait(idone); 	// wait for the result of one's counter
			
			Outport = ocount & mask; // even parity checker

			notify(Done);
		}
	}
};
