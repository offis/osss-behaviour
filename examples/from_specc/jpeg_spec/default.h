#ifndef DEFAULT_INCLUDED
#define DEFAULT_INCLUDED

/************	Fill default huffman tables of JPEG encoder 	*******/
/************	default.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/06/2001	*******/

#include "const.h"
#include "global.h"


/* Default huffman table */
static const int LuminanceDCBits[16] = {
0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const int LuminanceDCValues[162] = {
0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
0x08, 0x09, 0x0a, 0x0b, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00};

static const int LuminanceACBits[16] = {
0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03,
0x05, 0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7d};

static const int LuminanceACValues[162] = {
0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,
0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
0xf9, 0xfa};


OSSS_BEHAVIOUR(InitTable)
{
 public:
  osss::osss_out<XHuff> xhuff;
  osss::osss_out<XHuff> xhuff_out;

 protected:
  const int* bts;
  const int* hvls;

 public:

  InitTable(osss::osss_behaviour_name name,
            const int* bts_in,
            const int* hvls_in) 
  : osss::osss_behaviour(name),
    bts(bts_in),
    hvls(hvls_in)
  {}

  void main()
  {
    int i;
    int accum;

    XHuff tmp_xhuff;
 
    for(accum = 0, i = 0; i < 16; i++)
    {
      accum += bts[i];
      tmp_xhuff.bits[i+1] = bts[i];	/* Shift offset for internal specs.*/
    }
    for(i = 0; i < accum; i++)
    {
      tmp_xhuff.huffval[i] = hvls[i];
    }

    // write to osss_out port
    xhuff     = tmp_xhuff;
    xhuff_out = tmp_xhuff;
  }
};


OSSS_BEHAVIOUR(SizeTable)
{
  osss::osss_in<XHuff> xhuff;
  osss::osss_out<osss::osss_array<int, 257> > huffsize;
  osss::osss_out<int> lastp;

  BEHAVIOUR_CTOR(SizeTable) {}

  void main()
  {
    int i,j,p;
    // read from port
    XHuff tmp_xhuff = xhuff;

    for(p = 0, i = 1; i < 17; i++) {
      for(j = 1; j <= tmp_xhuff.bits[i]; j++) {
	huffsize[p++] = i;
      }
    }
    huffsize[p] = 0;
    lastp = p;
  }
};


OSSS_BEHAVIOUR(CodeTable)
{
  osss::osss_in<osss::osss_array<int, 257> >  huffsize;
  osss::osss_out<osss::osss_array<int, 257> > huffcode;

  BEHAVIOUR_CTOR(CodeTable) {}

  void main()
  {
    int p,code,size;

    p = 0;
    code = 0;
    size = huffsize[0];
    while(1)
    {
      do {
	huffcode[p++] = code++;
      }
      while((huffsize[p]==size)&&(p<257)); /* Overflow Detection */
      if (!huffsize[p]) { /* All finished. */
	break;
      }
      do {                /* Shift next code to expand prefix. */
	code <<= 1;
	size++;
      }
      while(huffsize[p] != size);
    }
  }
};


OSSS_BEHAVIOUR(OrderCodes)
{
  osss::osss_in<int> lastp; 
  osss::osss_in<XHuff> xhuff; 
  osss::osss_out<EHuff> ehuff;
  osss::osss_in<osss::osss_array<int, 257> > huffsize;
  osss::osss_in<osss::osss_array<int, 257> > huffcode;

  BEHAVIOUR_CTOR(OrderCodes) {}

  void main()
  {
    int index, p;
    EHuff tmp_ehuff;
    
    // read from port
    XHuff tmp_xhuff = xhuff;

    for(p = 0; p < lastp; p++)
    {
      index = tmp_xhuff.huffval[p];
      tmp_ehuff.ehufco[index] = huffcode[p];
      tmp_ehuff.ehufsi[index] = huffsize[p];
    }

    // write to port
    ehuff = tmp_ehuff;
  }
};

  
OSSS_BEHAVIOUR(SpecifiedHuffman)
{
 public:
  osss::osss_out<XHuff> Xhuff;
  osss::osss_out<EHuff> Ehuff;

 protected:
  osss::osss_shared_variable<XHuff> Xhuff_sv;
  osss::osss_shared_variable<osss::osss_array<int, 257> > huffcode;
  osss::osss_shared_variable<osss::osss_array<int, 257> > huffsize;
  osss::osss_shared_variable<int> lastp;
  
  InitTable  initTable;
  SizeTable  sizeTable;	
  CodeTable  codeTable;
  OrderCodes orderCodes;

 public:
  SpecifiedHuffman(osss::osss_behaviour_name name ,
                   const int* bts,
                   const int* hvls)
  : osss::osss_behaviour(name),
    Xhuff_sv("Xhuff_sv"),
    huffcode("huffcode"),
    huffsize("huffsize"),
    lastp("lastp"),
    initTable("initTable", bts, hvls),
    sizeTable("sizeTable"),
    codeTable("codeTable"),
    orderCodes("orderCodes")
  {
    initTable.xhuff(Xhuff_sv);
    initTable.xhuff_out(Xhuff);
    
    sizeTable.xhuff(Xhuff_sv);
    sizeTable.huffsize(huffsize);
    sizeTable.lastp(lastp);
	
    codeTable.huffsize(huffsize);
    codeTable.huffcode(huffcode);

    orderCodes.lastp(lastp);
    orderCodes.xhuff(Xhuff_sv);
    orderCodes.ehuff(Ehuff);
    orderCodes.huffsize(huffsize);
    orderCodes.huffcode(huffcode);

    osss_seq( initTable, 
              sizeTable  /*From Xhuff to Ehuff */, 
              codeTable, 
              orderCodes );
  }
  
};

OSSS_BEHAVIOUR(JpegDefaultHuffman)
{
 public:
  osss::osss_out<XHuff> DCXhuff;
  osss::osss_out<EHuff> DCEhuff;
  osss::osss_out<XHuff> ACXhuff;
  osss::osss_out<EHuff> ACEhuff;
  
 protected:
  SpecifiedHuffman sh1;
  SpecifiedHuffman sh2;

 public:
  BEHAVIOUR_CTOR(JpegDefaultHuffman) :
    sh1("sh1", LuminanceDCBits, LuminanceDCValues),
    sh2("sh2", LuminanceACBits, LuminanceACValues)
  {
    sh1.Xhuff(DCXhuff);
    sh1.Ehuff(DCEhuff);

    sh2.Xhuff(ACXhuff);
    sh2.Ehuff(ACEhuff);

    osss_par( sh1 | sh2 );
  }
  
};

#endif
