#ifndef ENCODE_INCLUDED
#define ENCODE_INCLUDED

/************	JPEG encode 	*******/
/************	encode.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/06/2001	*******/

#include "const.h"

#include "bus.h"
#include "handle.h"
#include "dct.h"
#include "quant.h"
#include "huff.h"


OSSS_BEHAVIOUR(BHDataSend)
{
  osss::osss_in<osss::osss_array<int, BLOCKSIZE> > HData;
  osss::osss_port<IBusMaster> bus;

  BEHAVIOUR_CTOR(BHDataSend) {}

  void main()
  {
    bus->sendBlock(HData.read(), DCT_DATAIN);
  }
};


OSSS_BEHAVIOUR(BDDataRecv)
{
  osss::osss_port<IBusMaster> bus;
  osss::osss_out<osss::osss_array<int, BLOCKSIZE> > DData;

  BEHAVIOUR_CTOR(BDDataRecv) {}

  void main()
  {
    osss::osss_array<int, BLOCKSIZE> tmp_data;
    bus->recvBlock(tmp_data, DCT_DATAOUT);
    DData = tmp_data;
  }
};


OSSS_BEHAVIOUR(JpegEncodeStripe)
{
  osss::osss_in<int> mduHigh;
  osss::osss_in<unsigned char*> stripe;
  osss::osss_in<int> MDUWide;
  osss::osss_in<EHuff> DCEhuff;
  osss::osss_in<EHuff> ACEhuff;
  osss::osss_port<IBusMaster> bus;
  osss::osss_port<ISendBits> chData;

 protected:
  osss::osss_shared_variable<int> mduWide;
  
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE> > hdata;
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE> > ddata;
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE> > qdata;
  
  HandleData	handleData;
  BHDataSend    sendHData;
  BDDataRecv    recvDData;
  Quantization	quantization;
  HuffmanEncode	huffmanEncode;

 public:
  BEHAVIOUR_CTOR(JpegEncodeStripe) :
    mduWide("mduWide"),
    hdata("hdata"),
    ddata("ddata"),
    qdata("qdata"),
    handleData("handleData"),
    sendHData("sendHData"),
    recvDData("recvDData"),
    quantization("quantization"),
    huffmanEncode("huffmanEncode")
  {
    handleData.mduWide(mduWide);
    handleData.mduHigh(mduHigh);
    handleData.stripe(stripe);
    handleData.MDUWide(MDUWide);
    handleData.HData(hdata);

    sendHData.HData(hdata);
    sendHData.bus(bus);
    
    recvDData.bus(bus);
    recvDData.DData(ddata);

    quantization.DData(ddata);
    quantization.QData(qdata);

    huffmanEncode.QData(qdata);
    huffmanEncode.DCEhuff(DCEhuff);
    huffmanEncode.ACEhuff(ACEhuff);
    huffmanEncode.chData(chData);

    osss_seq( handleData, sendHData, recvDData, quantization, huffmanEncode );
  }

  void main() 
  {
#ifdef PIPED_SW    
    mduWide = 0;
    
    handleData.main();
    sendHData.main();

    // Run software in parallel with hardware in pipelined fashion
    for(mduWide.write(1); 
        mduWide.read() < MDUWide; 
        mduWide.write(mduWide.read() + 1))
    {
      handleData.main();
      recvDData.main();
      sendHData.main();
      quantization.main();
      huffmanEncode.main();
    };
    
    recvDData.main();
    quantization.main();
    huffmanEncode.main();
  }
#else
    for(mduWide.write(0); 
        mduWide.read() < MDUWide; 
        mduWide.write(mduWide.read() + 1))
    {
      handleData.main();
      sendHData.main();
      recvDData.main();
      quantization.main();
      huffmanEncode.main();
    }
  }
#endif  
};


OSSS_BEHAVIOUR(JpegEncode)
{
  osss::osss_port<osss::osss_receiver_if<char> > chPixel;
  osss::osss_in<int> ImageWidth;
  osss::osss_in<int> ImageHeight;
  osss::osss_in<EHuff> DCEhuff;
  osss::osss_in<EHuff> ACEhuff;
  osss::osss_port<IBusMaster> bus;
  osss::osss_port<ISendBits> chData;

protected:
  osss::osss_shared_variable<int> mduHigh;
  osss::osss_shared_variable<int> MDUWide;
  int MDUHigh;
  osss::osss_shared_variable<unsigned char*> stripe;
  
  ReceiveData receiveData;
  JpegEncodeStripe encodeStripe;

public:
  BEHAVIOUR_CTOR(JpegEncode) :
    mduHigh("mduHigh"),
    MDUWide("MDUWide"),
    stripe("stripe"),
    receiveData("receiveData"),
    encodeStripe("encodeStripe")
  {
    receiveData.chPixel(chPixel);
    receiveData.mduHigh(mduHigh);
    receiveData.ImageWidth(ImageWidth);
    receiveData.ImageHeight(ImageHeight);
    receiveData.MDUWide(MDUWide);
    receiveData.stripe(stripe);

    encodeStripe.mduHigh(mduHigh);
    encodeStripe.stripe(stripe);
    encodeStripe.MDUWide(MDUWide);
    encodeStripe.DCEhuff(DCEhuff);
    encodeStripe.ACEhuff(ACEhuff); 
    encodeStripe.bus(bus);
    encodeStripe.chData(chData);

    osss_seq( receiveData, encodeStripe );
  }
  
  void main() 
  {
    for(mduHigh.write(0), MDUHigh = (ImageHeight+7)>>3; 
        mduHigh.read() < MDUHigh; 
        mduHigh.write(mduHigh.read() + 1))
    {
      receiveData.main();
      encodeStripe.main();
    }
  }
};

#endif
