#include <stdio.h>
#include <stdlib.h>

// blocking, unbuffered message passing
interface iRecv
{
    unsigned int recv();
};

interface iSend
{
	void send(unsigned int v);
};

channel ChMP()
    implements iSend, iRecv
{
    unsigned int data;
    event eReady, eAck;
	bool ready_flag= false;
	bool ack_flag = false;

	void send(unsigned int v)
	{
		data = v;

		ready_flag = true;
		notify(eReady);

		while(!ack_flag)
			wait(eAck);

		ready_flag = false;
		notify(eReady);

		while (ack_flag)
			wait(eAck);
	}

	unsigned int recv()
	{
		unsigned int rdata;

		while (!ready_flag)
			wait(eReady);

		rdata = data;
		ack_flag = true;
		notify(eAck);

		while(ready_flag)
			wait(eReady);

		ack_flag = false;
		notify(eAck);

		return rdata;
	}
};
