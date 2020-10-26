#ifndef HW_INCLUDED
#define HW_INCLUDED

/************	JPEG encoder Hardware 	*******/
/************	hw.sc		*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/

#include "const.h"
#include "dct.h"

OSSS_BEHAVIOUR(BHDataRecv)
{
  osss::osss_port<osss::osss_receiver_if<block> >   chHData;
  osss::osss_out<osss::osss_array<int, BLOCKSIZE> > HData;

  BEHAVIOUR_CTOR(BHDataRecv) {}
 
  void main()
  {
    osss::osss_array<int, BLOCKSIZE> tmp_data;
    chHData->receive(tmp_data);
    HData = tmp_data;
  }
};


OSSS_BEHAVIOUR(BDDataSend)
{
  osss::osss_in<osss::osss_array<int, BLOCKSIZE> > DData;
  osss::osss_port<osss::osss_sender_if<block> >    chDData;

  BEHAVIOUR_CTOR(BDDataSend) {}

  void main()
  {
    chDData->send(DData);
  }
};

#ifdef PIPED_HW  

// Hardware unit with separate, concurrent FSMDs for sending and receiving
OSSS_BEHAVIOUR(HW)
{
  osss::osss_port<osss::osss_receiver_if<block> > chHData;
  osss::osss_port<osss::osss_sender_if<block> >   chDData;

 protected:
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE>, 1> HData;
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE>, 1> DData;
  
  BHDataRecv recvHData;
  DCT	     dct;
  BDDataSend sendDData;

 public:
  BEHAVIOUR_CTOR(HW) :
    HData("HData"),
    DData("DData"),
    recvHData("recvHData"),
    dct("dct"),
    sendDData("sendDData")
  {
    recvHData.chDData(chHData);
    recvHData.HData(HData);

    dct.HData(HData);
    dct.DData(DData);

    sendDData.DData(DData);
    sendDData.chDData(chDData);

    osss_pipe(
      recvHData >> // receive hdata output
      dct >>       // original behavior
      sendDData    // send ddata output
    );
  }
};

#else

OSSS_PIPELINE_STAGE(DctHW)
{
  osss::osss_port<osss::osss_receiver_if<block> > chHData;
  osss::osss_port<osss::osss_sender_if<block> >   chDData;

 protected:
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE> > HData;
  osss::osss_shared_variable<osss::osss_array<int, BLOCKSIZE> > DData;
  
  BHDataRecv recvHData;
  DCT	     dct;
  BDDataSend sendDData;

 public:
  PIPELINE_STAGE_CTOR(DctHW) :
    HData("HData"),
    DData("DData"),
    recvHData("recvHData"),
    dct("dct"),
    sendDData("sendDData")
  {
    recvHData.chHData(chHData);
    recvHData.HData(HData);

    dct.HData(HData);
    dct.DData(DData);

    sendDData.DData(DData);
    sendDData.chDData(chDData);

    osss_seq(
      recvHData, // receive hdata output
      dct,       // original behavior
      sendDData  // send ddata output
    );
  }
};


OSSS_BEHAVIOUR(HW)
{
  osss::osss_port<osss::osss_receiver_if<block> > chHData;
  osss::osss_port<osss::osss_sender_if<block> >   chDData;

 protected:
  DctHW dctHW;
  
 public:
  BEHAVIOUR_CTOR(HW) :
    dctHW("dctHW")
  {
    dctHW.chHData(chHData);
    dctHW.chDData(chDData);

    osss_pipe( // endless loop
      dctHW
    );
  }
};

#endif

#endif
