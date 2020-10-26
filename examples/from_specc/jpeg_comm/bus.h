#ifndef BUS_INCLUDED
#define BUS_INCLUDED

/************	JPEG encoder bus interfaces 	*******/
/************	bus.sc		*******/
/************	A. Gerstlauer	*******/
/************	06/06/2001	*******/

#include "const.h"
#include "global.h"

using sc_dt::sc_uint;

// Bus addressing

#define DCT_DATAIN	0x30
#define DCT_DATAOUT	0x40

/**************************************************************************
**		SW bus master driver
**************************************************************************/


/* ------ Protocol Layer ------ */

class IProtocolMaster : public virtual osss::osss_interface
{
 public:
  virtual sc_uint<32> read(sc_uint<32> addr) = 0;
  virtual void write(sc_uint<32> addr, sc_uint<32> data) = 0;
};


class CSWProtocol : public IProtocolMaster
{
 public:
  sc_core::sc_out<sc_uint<32> > MWDATA;
  sc_core::sc_in<sc_uint<32> >  MRDATA;
  sc_core::sc_out<bool>         MTSB;
  sc_core::sc_in<bool>          MTAB;
  sc_core::sc_out<bool>         MWDATAOE;
  sc_core::sc_out<sc_uint<32> > MADDR;
  sc_core::sc_out<bool>         MRWB;
  
  sc_uint<32> read(sc_uint<32> addr)
  {
    sc_uint<32>   data;
    bool val;
    osss::wait(6);
       
    // first clock cycle CR0
    MADDR = addr;	//  assign address lines
    MRWB = 1;		//  assert read control
    MTSB = 0;		//  assert the start of bus transfer
    osss::wait(6);
       
    // second clock cycle CR1
    val = MTAB;		//  sample acknowledge line
    while(val != 0)
    {
      MADDR = addr;		//  assign address lines
      MTSB = 1;			//  deassert the start of bus transfer
      MRWB = 1;			//  assert read control
      osss::wait(6);
    
      val = MTAB;		//  sample acknowledge line
    }
	
    data = MRDATA;		//  sample data bus 
    osss::wait(6);
   
    return data;
  }

  void write(sc_uint<32> addr, sc_uint<32> data)
  {
    bool val;
    osss::wait(6);
         
    // first clock cycle CW0
    MADDR = addr; 	//  assign address lines
    MRWB = 0;		//  assert write control
    MWDATAOE = 0; 	//  deassert write data available
    MTSB = 0;		//  assert the start of bus transfer
    osss::wait(6);
   
    // second clock cycle CW1
    val = MTAB;		//  sample acknowledge line
    
    while(val != 0)
    {
      MADDR = addr;  	//  assign address lines
      MTSB = 1;		//  deassert the start of bus transfer
      MRWB = 0;		//  assert write control
      MWDATAOE = 1;	//  assert write data available
      MWDATA = data; 	//  drive data outputs
      osss::wait(6);
    
      val = MTAB; 	// sample acknowledge line
    }
    osss::wait(6);
  }
};


/* ------ Application Layer ------ */


class IBusMaster : public virtual osss::osss_interface
{
 public:
  virtual void sendInt(int data, int addr) = 0;
  virtual void sendBlock(const osss::osss_array<int, BLOCKSIZE>& data, 
                         int addr) = 0;

  virtual void recvInt(int& data, int addr) = 0;
  virtual void recvBlock(osss::osss_array<int, BLOCKSIZE>& data, 
                         int addr) = 0;
};


class CSWBus : 
  public IBusMaster, 
  public osss::osss_prim_channel
{
 public:
  sc_core::sc_out<sc_uint<32> > MWDATA;
  sc_core::sc_in<sc_uint<32> >  MRDATA;
  sc_core::sc_out<bool>         MTSB;
  sc_core::sc_in<bool>          MTAB;
  sc_core::sc_out<bool>         MWDATAOE;
  sc_core::sc_out<sc_uint<32> > MADDR;
  sc_core::sc_out<bool>         MRWB;
  sc_core::sc_in<bool>          INTC;
    
 protected:
  CSWProtocol protocol;

 public:
  CSWBus() : osss::osss_prim_channel("CSWBus") 
  {
    protocol.MWDATA(MWDATA);
    protocol.MRDATA(MRDATA);
    protocol.MTSB(MTSB);
    protocol.MTAB(MTAB);
    protocol.MWDATAOE(MWDATAOE);
    protocol.MADDR(MADDR);
    protocol.MRWB(MRWB);
  }
  
  CSWBus(const char* name) : osss::osss_prim_channel(name) 
  {
    protocol.MWDATA(MWDATA);
    protocol.MRDATA(MRDATA);
    protocol.MTSB(MTSB);
    protocol.MTAB(MTAB);
    protocol.MWDATAOE(MWDATAOE);
    protocol.MADDR(MADDR);
    protocol.MRWB(MRWB);
  }

  void sendInt(int data, int addr)
  {
    // data transfer
    protocol.write(addr, data);
  }

  void recvInt(int& data, int addr)
  {
    // high-level synchronization, wait for ready signal
    wait(INTC.negedge_event());
    
    // data transfer
    data = protocol.read(addr);
  }
	
  void sendBlock(const osss::osss_array<int, BLOCKSIZE>& data, int addr)
  {
    int i;
    
    // data transfer
    for (i = 0; i < BLOCKSIZE; i++)
      protocol.write(addr, data[i]);
  }

  void recvBlock(osss::osss_array<int, BLOCKSIZE>& data, int addr)
  {
    int i;
    
    // high-level synchronization, wait for ready signal
    wait(INTC.negedge_event());

    // data transfer
    for (i = 0; i < BLOCKSIZE; i++)
      data[i] = protocol.read(addr);
  }
};


/**************************************************************************
**		HW bus slave interface
**************************************************************************/


/* ------ Protocol Layer ------ */


class IProtocolSlave : public virtual osss::osss_interface
{
 public:
  virtual sc_uint<32> read(sc_uint<32> addr) = 0;
  virtual void write(sc_uint<32> addr, sc_uint<32> data) = 0;
};


class CHWProtocol : public IProtocolSlave
{
 public:
  sc_core::sc_inout<sc_uint<32> > DB;
  sc_core::sc_in<bool>            DBOE;
  sc_core::sc_in<bool>            TSB;
  sc_core::sc_out<bool>           TAB;
  sc_core::sc_in<sc_uint<32> >    MADDR;
  sc_core::sc_in<bool>            MRWB;
  
  sc_uint<32> read(sc_uint<32> addr)
  {
    sc_uint<32> Reg_In;

    // DW0, idle state, waiting for new bus transfer
    TAB = 1;
    wait (TSB.negedge_event());
   
    // exception
    if(addr != MADDR)
    {
      error("Unexpected address on bus!", 0);
    }

    osss::wait(4);
      
    // DW1,
    wait(DBOE.posedge_event());

    Reg_In = DB;
    TAB = 1;
    osss::wait(4);
      
    // DW2,
    TAB = 0;
    osss::wait(4);
  
    return Reg_In;
  }
  
  void write(sc_uint<32> addr, sc_uint<32> data)
  {
    sc_uint<32> Reg_Out;
    
    // DR0, idle state, waiting for new bus transfer
    TAB = 1;
    wait (TSB.negedge_event());
    
    // exception
    if(addr != MADDR)
    {
      error("Unexpected address on bus!", 0);
    }
    
    osss::wait(4);
  
    // DR1, 
    Reg_Out = data;
    TAB = 1;
    osss::wait(4);
  
    // DR2
    DB = Reg_Out;
    TAB = 0;
    osss::wait(4);
  }
};


/* ------ Application Layer ------ */


class IBusSlave : public virtual osss::osss_interface
{
 public:
  virtual void sendBlock(const osss::osss_array<int, BLOCKSIZE>& data, 
                         int addr) = 0;

  virtual void recvInt(int& data, int addr) = 0;

  virtual void recvBlock(osss::osss_array<int, BLOCKSIZE>& data, 
                         int addr) = 0;
};


class CHWBus : 
  public IBusSlave,
  public osss::osss_prim_channel
{
 public:
  sc_core::sc_inout<sc_uint<32> > DB;
  sc_core::sc_in<bool>            DBOE;
  sc_core::sc_in<bool>            TSB;
  sc_core::sc_out<bool>           TAB;
  sc_core::sc_in<sc_uint<32> >    MADDR;
  sc_core::sc_in<bool>            MRWB;
  sc_core::sc_out<bool>           INTC;
  
 protected:
  CHWProtocol protocol;
	
 public:
  CHWBus() : osss::osss_prim_channel("CHWBus")
  {
    protocol.DB(DB);
    protocol.DBOE(DBOE);
    protocol.TSB(TSB);
    protocol.TAB(TAB);
    protocol.MADDR(MADDR);
    protocol.MRWB(MRWB);
  }

  CHWBus(const char* name) : osss::osss_prim_channel(name)
  {
    protocol.DB(DB);
    protocol.DBOE(DBOE);
    protocol.TSB(TSB);
    protocol.TAB(TAB);
    protocol.MADDR(MADDR);
    protocol.MRWB(MRWB);
  }

  void recvInt(int& data, int addr)
  {
    // data transfer
    data = protocol.read(addr);
  }
	
  void sendBlock(const osss::osss_array<int, BLOCKSIZE>& data, int addr)
  {
    int i;
    
    // high-level synchronization, wait for ready signal
    INTC = 0;
    
    // data transfer
    for(i = 0; i < BLOCKSIZE; i++)
      protocol.write(addr, data[i]);
    
    INTC = 1;
  }

  void recvBlock(osss::osss_array<int, BLOCKSIZE>& data, int addr)
  {
    int i;
    
    // data transfer
    for(i = 0; i < BLOCKSIZE; i++)
      data[i] = protocol.read(addr);
  }
};

#endif
