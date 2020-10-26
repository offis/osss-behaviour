/************	JPEG encoder bus interfaces 	*******/
/************	bus.sc		*******/
/************	A. Gerstlauer	*******/
/************	06/06/2001	*******/

#include "const.sh"
#include "bus.sh"

import "global";


/**************************************************************************
**		SW bus master driver
**************************************************************************/


/* ------ Protocol Layer ------ */

interface IProtocolMaster
{
  bit[31:0] read(bit[31:0] addr);
  void write(bit[31:0] addr, bit[31:0] data);
};


channel CSWProtocol(out signal bit[31:0] MWDATA,
		    in  signal bit[31:0] MRDATA,
		    out signal bit[1]    MTSB,
		    in  signal bit[1]    MTAB,
		    out signal bit[1]    MWDATAOE,
		    out signal bit[31:0] MADDR,
		    out signal bit[1]    MRWB
		    )
  implements IProtocolMaster
{
  bit[31:0] read(bit[31:0] addr)
  {
    bit[31:0] data;
    bit[1] val;
	
    waitfor(6);
    
    // first clock cycle CR0
    MADDR = addr;	//  assign address lines
    MRWB = 1;		//  assert read control
    MTSB = 0;		//  assert the start of bus transfer
    
    waitfor(6);
    
    // second clock cycle CR1
    val = MTAB;		//  sample acknowledge line
    while(val != 0)
    {
      MADDR = addr;		//  assign address lines
      MTSB = 1;			//  deassert the start of bus transfer
      MRWB = 1;			//  assert read control
      waitfor(6);
      val = MTAB;		//  sample acknowledge line
    }
	
    data = MRDATA;		//  sample data bus 
    waitfor(6);

    return data;
  }

  void write(bit[31:0] addr, bit[31:0] data)
  {
    bit[1] val;

    waitfor(6);
    
    // first clock cycle CW0
    MADDR = addr; 		//  assign address lines
    MRWB = 0;		//  assert write control
    MWDATAOE = 0; 	//  deassert write data available
    MTSB = 0;		//  assert the start of bus transfer
    
    waitfor(6);

    // second clock cycle CW1
    val = MTAB;		//  sample acknowledge line
    while(val != 0)
    {
      MADDR = addr;  		//  assign address lines
      MTSB = 1;		//  deassert the start of bus transfer
      MRWB = 0;		//  assert write control
      MWDATAOE = 1;	//  assert write data available
      MWDATA = data; 		//  drive data outputs
      waitfor(6); 
      val = MTAB; 	// sample acknowledge line
    }
    
    waitfor(6);
  }
};


/* ------ Application Layer ------ */


interface IBusMaster
{
  void sendInt(int data, int addr);
  void sendBlock(int data[BLOCKSIZE], int addr);

  void recvInt(int* data, int addr);
  void recvBlock(int data[BLOCKSIZE], int addr);
};


channel CSWBus(out signal bit[31:0] MWDATA,
	       in  signal bit[31:0] MRDATA,
               out signal bit[1]    MTSB,
               in  signal bit[1]    MTAB,
               out signal bit[1]    MWDATAOE,
               out signal bit[31:0] MADDR,
               out signal bit[1]    MRWB,
               in  signal bit[1]    INTC
               )
  implements IBusMaster
{	
  CSWProtocol protocol(MWDATA, MRDATA, MTSB, MTAB, MWDATAOE, MADDR, MRWB);
	
  void sendInt(int data, int addr)
  {
    // data transfer
    protocol.write(addr, data);
  }

  void recvInt(int *data, int addr)
  {
    // high-level synchronization, wait for ready signal
    while(INTC)
      wait(falling INTC);
    
    // data transfer
    *data = protocol.read(addr);
  }
	
  void sendBlock(int data[BLOCKSIZE], int addr)
  {
    int i;
    
    // data transfer
    for (i = 0; i < BLOCKSIZE; i++)
      protocol.write(addr, data[i]);
  }

  void recvBlock(int data[BLOCKSIZE], int addr)
  {
    int i;
    
    // high-level synchronization, wait for ready signal
    while(INTC)
      wait(falling INTC);
		
    // data transfer
    for (i = 0; i < BLOCKSIZE; i++)
      data[i] = protocol.read(addr);
  }
};


/**************************************************************************
**		HW bus slave interface
**************************************************************************/


/* ------ Protocol Layer ------ */


interface IProtocolSlave
{
  bit[31:0] read(bit[31:0] addr);
  void write(bit[31:0] addr, bit[31:0] data);
};


channel CHWProtocol(    signal bit[31:0] DB,
		    in  signal bit[1]    DBOE, 
		    in  signal bit[1]    TSB,
		    out signal bit[1]    TAB,
		    in  signal bit[31:0] MADDR,
		    in  signal bit[1]    MRWB
		    )
  implements IProtocolSlave
{
  bit[31:0] read(bit[31:0] addr)
  {
    bit[31:0] Reg_In;

    // DW0, idle state, waiting for new bus transfer
    TAB = 1;
    wait (falling TSB);
    
    // exception
    if(addr != MADDR)
    {
      error("Unexpected address on bus!", 0);
    }
    waitfor(4);
    
    // DW1,
    while(!DBOE)
      wait(rising DBOE);
    Reg_In = DB;
    TAB = 1;
    waitfor(4);
    
    // DW2,
    TAB = 0;
    waitfor(4);
    return Reg_In;
  }
  
  void write(bit[31:0] addr, bit[31:0] data)
  {
    bit[31:0] Reg_Out;
    
    // DR0, idle state, waiting for new bus transfer
    TAB = 1;
    wait (falling TSB);
    
    // exception
    if(addr != MADDR)
    {
      error("Unexpected address on bus!", 0);
    }
    waitfor(4);

    // DR1, 
    Reg_Out = data;
    TAB = 1;
    waitfor(4);

    // DR2
    DB = Reg_Out;
    TAB = 0;
    waitfor(4);
    return;
  }
};


/* ------ Application Layer ------ */


interface IBusSlave
{
  void sendBlock(int data[BLOCKSIZE], int addr);
  void recvInt(int *data, int addr);
  void recvBlock(int* data, int addr);
};


channel CHWBus(    signal bit[31:0] DB,
	       in  signal bit[1]    DBOE, 
               in  signal bit[1]    TSB,
               out signal bit[1]    TAB,
               in  signal bit[31:0] MADDR,
               in  signal bit[1]    MRWB,
               out signal bit[1]    INTC
               )
  implements IBusSlave
{
  CHWProtocol protocol(DB, DBOE, TSB, TAB, MADDR, MRWB);
	
  void recvInt(int *data, int addr)
  {
    // data transfer
    *data = protocol.read(addr);
  }
	
  void sendBlock(int data[BLOCKSIZE], int addr)
  {
    int i;
    
    // high-level synchronization, wait for ready signal
    INTC = 0;
    
    // data transfer
    for(i = 0; i < BLOCKSIZE; i++)
      protocol.write(addr, data[i]);
    
    INTC = 1;
  }

  void recvBlock(int *data, int addr)
  {
    int i;
    
    // data transfer
    for(i = 0; i < BLOCKSIZE; i++)
      data[i] = protocol.read(addr);
  }
};

