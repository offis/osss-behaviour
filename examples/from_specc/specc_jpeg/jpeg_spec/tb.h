//////////////////////////////////////////////////////////////////////
// C++ header file generated by SCRC V2.1
// Design: tb
// File:   tb.h
// Time:   Sat Mar 22 17:49:30 2008
//////////////////////////////////////////////////////////////////////

#include <specc.h>
#include <longlong.h>
#include <bit.h>

// named user-defined type declarations //////////////////////////////

struct _IO_FILE;
struct _IO_FILE_plus;
struct _IO_jump_t;
struct _IO_marker;
struct __gconv_info;
struct __gconv_loaded_object;
struct __gconv_step;
struct __gconv_step_data;
struct __gconv_trans_data;
struct huffman_encoder;
struct huffman_standard_structure;
struct tagBITMAPFILEHEADER;
struct tagBITMAPINFOHEADER;
struct tagRGBTRIPLE;

// named enumerator type definitions /////////////////////////////////

#line 204 "/usr/include/libio.h"
enum __codecvt_result { __codecvt_ok=0,
    __codecvt_partial=1,
    __codecvt_error=2,
    __codecvt_noconv=3
};

// unnamed enumerator type definitions ///////////////////////////////

#line 39 "/usr/include/gconv.h"
enum { __GCONV_OK=0,
    __GCONV_NOCONV=1,
    __GCONV_NODB=2,
    __GCONV_NOMEM=3,

    __GCONV_EMPTY_INPUT=4,
    __GCONV_FULL_OUTPUT=5,
    __GCONV_ILLEGAL_INPUT=6,
    __GCONV_INCOMPLETE_INPUT=7,

    __GCONV_ILLEGAL_DESCRIPTOR=8,
    __GCONV_INTERNAL_ERROR=9
};

#line 57 "/usr/include/gconv.h"
enum { __GCONV_IS_LAST=1,
    __GCONV_IGNORE_ERRORS=2
};

// struct/union type definitions /////////////////////////////////////

#line 64 "/usr/include/bits/types.h"
typedef struct { long int __val[2]; }
    __quad_t;

#line 268 "/usr/include/libio.h"
struct _IO_FILE { int _flags;




    char *_IO_read_ptr;
    char *_IO_read_end;
    char *_IO_read_base;
    char *_IO_write_base;
    char *_IO_write_ptr;
    char *_IO_write_end;
    char *_IO_buf_base;
    char *_IO_buf_end;

    char *_IO_save_base;
    char *_IO_backup_base;
    char *_IO_save_end;

    struct _IO_marker *_markers;

    struct _IO_FILE *_chain;

    int _fileno;



    int _flags2;

    long int _old_offset;



    unsigned short int _cur_column;
    char _vtable_offset;
    char _shortbuf[1];



    void *_lock;

#line 315 "/usr/include/libio.h"
    __quad_t _offset;

#line 324 "/usr/include/libio.h"
    void *__pad1;
    void *__pad2;
    void *__pad3;
    void *__pad4;
    unsigned int __pad5;

    int _mode;

    char _unused2[40];
};

#line 183 "/usr/include/libio.h"
struct _IO_marker { struct _IO_marker *_next;
    struct _IO_FILE *_sbuf;



    int _pos;
};

#line 78 "/usr/include/wchar.h"
typedef struct { int __count;




    union { unsigned int __wch; char __wchb[4]; } __value; }
    __mbstate_t;

#line 147 "/usr/include/gconv.h"
struct __gconv_step_data { unsigned char *__outbuf;
    unsigned char *__outbufend;



    int __flags;



    int __invocation_counter;



    int __internal_use;

    __mbstate_t *__statep;
    __mbstate_t __state;



    struct __gconv_trans_data *__trans;
};

#line 174 "/usr/include/gconv.h"
struct __gconv_info { unsigned int __nsteps;
    struct __gconv_step *__steps;
    struct __gconv_step_data __data[1];
};

#line 117 "/usr/include/gconv.h"
struct __gconv_step { struct __gconv_loaded_object *__shlib_handle;
    const char *__modname;

    int __counter;

    char *__from_name;
    char *__to_name;

    int (*__fct)(struct __gconv_step *, struct __gconv_step_data *, const unsigned char **, const unsigned char *, unsigned char **, unsigned int *, int, int);
    unsigned int (*__btowc_fct)(struct __gconv_step *, unsigned char);
    int (*__init_fct)(struct __gconv_step *);
    void (*__end_fct)(struct __gconv_step *);



    int __min_needed_from;
    int __max_needed_from;
    int __min_needed_to;
    int __max_needed_to;


    int __stateful;

    void *__data;
};

#line 106 "/usr/include/gconv.h"
struct __gconv_trans_data { int (*__trans_fct)(struct __gconv_step *, struct __gconv_step_data *, void *, const unsigned char *, const unsigned char **, const unsigned char *, unsigned char **, unsigned int *);
    int (*__trans_context_fct)(void *, const unsigned char *, const unsigned char *, unsigned char *, unsigned char *);
    void (*__trans_end_fct)(void *);
    void *__data;
    struct __gconv_trans_data *__next;
};

#line 40 "./global.sc"
struct huffman_encoder { int ehufco[257];
    int ehufsi[257];
};

#line 35 "./global.sc"
struct huffman_standard_structure { int bits[36];
    int huffval[257];
};

#line 26 "./io.sc"
struct tagBITMAPFILEHEADER { short int bfType;
    long int bfSize;
    short int bfReserved1;
    short int bfReserved2;
    long int bfOffBits;
};



struct tagBITMAPINFOHEADER { long int biSize;
    long int biWidth;
    long int biHeight;
    short int biPlanes;
    short int biBitCount;


    long int biCompression;
    long int biSizeImage;
    long int biXPelsPerMeter;
    long int biYPelsPerMeter;
    long int biClrUsed;
    long int biClrImportant;
};


struct tagRGBTRIPLE { char B; char G; char R;
};

#line 100 "/usr/include/stdlib.h"
typedef struct { int quot;
    int rem; }
    div_t;

#line 108 "/usr/include/stdlib.h"
typedef struct { long int quot;
    long int rem; }
    ldiv_t;

#line 68 "/usr/include/bits/types.h"
typedef struct { unsigned long int __val[2]; }
    __u_quad_t;

#line 147 "/usr/include/bits/types.h"
typedef struct { int __val[2]; } __fsid_t;

#line 28 "/usr/include/_G_config.h"
typedef struct { long int __pos;
    __mbstate_t __state; }
    _G_fpos_t;


typedef struct { __quad_t __pos;
    __mbstate_t __state; }
    _G_fpos64_t;

#line 47 "/usr/include/_G_config.h"
typedef union { struct __gconv_info __cd;




    struct { struct __gconv_info __cd; struct __gconv_step_data __data; } __combined; }
    _G_iconv_t;

// type synonym definitions //////////////////////////////////////////

#line 21 "./chann.sc"
typedef char byte;

#line 43 "./chann.sc"
typedef int block[64];

#line 37 "./global.sc"
typedef struct huffman_standard_structure XHuff;




typedef struct huffman_encoder EHuff;

#line 21 "./io.sc"
typedef short int WORD;
typedef long int DWORD;
typedef char BYTE;

#line 31 "./io.sc"
typedef struct tagBITMAPFILEHEADER BITMAPFILEHEADER;

#line 48 "./io.sc"
typedef struct tagBITMAPINFOHEADER BITMAPINFOHEADER;



typedef struct tagRGBTRIPLE RGBTRIPLE;

#line 31 "/home/kim/SpecC-2.1/inc/sim.sh"
typedef unsigned long long int sim_time;

#line 43 "/home/sw/tools/gcc/gcc-4.1.1/bin/../lib/gcc/i686-pc-linux-gnu/4.1.1/include/stdarg.h"
typedef void *__gnuc_va_list;

#line 214 "/home/sw/tools/gcc/gcc-4.1.1/bin/../lib/gcc/i686-pc-linux-gnu/4.1.1/include/stddef.h"
typedef unsigned int size_t;

#line 355 "/home/sw/tools/gcc/gcc-4.1.1/bin/../lib/gcc/i686-pc-linux-gnu/4.1.1/include/stddef.h"
typedef unsigned int wint_t;

#line 55 "/usr/include/_G_config.h"
typedef int _G_int16_t;
typedef int _G_int32_t;
typedef unsigned int _G_uint16_t;
typedef unsigned int _G_uint32_t;

#line 34 "/usr/include/bits/types.h"
typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef char __int8_t;
typedef unsigned char __uint8_t;
typedef short int __int16_t;
typedef unsigned short int __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;

#line 138 "/usr/include/bits/types.h"
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;

typedef unsigned int __mode_t;
typedef unsigned int __nlink_t;
typedef long int __off_t;

typedef int __pid_t;

typedef long int __clock_t;
typedef unsigned long int __rlim_t;

typedef unsigned int __id_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;

typedef int __daddr_t;
typedef long int __swblk_t;
typedef int __key_t;


typedef int __clockid_t;


typedef void *__timer_t;


typedef long int __blksize_t;




typedef long int __blkcnt_t;



typedef unsigned long int __fsblkcnt_t;



typedef unsigned long int __fsfilcnt_t;


typedef int __ssize_t;




typedef __quad_t *__qaddr_t;
typedef char *__caddr_t;


typedef int __intptr_t;


typedef unsigned int __socklen_t;

#line 70 "/usr/include/gconv.h"
typedef int (*__gconv_fct)(struct __gconv_step *, struct __gconv_step_data *, const unsigned char **, const unsigned char *, unsigned char **, unsigned int *, int, int);




typedef unsigned int (*__gconv_btowc_fct)(struct __gconv_step *, unsigned char);


typedef int (*__gconv_init_fct)(struct __gconv_step *);
typedef void (*__gconv_end_fct)(struct __gconv_step *);



typedef int (*__gconv_trans_fct)(struct __gconv_step *, struct __gconv_step_data *, void *, const unsigned char *, const unsigned char **, const unsigned char *, unsigned char **, unsigned int *);

#line 91 "/usr/include/gconv.h"
typedef int (*__gconv_trans_context_fct)(void *, const unsigned char *, const unsigned char *, unsigned char *, unsigned char *);




typedef int (*__gconv_trans_query_fct)(const char *, const char ***, unsigned int *);



typedef int (*__gconv_trans_init_fct)(void **, const char *);
typedef void (*__gconv_trans_end_fct)(void *);

#line 177 "/usr/include/gconv.h"
typedef struct __gconv_info *__gconv_t;

#line 176 "/usr/include/libio.h"
typedef void _IO_lock_t;

#line 337 "/usr/include/libio.h"
typedef struct _IO_FILE _IO_FILE;

#line 360 "/usr/include/libio.h"
typedef int __io_read_fn(void *, char *, unsigned int);

#line 368 "/usr/include/libio.h"
typedef int __io_write_fn(void *, const char *, unsigned int);

#line 377 "/usr/include/libio.h"
typedef int __io_seek_fn(void *, __quad_t *, int);


typedef int __io_close_fn(void *);

#line 46 "/usr/include/stdio.h"
typedef struct _IO_FILE FILE;

#line 62 "/usr/include/stdio.h"
typedef struct _IO_FILE __FILE;

#line 765 "/usr/include/stdlib.h"
typedef int (*__compar_fn_t)(const void *, const void *);

// class declarations ////////////////////////////////////////////////

#line 465 "tb.h"
class ISendBits;
class CBitAdapter;
class i_byte_sender;
class i_byte_receiver;
class i_byte_tranceiver;
class c_byte_double_handshake;
class i_int_sender;
class i_int_receiver;
class i_int_tranceiver;
class c_int_double_handshake;
class i_block_sender;
class i_block_receiver;
class i_block_tranceiver;
class c_block_double_handshake;
class ChenDct;
class PreshiftDctMatrix;
class BoundDctMatrix;
class DCT;
class InitTable;
class SizeTable;
class CodeTable;
class OrderCodes;
class SpecifiedHuffman;
class JpegDefaultHuffman;
class Design;
class JpegEncodeStripe;
class JpegEncode;
class ReceiveData;
class HandleData;
class JpegHeader;
class ZigzagMatrix;
class EncodeDC;
class EncodeAC;
class HuffmanEncode;
class Input;
class Output;
class JpegStart;
class JpegInit;
class JpegEnd;
class Jpeg;
class Quantization;
class i_receiver;
class i_sender;
class Main;

// interface class definitions ///////////////////////////////////////

#line 9 "./adapter.sc"
class ISendBits
{
private:
public:

#line 519 "tb.h"
    virtual ~ISendBits(void){};

#line 14 "./adapter.sc"
    virtual int WriteBits(int, int) = 0;

#line 12 "./adapter.sc"
    virtual int WriteByte(int) = 0;
    virtual void WriteMarker(int) = 0;

#line 11 "./adapter.sc"
    virtual int WriteWord(int) = 0;
private:
};

#line 23 "./chann.sc"
class i_byte_sender
{
private:
public:

#line 540 "tb.h"
    virtual ~i_byte_sender(void){};

#line 23 "./chann.sc"
    virtual void send(char) = 0;
private:
};

#line 24 "./chann.sc"
class i_byte_receiver
{
private:
public:

#line 554 "tb.h"
    virtual ~i_byte_receiver(void){};

#line 24 "./chann.sc"
    virtual void receive(char *) = 0;
private:
};

#line 25 "./chann.sc"
class i_byte_tranceiver
{
private:
public:

#line 568 "tb.h"
    virtual ~i_byte_tranceiver(void){};

#line 25 "./chann.sc"
    virtual void receive(char *) = 0; virtual void send(char) = 0;
private:
};

#line 33 "./chann.sc"
class i_int_sender
{
private:
public:

#line 582 "tb.h"
    virtual ~i_int_sender(void){};

#line 33 "./chann.sc"
    virtual void send(int) = 0;
private:
};

#line 34 "./chann.sc"
class i_int_receiver
{
private:
public:

#line 596 "tb.h"
    virtual ~i_int_receiver(void){};

#line 34 "./chann.sc"
    virtual void receive(int *) = 0;
private:
};

#line 35 "./chann.sc"
class i_int_tranceiver
{
private:
public:

#line 610 "tb.h"
    virtual ~i_int_tranceiver(void){};

#line 35 "./chann.sc"
    virtual void receive(int *) = 0; virtual void send(int) = 0;
private:
};

#line 45 "./chann.sc"
class i_block_sender
{
private:
public:

#line 624 "tb.h"
    virtual ~i_block_sender(void){};

#line 45 "./chann.sc"
    virtual void send(int [64]) = 0;
private:
};

#line 46 "./chann.sc"
class i_block_receiver
{
private:
public:

#line 638 "tb.h"
    virtual ~i_block_receiver(void){};

#line 46 "./chann.sc"
    virtual void receive(int (*)[64]) = 0;
private:
};

#line 47 "./chann.sc"
class i_block_tranceiver
{
private:
public:

#line 652 "tb.h"
    virtual ~i_block_tranceiver(void){};

#line 47 "./chann.sc"
    virtual void receive(int (*)[64]) = 0; virtual void send(int [64]) = 0;
private:
};

#line 23 "/home/kim/SpecC-2.1/import/i_receiver.sc"
class i_receiver
{
private:
public:

#line 666 "tb.h"
    virtual ~i_receiver(void){};

#line 25 "/home/kim/SpecC-2.1/import/i_receiver.sc"
    virtual void receive(void *, unsigned long int) = 0;
private:
};

#line 24 "/home/kim/SpecC-2.1/import/i_sender.sc"
class i_sender
{
private:
public:

#line 680 "tb.h"
    virtual ~i_sender(void){};

#line 26 "/home/kim/SpecC-2.1/import/i_sender.sc"
    virtual void send(const void *, unsigned long int) = 0;
private:
};

// behavior and channel class definitions ////////////////////////////

#line 18 "./adapter.sc"
class CBitAdapter : public _specc::channel, public ISendBits
{
private:

#line 18 "./adapter.sc"
    i_byte_sender (&chData);
public:

#line 699 "tb.h"
    CBitAdapter(i_byte_sender (&chData));
    virtual ~CBitAdapter(void);
    void FlushBits(void);
    int WriteBits(int, int);
    int WriteByte(int);
    void WriteMarker(int);
    int WriteWord(int);
private:

#line 22 "./adapter.sc"
    int left_bits;

#line 21 "./adapter.sc"
    unsigned char write_byte;
};

#line 26 "./chann.sc"
class c_byte_double_handshake : public _specc::channel, public i_byte_sender, public i_byte_receiver, public i_byte_tranceiver
{
private:
public:

#line 722 "tb.h"
    c_byte_double_handshake(void);
    virtual ~c_byte_double_handshake(void);
    void receive(char *);
    void send(char);
private:

#line 26 "./chann.sc"
    _specc::event ack; _specc::event req; char tmpd; bool v; bool w;
};

#line 36 "./chann.sc"
class c_int_double_handshake : public _specc::channel, public i_int_sender, public i_int_receiver, public i_int_tranceiver
{
private:
public:

#line 739 "tb.h"
    c_int_double_handshake(void);
    virtual ~c_int_double_handshake(void);
    void receive(int *);
    void send(int);
private:

#line 36 "./chann.sc"
    _specc::event ack; _specc::event req; int tmpd; bool v; bool w;
};

#line 48 "./chann.sc"
class c_block_double_handshake : public _specc::channel, public i_block_sender, public i_block_receiver, public i_block_tranceiver
{
private:
public:

#line 756 "tb.h"
    c_block_double_handshake(void);
    virtual ~c_block_double_handshake(void);
    void receive(int (*)[64]);
    void send(int [64]);
private:

#line 48 "./chann.sc"
    _specc::event ack; _specc::event req; int tmpd[64]; bool v; bool w;
};

#line 61 "./dct.sc"
class ChenDct : public _specc::behavior
{
private:

#line 61 "./dct.sc"
    int (&x)[64]; int (&y)[64];
public:

#line 776 "tb.h"
    ChenDct(int (&x)[64], int (&y)[64]);
    virtual ~ChenDct(void);
    void main(void);
private:
};

#line 174 "./dct.sc"
class PreshiftDctMatrix : public _specc::behavior
{
private:

#line 174 "./dct.sc"
    int (&mi)[64]; int (&mo)[64];
    int (&shift);
public:

#line 793 "tb.h"
    PreshiftDctMatrix(int (&mi)[64], int (&mo)[64], int (&shift));
    virtual ~PreshiftDctMatrix(void);
    void main(void);
private:
};

#line 186 "./dct.sc"
class BoundDctMatrix : public _specc::behavior
{
private:

#line 186 "./dct.sc"
    int (&mi)[64]; int (&mo)[64];
    int (&bound);
public:

#line 810 "tb.h"
    BoundDctMatrix(int (&mi)[64], int (&mo)[64], int (&bound));
    virtual ~BoundDctMatrix(void);
    void main(void);
private:
};

#line 204 "./dct.sc"
class DCT : public _specc::behavior
{
private:

#line 204 "./dct.sc"
    int (&HData)[64]; int (&DData)[64];
public:

#line 826 "tb.h"
    DCT(int (&HData)[64], int (&DData)[64]);
    virtual ~DCT(void);
    void main(void);
private:

#line 207 "./dct.sc"
    int DDataDct[64];

#line 206 "./dct.sc"
    int HDataDct[64];
    int _scc_const_port_0;
    int _scc_const_port_1;


    BoundDctMatrix boundDctMatrix;

#line 210 "./dct.sc"
    ChenDct chenDct;

#line 209 "./dct.sc"
    PreshiftDctMatrix preshiftDctMatrix;
};

#line 48 "./default.sc"
class InitTable : public _specc::behavior
{
private:

#line 48 "./default.sc"
    const int (&bts)[16]; const int (&hvls)[162];
    struct huffman_standard_structure (&xhuff);
public:

#line 860 "tb.h"
    InitTable(const int (&bts)[16], const int (&hvls)[162], struct huffman_standard_structure (&xhuff));
    virtual ~InitTable(void);
    void main(void);
private:
};

#line 70 "./default.sc"
class SizeTable : public _specc::behavior
{
private:

#line 70 "./default.sc"
    struct huffman_standard_structure (&xhuff); int (&huffsize)[257]; int (&lastp);
public:

#line 876 "tb.h"
    SizeTable(struct huffman_standard_structure (&xhuff), int (&huffsize)[257], int (&lastp));
    virtual ~SizeTable(void);
    void main(void);
private:
};

#line 87 "./default.sc"
class CodeTable : public _specc::behavior
{
private:

#line 87 "./default.sc"
    int (&huffsize)[257]; int (&huffcode)[257];
public:

#line 892 "tb.h"
    CodeTable(int (&huffsize)[257], int (&huffcode)[257]);
    virtual ~CodeTable(void);
    void main(void);
private:
};

#line 115 "./default.sc"
class OrderCodes : public _specc::behavior
{
private:

#line 115 "./default.sc"
    int (&lastp); struct huffman_standard_structure (&xhuff); struct huffman_encoder (&ehuff);
    int (&huffsize)[257]; int (&huffcode)[257];
public:

#line 909 "tb.h"
    OrderCodes(int (&lastp), struct huffman_standard_structure (&xhuff), struct huffman_encoder (&ehuff), int (&huffsize)[257], int (&huffcode)[257]);
    virtual ~OrderCodes(void);
    void main(void);
private:
};

#line 132 "./default.sc"
class SpecifiedHuffman : public _specc::behavior
{
private:

#line 132 "./default.sc"
    const int (&bts)[16]; const int (&hvls)[162];
    struct huffman_standard_structure (&Xhuff); struct huffman_encoder (&Ehuff);
public:

#line 926 "tb.h"
    SpecifiedHuffman(const int (&bts)[16], const int (&hvls)[162], struct huffman_standard_structure (&Xhuff), struct huffman_encoder (&Ehuff));
    virtual ~SpecifiedHuffman(void);
    void main(void);
private:

#line 135 "./default.sc"
    int huffcode[257];
    int huffsize[257];
    int lastp;



    CodeTable codeTable;

#line 139 "./default.sc"
    InitTable initTable;


    OrderCodes orderCodes;

#line 140 "./default.sc"
    SizeTable sizeTable;
};

#line 154 "./default.sc"
class JpegDefaultHuffman : public _specc::behavior
{
private:

#line 154 "./default.sc"
    struct huffman_standard_structure (&DCXhuff); struct huffman_encoder (&DCEhuff);
    struct huffman_standard_structure (&ACXhuff); struct huffman_encoder (&ACEhuff);
public:

#line 961 "tb.h"
    JpegDefaultHuffman(struct huffman_standard_structure (&DCXhuff), struct huffman_encoder (&DCEhuff), struct huffman_standard_structure (&ACXhuff), struct huffman_encoder (&ACEhuff));
    virtual ~JpegDefaultHuffman(void);
    void main(void);
private:

#line 157 "./default.sc"
    SpecifiedHuffman sh1;
    SpecifiedHuffman sh2;
};

#line 58 "./handle.sc"
class HandleData : public _specc::behavior
{
private:

#line 58 "./handle.sc"
    int (&mduWide); int (&mduHigh);
    unsigned char *(&stripe); int (&MDUWide);
    int (&HData)[64];
public:

#line 983 "tb.h"
    HandleData(int (&mduWide), int (&mduHigh), unsigned char *(&stripe), int (&MDUWide), int (&HData)[64]);
    virtual ~HandleData(void);
    void main(void);
private:

#line 62 "./handle.sc"
    int block;
};

#line 180 "./huff.sc"
class EncodeAC : public _specc::behavior
{
private:

#line 180 "./huff.sc"
    int (&matrix)[64]; struct huffman_encoder (&Ehuff);
    ISendBits (&chData);
public:

#line 1003 "tb.h"
    EncodeAC(int (&matrix)[64], struct huffman_encoder (&Ehuff), ISendBits (&chData));
    virtual ~EncodeAC(void);
    void main(void);
private:
};

#line 143 "./huff.sc"
class EncodeDC : public _specc::behavior
{
private:

#line 143 "./huff.sc"
    int (&matrix)[64]; struct huffman_encoder (&Ehuff);
    ISendBits (&chData);
public:

#line 1020 "tb.h"
    EncodeDC(int (&matrix)[64], struct huffman_encoder (&Ehuff), ISendBits (&chData));
    virtual ~EncodeDC(void);
    void main(void);
private:

#line 146 "./huff.sc"
    int LastDC;
};

#line 117 "./huff.sc"
class ZigzagMatrix : public _specc::behavior
{
private:

#line 117 "./huff.sc"
    int (&imatrix)[64]; int (&omatrix)[64];
public:

#line 1039 "tb.h"
    ZigzagMatrix(int (&imatrix)[64], int (&omatrix)[64]);
    virtual ~ZigzagMatrix(void);
    void main(void);
private:
};

#line 228 "./huff.sc"
class HuffmanEncode : public _specc::behavior
{
private:

#line 228 "./huff.sc"
    int (&QData)[64];
    struct huffman_encoder (&DCEhuff); struct huffman_encoder (&ACEhuff);
    ISendBits (&chData);
public:

#line 1057 "tb.h"
    HuffmanEncode(int (&QData)[64], struct huffman_encoder (&DCEhuff), struct huffman_encoder (&ACEhuff), ISendBits (&chData));
    virtual ~HuffmanEncode(void);
    void main(void);
private:

#line 232 "./huff.sc"
    int OData[64];



    EncodeAC encodeAC;

#line 235 "./huff.sc"
    EncodeDC encodeDC;

#line 234 "./huff.sc"
    ZigzagMatrix zigzagMatrix;
};

#line 12 "./quant.sc"
class Quantization : public _specc::behavior
{
private:

#line 12 "./quant.sc"
    int (&DData)[64]; int (&QData)[64];
public:

#line 1086 "tb.h"
    Quantization(int (&DData)[64], int (&QData)[64]);
    virtual ~Quantization(void);
    void main(void);
private:
};

#line 14 "./encode.sc"
class JpegEncodeStripe : public _specc::behavior
{
private:

#line 14 "./encode.sc"
    int (&mduHigh);
    unsigned char *(&stripe); int (&MDUWide);
    struct huffman_encoder (&DCEhuff); struct huffman_encoder (&ACEhuff);
    ISendBits (&chData);
public:

#line 1105 "tb.h"
    JpegEncodeStripe(int (&mduHigh), unsigned char *(&stripe), int (&MDUWide), struct huffman_encoder (&DCEhuff), struct huffman_encoder (&ACEhuff), ISendBits (&chData));
    virtual ~JpegEncodeStripe(void);
    void main(void);
private:

#line 22 "./encode.sc"
    _specc::piped<int [64],1> ddata;

#line 21 "./encode.sc"
    _specc::piped<int [64],1> hdata;

#line 19 "./encode.sc"
    int mduWide;



    _specc::piped<int [64],1> qdata;


    DCT dct;

#line 25 "./encode.sc"
    HandleData handleData;


    HuffmanEncode huffmanEncode;

#line 27 "./encode.sc"
    Quantization quantization;
};

#line 14 "./handle.sc"
class ReceiveData : public _specc::behavior
{
private:

#line 14 "./handle.sc"
    i_byte_receiver (&chPixel); int (&mduHigh);
    int (&ImageWidth); int (&ImageHeight);
    int (&MDUWide); unsigned char *(&stripe);
public:

#line 1148 "tb.h"
    ReceiveData(i_byte_receiver (&chPixel), int (&mduHigh), int (&ImageWidth), int (&ImageHeight), int (&MDUWide), unsigned char *(&stripe));
    virtual ~ReceiveData(void);
    void main(void);
private:
};

#line 44 "./encode.sc"
class JpegEncode : public _specc::behavior
{
private:

#line 44 "./encode.sc"
    i_byte_receiver (&chPixel);
    int (&ImageWidth); int (&ImageHeight);
    struct huffman_encoder (&DCEhuff); struct huffman_encoder (&ACEhuff);
    ISendBits (&chData);
public:

#line 1167 "tb.h"
    JpegEncode(i_byte_receiver (&chPixel), int (&ImageWidth), int (&ImageHeight), struct huffman_encoder (&DCEhuff), struct huffman_encoder (&ACEhuff), ISendBits (&chData));
    virtual ~JpegEncode(void);
    void main(void);
private:

#line 50 "./encode.sc"
    int MDUHigh;

    _specc::piped<int,1> MDUWide;

#line 49 "./encode.sc"
    int mduHigh;



    _specc::piped<unsigned char *,1> stripe;



    JpegEncodeStripe encodeStripe;

#line 55 "./encode.sc"
    ReceiveData receiveData;
};

#line 66 "./jpeg.sc"
class JpegEnd : public _specc::behavior
{
private:

#line 66 "./jpeg.sc"
    ISendBits (&chData);
public:

#line 1202 "tb.h"
    JpegEnd(ISendBits (&chData));
    virtual ~JpegEnd(void);
    void main(void);
private:
};

#line 25 "./header.sc"
class JpegHeader : public _specc::behavior
{
private:

#line 25 "./header.sc"
    int (&ImageWidth); int (&ImageHeight);
    struct huffman_standard_structure (&DCXhuff); struct huffman_standard_structure (&ACXhuff);
    ISendBits (&chData);
public:

#line 1220 "tb.h"
    JpegHeader(int (&ImageWidth), int (&ImageHeight), struct huffman_standard_structure (&DCXhuff), struct huffman_standard_structure (&ACXhuff), ISendBits (&chData));
    virtual ~JpegHeader(void);
    void WriteAPP0();
    void WriteDHT();
    void WriteDQT();
    void WriteHuffman(struct huffman_standard_structure *);
    void WriteSOF(void);
    void WriteSOS();
    void main(void);
private:
};

#line 17 "./jpeg.sc"
class JpegStart : public _specc::behavior
{
private:

#line 17 "./jpeg.sc"
    i_int_receiver (&chHeader);
    int (&ImageWidth); int (&ImageHeight);
public:

#line 1243 "tb.h"
    JpegStart(i_int_receiver (&chHeader), int (&ImageWidth), int (&ImageHeight));
    virtual ~JpegStart(void);
    void main(void);
private:
};

#line 46 "./jpeg.sc"
class JpegInit : public _specc::behavior
{
private:

#line 46 "./jpeg.sc"
    i_int_receiver (&chHeader); ISendBits (&chData);
    struct huffman_encoder (&DCEhuff); struct huffman_encoder (&ACEhuff);
    int (&ImageWidth); int (&ImageHeight);
public:

#line 1261 "tb.h"
    JpegInit(i_int_receiver (&chHeader), ISendBits (&chData), struct huffman_encoder (&DCEhuff), struct huffman_encoder (&ACEhuff), int (&ImageWidth), int (&ImageHeight));
    virtual ~JpegInit(void);
    void main(void);
private:

#line 50 "./jpeg.sc"
    struct huffman_standard_structure ACXhuff; struct huffman_standard_structure DCXhuff;



    JpegHeader header;

#line 53 "./jpeg.sc"
    JpegDefaultHuffman huffman;

#line 52 "./jpeg.sc"
    JpegStart start;
};

#line 82 "./jpeg.sc"
class Jpeg : public _specc::behavior
{
private:

#line 82 "./jpeg.sc"
    i_int_receiver (&chHeader); i_byte_receiver (&chPixel);
    i_byte_sender (&chData);
public:

#line 1291 "tb.h"
    Jpeg(i_int_receiver (&chHeader), i_byte_receiver (&chPixel), i_byte_sender (&chData));
    virtual ~Jpeg(void);
    void main(void);
private:

#line 86 "./jpeg.sc"
    struct huffman_encoder ACEhuff; struct huffman_encoder DCEhuff;

#line 85 "./jpeg.sc"
    int imageHeight; int imageWidth;


    CBitAdapter bitAdapter;



    JpegEncode encode;

    JpegEnd end;

#line 90 "./jpeg.sc"
    JpegInit init;
};

#line 11 "./design.sc"
class Design : public _specc::behavior
{
private:

#line 11 "./design.sc"
    i_int_receiver (&chHeader);
    i_byte_receiver (&chPixel);
    i_byte_sender (&chData);
public:

#line 1327 "tb.h"
    Design(i_int_receiver (&chHeader), i_byte_receiver (&chPixel), i_byte_sender (&chData));
    virtual ~Design(void);
    void main(void);
private:

#line 15 "./design.sc"
    Jpeg jpeg;
};

#line 58 "./io.sc"
class Input : public _specc::behavior
{
private:

#line 58 "./io.sc"
    char *(&ifname);
    i_int_sender (&Header);
    i_byte_sender (&Pixel);
public:

#line 1348 "tb.h"
    Input(char *(&ifname), i_int_sender (&Header), i_byte_sender (&Pixel));
    virtual ~Input(void);
    int IsBmpFile();
    void ReadBmpHeader();
    int ReadByte();
    long int ReadDWord();
    long int ReadRevDWord();
    int ReadRevWord();
    int ReadWord();
    void main(void);
private:

#line 65 "./io.sc"
    struct tagRGBTRIPLE *BmpColors;

#line 63 "./io.sc"
    struct tagBITMAPFILEHEADER BmpFileHeader;
    struct tagBITMAPINFOHEADER BmpInfoHeader;

    int BmpScanHeight; int BmpScanWidth;

#line 62 "./io.sc"
    struct _IO_FILE *ifp;
};

#line 260 "./io.sc"
class Output : public _specc::behavior
{
private:

#line 260 "./io.sc"
    char *(&ofname);
    i_byte_receiver (&Data);
public:

#line 1384 "tb.h"
    Output(char *(&ofname), i_byte_receiver (&Data));
    virtual ~Output(void);
    void main(void);
private:

#line 263 "./io.sc"
    struct _IO_FILE *ofp;
};

#line 10 "tb.sc"
class Main
{
private:
public:

#line 1400 "tb.h"
    Main(void);
    virtual ~Main(void);
    int main(int, char **);
private:

#line 12 "tb.sc"
    char *ifname;
    char *ofname;




    c_byte_double_handshake data;



    Design design;

#line 16 "tb.sc"
    c_int_double_handshake header;



    Input input;


    Output output;

#line 17 "tb.sc"
    c_byte_double_handshake pixel;
};

// variable and function declarations ////////////////////////////////

#line 16 "./extern.h"
extern "C" void error(const char *, ...);


extern "C" void msg(const char *, ...);



extern "C" struct _IO_FILE *stdOut();
extern "C" struct _IO_FILE *stdErr();

#line 1446 "tb.h"
void PrintTable(int *);
void PrintHuffman(struct huffman_encoder *, struct huffman_standard_structure *);
void EncodeHuffman(struct huffman_encoder *, int, ISendBits &);

#line 43 "/home/kim/SpecC-2.1/inc/sim.sh"
extern "C" unsigned long long int now(void);


extern "C" const char *time2str(unsigned long long int);


extern "C" unsigned long long int str2time(const char *);


extern "C" char *ll2str(unsigned int, char *, long long int);




extern "C" char *ull2str(unsigned int, char *, unsigned long long int);




extern "C" long long int str2ll(unsigned int, const char *);



extern "C" unsigned long long int str2ull(unsigned int, const char *);



extern "C" char *bit2str(unsigned int, char *, ...);

#line 76 "/home/kim/SpecC-2.1/inc/sim.sh"
extern "C" char *ubit2str(unsigned int, char *, ...);




extern "C" void str2bit(unsigned int, const char *, ...);




extern "C" void str2ubit(unsigned int, const char *, ...);

#line 342 "/usr/include/libio.h"
extern struct _IO_FILE_plus _IO_2_1_stdin_;
extern struct _IO_FILE_plus _IO_2_1_stdout_;
extern struct _IO_FILE_plus _IO_2_1_stderr_;

#line 412 "/usr/include/libio.h"
extern "C" int __underflow(struct _IO_FILE *);
extern "C" int __uflow(struct _IO_FILE *);
extern "C" int __overflow(struct _IO_FILE *, int);
extern "C" unsigned int __wunderflow(struct _IO_FILE *);
extern "C" unsigned int __wuflow(struct _IO_FILE *);
extern "C" unsigned int __woverflow(struct _IO_FILE *, unsigned int);

#line 450 "/usr/include/libio.h"
extern "C" int _IO_getc(struct _IO_FILE *);
extern "C" int _IO_putc(int, struct _IO_FILE *);
extern "C" int _IO_feof(struct _IO_FILE *);
extern "C" int _IO_ferror(struct _IO_FILE *);

extern "C" int _IO_peekc_locked(struct _IO_FILE *);

#line 461 "/usr/include/libio.h"
extern "C" void _IO_flockfile(struct _IO_FILE *);
extern "C" void _IO_funlockfile(struct _IO_FILE *);
extern "C" int _IO_ftrylockfile(struct _IO_FILE *);

#line 480 "/usr/include/libio.h"
extern "C" int _IO_vfscanf(struct _IO_FILE *, const char *, void *, int *);

extern "C" int _IO_vfprintf(struct _IO_FILE *, const char *, void *);

extern "C" int _IO_padn(struct _IO_FILE *, int, int);
extern "C" unsigned int _IO_sgetn(struct _IO_FILE *, void *, unsigned int);

extern "C" __quad_t _IO_seekoff(struct _IO_FILE *, __quad_t, int, int);
extern "C" __quad_t _IO_seekpos(struct _IO_FILE *, __quad_t, int);

extern "C" void _IO_free_backup_area(struct _IO_FILE *);

#line 142 "/usr/include/stdio.h"
extern struct _IO_FILE *stdin;
extern struct _IO_FILE *stdout;
extern struct _IO_FILE *stderr;

#line 154 "/usr/include/stdio.h"
extern "C" int remove(const char *);

extern "C" int rename(const char *, const char *);

#line 171 "/usr/include/stdio.h"
extern "C" struct _IO_FILE *tmpfile(void);

#line 185 "/usr/include/stdio.h"
extern "C" char *tmpnam(char *);

#line 213 "/usr/include/stdio.h"
extern "C" int fclose(struct _IO_FILE *);




extern "C" int fflush(struct _IO_FILE *);

#line 248 "/usr/include/stdio.h"
extern "C" struct _IO_FILE *fopen(const char *, const char *);

#line 254 "/usr/include/stdio.h"
extern "C" struct _IO_FILE *freopen(const char *, const char *, struct _IO_FILE *);

#line 303 "/usr/include/stdio.h"
extern "C" void setbuf(struct _IO_FILE *, char *);



extern "C" int setvbuf(struct _IO_FILE *, char *, int, unsigned int);

#line 327 "/usr/include/stdio.h"
extern "C" int fprintf(struct _IO_FILE *, const char *, ...);

#line 333 "/usr/include/stdio.h"
extern "C" int printf(const char *, ...);

extern "C" int sprintf(char *, const char *, ...);

#line 342 "/usr/include/stdio.h"
extern "C" int vfprintf(struct _IO_FILE *, const char *, void *);

#line 348 "/usr/include/stdio.h"
extern "C" int vprintf(const char *, void *);

extern "C" int vsprintf(char *, const char *, void *);

#line 399 "/usr/include/stdio.h"
extern "C" int fscanf(struct _IO_FILE *, const char *, ...);

#line 405 "/usr/include/stdio.h"
extern "C" int scanf(const char *, ...);

extern "C" int sscanf(const char *, const char *, ...);

#line 441 "/usr/include/stdio.h"
extern "C" int fgetc(struct _IO_FILE *);
extern "C" int getc(struct _IO_FILE *);

#line 448 "/usr/include/stdio.h"
extern "C" int getchar(void);

#line 483 "/usr/include/stdio.h"
extern "C" int fputc(int, struct _IO_FILE *);
extern "C" int putc(int, struct _IO_FILE *);

#line 490 "/usr/include/stdio.h"
extern "C" int putchar(int);

#line 532 "/usr/include/stdio.h"
extern "C" char *fgets(char *, int, struct _IO_FILE *);

#line 540 "/usr/include/stdio.h"
extern "C" char *gets(char *);

#line 590 "/usr/include/stdio.h"
extern "C" int fputs(const char *, struct _IO_FILE *);

#line 596 "/usr/include/stdio.h"
extern "C" int puts(const char *);

#line 603 "/usr/include/stdio.h"
extern "C" int ungetc(int, struct _IO_FILE *);

#line 610 "/usr/include/stdio.h"
extern "C" unsigned int fread(void *, unsigned int, unsigned int, struct _IO_FILE *);

#line 616 "/usr/include/stdio.h"
extern "C" unsigned int fwrite(const void *, unsigned int, unsigned int, struct _IO_FILE *);

#line 650 "/usr/include/stdio.h"
extern "C" int fseek(struct _IO_FILE *, long int, int);




extern "C" long int ftell(struct _IO_FILE *);




extern "C" void rewind(struct _IO_FILE *);

#line 699 "/usr/include/stdio.h"
extern "C" int fgetpos(struct _IO_FILE *, _G_fpos_t *);




extern "C" int fsetpos(struct _IO_FILE *, const _G_fpos_t *);

#line 727 "/usr/include/stdio.h"
extern "C" void clearerr(struct _IO_FILE *);

extern "C" int feof(struct _IO_FILE *);

extern "C" int ferror(struct _IO_FILE *);

#line 747 "/usr/include/stdio.h"
extern "C" void perror(const char *);

#line 140 "/usr/include/stdlib.h"
extern "C" unsigned int __ctype_get_mb_cur_max(void);




extern "C" double atof(const char *);


extern "C" int atoi(const char *);


extern "C" long int atol(const char *);

#line 165 "/usr/include/stdlib.h"
extern "C" double strtod(const char *, char **);

#line 184 "/usr/include/stdlib.h"
extern "C" long int strtol(const char *, char **, int);



extern "C" unsigned long int strtoul(const char *, char **, int);

#line 279 "/usr/include/stdlib.h"
extern "C" double __strtod_internal(const char *, char **, int);


extern "C" float __strtof_internal(const char *, char **, int);


extern "C" long double __strtold_internal(const char *, char **, int);




extern "C" long int __strtol_internal(const char *, char **, int, int);

#line 297 "/usr/include/stdlib.h"
extern "C" unsigned long int __strtoul_internal(const char *, char **, int, int);

#line 498 "/usr/include/stdlib.h"
extern "C" int rand(void);

extern "C" void srand(unsigned int);

#line 589 "/usr/include/stdlib.h"
extern "C" void *malloc(unsigned int);

extern "C" void *calloc(unsigned int, unsigned int);

#line 600 "/usr/include/stdlib.h"
extern "C" void *realloc(void *, unsigned int);


extern "C" void free(void *);

#line 628 "/usr/include/stdlib.h"
extern "C" void abort(void);



extern "C" int atexit(void (*)(void));

#line 646 "/usr/include/stdlib.h"
extern "C" void exit(int);

#line 660 "/usr/include/stdlib.h"
extern "C" char *getenv(const char *);




extern "C" char *__secure_getenv(const char *);

#line 738 "/usr/include/stdlib.h"
extern "C" int system(const char *);

#line 775 "/usr/include/stdlib.h"
extern "C" void *bsearch(const void *, const void *, unsigned int, unsigned int, int (*)(const void *, const void *));

#line 781 "/usr/include/stdlib.h"
extern "C" void qsort(void *, unsigned int, unsigned int, int (*)(const void *, const void *));




extern "C" int abs(int);
extern "C" long int labs(long int);

#line 800 "/usr/include/stdlib.h"
extern "C" div_t div(int, int);

extern "C" ldiv_t ldiv(long int, long int);

#line 874 "/usr/include/stdlib.h"
extern "C" int mblen(const char *, unsigned int);


extern "C" int mbtowc(long int *, const char *, unsigned int);



extern "C" int wctomb(char *, long int);



extern "C" unsigned int mbstowcs(long int *, const char *, unsigned int);


extern "C" unsigned int wcstombs(char *, const long int *, unsigned int);

//////////////////////////////////////////////////////////////////////
// End of file tb.h
//////////////////////////////////////////////////////////////////////