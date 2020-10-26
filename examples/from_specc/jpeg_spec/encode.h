#ifndef ENCODE_INCLUDED
#define ENCODE_INCLUDED

/************	JPEG encode 	*******/
/************	encode.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/06/2001	*******/

#define PIPELINED_OUTER
#define PIPELINED_INNER

#include "const.h"
#include "handle.h"
#include "dct.h"
#include "quant.h"
#include "huff.h"

#ifdef PIPELINED_OUTER
OSSS_PIPELINE_STAGE(JpegEncodeStripe)
#else
OSSS_BEHAVIOUR(JpegEncodeStripe)
#endif
{
 public:
  osss::osss_in<int> mduHigh;
  osss::osss_in<unsigned char*> stripe; 
  osss::osss_in<int> MDUWide;
  osss::osss_in<EHuff> DCEhuff; 
  osss::osss_in<EHuff> ACEhuff;

  osss::osss_port<ISendBits> chData;

 protected:
  osss::osss_shared_variable<int> mduWide;
  
#ifdef PIPELINED_INNER
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE>, 1> hdata;
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE>, 1> ddata;
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE>, 1> qdata;
#else
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE> > hdata;
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE> > ddata;
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE> > qdata;
#endif
  
  HandleData	handleData;
  DCT		dct;
  Quantization	quantization;
  HuffmanEncode	huffmanEncode;

 public:
#ifdef PIPELINED_OUTER
  PIPELINE_STAGE_CTOR(JpegEncodeStripe) :
#else
  BEHAVIOUR_CTOR(JpegEncodeStripe) :
#endif
    mduWide("mduWide"),
    hdata("hdata"),
    ddata("ddata"),
    qdata("qdata"),
    handleData("handleData"),
    dct("dct"),
    quantization("quantization"),
    huffmanEncode("huffmanEncode")
  {
    handleData.mduWide(mduWide); 
    handleData.mduHigh(mduHigh);
    handleData.stripe(stripe);
    handleData.MDUWide(MDUWide);
    handleData.HData(hdata);

    dct.HData(hdata);
    dct.DData(ddata);
    
    quantization.DData(ddata);
    quantization.QData(qdata);

    huffmanEncode.QData(qdata);
    huffmanEncode.DCEhuff(DCEhuff);
    huffmanEncode.ACEhuff(ACEhuff);
    huffmanEncode.chData(chData);

#ifdef PIPELINED_INNER
    osss_pipe( handleData >> dct >> quantization >> huffmanEncode );
#else
    osss_seq( handleData, dct, quantization, huffmanEncode );
#endif
  }

#ifndef PIPELINED_INNER
  virtual void main()
  {
    for(mduWide.write(0); 
        mduWide.read() < MDUWide.read(); 
        mduWide.write(mduWide.read() + 1))
    {
      handleData.main();
      dct.main();
      quantization.main();
      huffmanEncode.main();
    }
  }
#else
  virtual void init()
  {
    mduWide.write(0);
  }

  virtual void pipe_post()
  {
    mduWide.write(mduWide.read() + 1);
  }

  virtual bool pipe_stop_condition() const
  { 
    return (mduWide.read() == MDUWide - 1);
  }
#endif

};


OSSS_BEHAVIOUR(JpegEncode)
{
 public:
  osss::osss_port<osss::osss_receiver_if<char> > chPixel;

  osss::osss_in<int>   ImageWidth, ImageHeight;
  osss::osss_in<EHuff> DCEhuff, ACEhuff;

  osss::osss_port<ISendBits> chData;

 protected:

  osss::osss_shared_variable<int> mduHigh;
  osss::osss_shared_variable<int> MDUHigh;

#ifdef PIPELINED_OUTER  
  osss::osss_shared_variable<int, 1> MDUWide;
  osss::osss_shared_variable<unsigned char*, 1> stripe;
#else
  osss::osss_shared_variable<int> MDUWide;
  osss::osss_shared_variable<unsigned char*> stripe;
#endif
    
  ReceiveData      receiveData;
  JpegEncodeStripe encodeStripe;

 public:
  BEHAVIOUR_CTOR(JpegEncode) :
    mduHigh("mduHigh"),
    MDUHigh("MDUHigh"),
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
    encodeStripe.chData(chData);
    
#ifdef PIPELINED_OUTER
    osss_pipe( receiveData >> encodeStripe );
#else
    osss_seq( receiveData, encodeStripe );
#endif
  }

#ifndef PIPELINED_OUTER
  virtual void main() 
  {
    for(mduHigh.write(0), MDUHigh.write((ImageHeight+7)>>3); 
        mduHigh.read() < MDUHigh; 
        mduHigh.write(mduHigh.read() + 1))
    {
      receiveData.main();
      encodeStripe.main();
    }
  }
#else
  virtual void init()
  {
    mduHigh.write(0); 
    MDUHigh.write((ImageHeight+7)>>3);
  }

  virtual void pipe_post()
  {
    mduHigh.write(mduHigh.read() + 1);
  }

  virtual bool pipe_stop_condition() const
  { 
    return (mduHigh.read() == MDUHigh - 1);
  }
#endif

};

#endif
