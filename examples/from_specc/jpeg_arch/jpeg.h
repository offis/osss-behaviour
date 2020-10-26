#ifndef JPEG_INCLUDED
#define JPEG_INCLUDED

/************	JPEG encode 	*******/
/************	jpeg.sc		*******/
/************	A. Gerstlauer	*******/
/************	06/06/2001	*******/

#include "const.h"

#include "default.h"
#include "header.h"
#include "encode.h"

OSSS_BEHAVIOUR(JpegStart)
{
 public:
  osss::osss_port<osss::osss_receiver_if<int> > chHeader;

  osss::osss_out<int> ImageWidth,     ImageHeight;
  osss::osss_out<int> ImageWidth_out, ImageHeight_out;
  
  BEHAVIOUR_CTOR(JpegStart) {}

  void main()
  {
    int width, height;
    
#ifdef JPEG_DEBUG	
    msg("HandleData: Receiving header with image size...\n");
#endif
    
    chHeader->receive(width);
    chHeader->receive(height);
    ImageWidth      = width;
    ImageWidth_out  = width;
    ImageHeight     = height;    
    ImageHeight_out = height;

#ifdef JPEG_DEBUG	
    msg("HandleData: Got header width=%d, height=%d\n", width, height);
#endif
  
#if defined(JPEG_VERBOSE) || defined(JPEG_DEBUG)
    std::cout << "****************************************************" 
              << std::endl;
    std::cout << "Time " << sc_core::sc_time_stamp() 
              << " (" << sc_core::sc_delta_count() << "): "
              << "JPEG Encoder Begin..." 
              << std::endl;
    std::cout << "****************************************************" 
              << std::endl;
#endif
  }
};


OSSS_BEHAVIOUR(JpegInit)
{
 public:
  osss::osss_port<osss::osss_receiver_if<int> > chHeader; 
  osss::osss_port<ISendBits>                    chData;
		  
  osss::osss_out<EHuff> DCEhuff, ACEhuff;
  osss::osss_out<int>   ImageWidth, ImageHeight;

 protected:
  osss::osss_shared_variable<int>   ImageWidth_sv, ImageHeight_sv;
  osss::osss_shared_variable<XHuff> DCXhuff_sv, ACXhuff_sv;

  JpegStart start;
  JpegDefaultHuffman huffman;
  JpegHeader header;

 public:
  BEHAVIOUR_CTOR(JpegInit) :
    ImageWidth_sv("ImageWidth_sv"),
    ImageHeight_sv("ImageHeight_sv"),
    DCXhuff_sv("DCXhuff_sv"),
    ACXhuff_sv("ACXhuff_sv"),
    start("start"),
    huffman("huffman"),
    header("header")
  {
    start.chHeader(chHeader);
    start.ImageWidth(ImageWidth_sv);
    start.ImageHeight(ImageHeight_sv);
    // propagate values to output ports
    start.ImageWidth_out(ImageWidth);
    start.ImageHeight_out(ImageHeight);

    huffman.DCXhuff(DCXhuff_sv);
    huffman.DCEhuff(DCEhuff);
    huffman.ACXhuff(ACXhuff_sv);
    huffman.ACEhuff(ACEhuff);

    header.ImageWidth(ImageWidth_sv);
    header.ImageHeight(ImageHeight_sv);
    header.DCXhuff(DCXhuff_sv);
    header.ACXhuff(ACXhuff_sv);
    header.chData(chData);
   
    osss_seq( start, huffman, header );
  }
  
};


OSSS_BEHAVIOUR(JpegEnd)
{
 public:
  osss::osss_port<ISendBits> chData;

  BEHAVIOUR_CTOR(JpegEnd) {}

  void main()
  {        
    chData->WriteBits(-1, 0);
    chData->WriteMarker(M_EOI);

#if defined(JPEG_VERBOSE) || defined(JPEG_DEBUG)
    std::cout << "****************************************************" 
              << std::endl;
    std::cout << "Time " << sc_core::sc_time_stamp() 
              << " (" << sc_core::sc_delta_count() << "): "
              << "JPEG Encoder End..." 
              << std::endl;
    std::cout << "****************************************************" 
              << std::endl;
#endif

    // we are finished... stop the simulation
    sc_core::sc_stop(); 
  }
};


OSSS_PIPELINE_STAGE(JpegSW)
{
 public:
  osss::osss_port<osss::osss_receiver_if<int> >  chHeader; 
  osss::osss_port<osss::osss_receiver_if<char> > chPixel;

  osss::osss_port<osss::osss_sender_if<block> >   chHData;
  osss::osss_port<osss::osss_receiver_if<block> > chDData;

  osss::osss_port<osss::osss_sender_if<char> >   chData;

 protected:
  osss::osss_shared_variable<int>   imageWidth, imageHeight;  
  osss::osss_shared_variable<EHuff> DCEhuff, ACEhuff;
  
  CBitAdapter bitAdapter;
  
  JpegInit   init;
  JpegEncode encode;
  JpegEnd    end;

 public:
  PIPELINE_STAGE_CTOR(JpegSW) :
    imageWidth("imageWidth"),
    imageHeight("imageHeight"),
    DCEhuff("DCEhuff"),
    ACEhuff("ACEhuff"),
    bitAdapter("bitAdapter"),
    init("init"),
    encode("encode"),
    end("end")
  {
    bitAdapter.chData(chData);

    init.chHeader(chHeader);
    init.chData(bitAdapter); 
    init.DCEhuff(DCEhuff);
    init.ACEhuff(ACEhuff); 
    init.ImageWidth(imageWidth);
    init.ImageHeight(imageHeight);

    encode.chPixel(chPixel);
    encode.ImageWidth(imageWidth);
    encode.ImageHeight(imageHeight);
    encode.DCEhuff(DCEhuff);
    encode.ACEhuff(ACEhuff);
    encode.chHData(chHData);
    encode.chDData(chDData);
    encode.chData(bitAdapter);
  
    end.chData(bitAdapter);

    osss_seq( init, encode, end );
  }
  
};

#endif

