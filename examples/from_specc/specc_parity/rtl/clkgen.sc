/****************************************************************************
*  Title: clkgen.sc
*  Author: Dongwan Shin
*  Date: 11/15/2000
*  Description: clock generator
****************************************************************************/

behavior clkgen(out event clk, in int clk_period)
{
	void main(void) {
		while (1) {
			waitfor(clk_period);
			notify(clk);
		}
	}
};
