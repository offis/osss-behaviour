#include <stdio.h>
#include <stdlib.h>

//Signal channel for representation of control signal
interface iOSignal
{
    void assign ( int v ) ;
};

interface iISignal
{
    int val() ;
    void waitval ( int v ) ;
};

channel cSignal()
    implements iISignal, iOSignal
{
    int value=0;
    event ev;

    void assign ( int v )   // assign a value
    {
        value = v ;
        notify ( ev ) ;
    }

    int val()   // return a value
    {
        return value ;
    }

    void waitval ( int v )  // wait for a value
    {
        while ( value != v )
            wait ( ev ) ;
    }
};

// bus interface
interface iBus
{
	unsigned bit[31:0] read();
	void write(unsigned bit[31:0] data);
};

channel cBus()
	implements iBus
{

	unsigned bit[31:0] data;
	cSignal ready;
	cSignal ack;

	// read bus
	unsigned bit[31:0] read() {
		unsigned bit[31:0] rdata;

		ready.waitval(1);	// wait for ready signal
		rdata = data;		// sample data from bus 
		ack.assign(1);		// assert ack signal
		ready.waitval(0);
		ack.assign(0);		
		return data;
	}

	// write bus
	void write(unsigned bit[31:0] wdata) {
		ready.assign(1);	// assert ready signal
		data = wdata;		// load data into bus
		ack.waitval(1);		// wait for ack signal
		ready.assign(0);	// deassert ready signal
		ack.waitval(0);
	}
};
