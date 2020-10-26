#ifndef IO_INCLUDED
#define IO_INCLUDED

// ----------------------------------------------------------------------
// Input/Output behaviors for testbench
// ----------------------------------------------------------------------
//
// 05/17/99  A. Gerstlauer
//   * Fixed a bug in BMP file reading: scan lines are aligned to
//     LONG boundaries.
//
// 05/10/99  A. Gerstlauer
//

#include "global.h"

// ----------------------------------------------------------------------
// Input stimuli generator

// Type definitions for BMP file format

typedef short WORD;
typedef long  DWORD;
typedef char  BYTE;

typedef struct tagBITMAPFILEHEADER {
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	/* BITMAP core header info -> OS/2 */
	DWORD biSize;
	DWORD biWidth;
	DWORD biHeight;
	WORD biPlanes;
	WORD biBitCount;

	/* BITMAP info -> Windows 3.1 */
	DWORD biCompression;
	DWORD biSizeImage;
	DWORD biXPelsPerMeter;
	DWORD biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBTRIPLE {
	BYTE B, G, R;
} RGBTRIPLE;


// Input behavior
// Read BMP file and send data to JPEG behavior.
// NOTE: Only able to handle certain BMP files: grayscale, non-compressed...
OSSS_BEHAVIOUR(Input)
{
 public:
  
  sc_core::sc_port<osss::osss_sender_if<int> >  Header;
  sc_core::sc_port<osss::osss_sender_if<char> > Pixel;

 protected:
  std::string in_file_name;
  FILE* ifp;
  BITMAPFILEHEADER BmpFileHeader;
  BITMAPINFOHEADER BmpInfoHeader;
  RGBTRIPLE *BmpColors;
  int BmpScanWidth, BmpScanHeight;
  
  int ReadRevWord()
  {
    int c;
    c = fgetc(ifp);
    c |= fgetc(ifp) << 8;
    
    return c;
  }

  int ReadWord()
  {
    int c;
    c = fgetc(ifp) << 8;
    c |= fgetc(ifp);

    return c;
  }

  int ReadByte()
  {
    return fgetc(ifp);
  }

  long ReadRevDWord()
  {
    long c;
    c = fgetc(ifp);
    c |= fgetc(ifp) << 8;
    c |= fgetc(ifp) << 16;
    c |= fgetc(ifp) << 24;
    
    return c;
  }

  long ReadDWord()
  {
    long c;
    c = fgetc(ifp) << 24;
    c |= fgetc(ifp) << 16;
    c |= fgetc(ifp) << 8;
    c |= fgetc(ifp);
    
    return c;
  }
    
  int  IsBmpFile()
  {
    int t = ('B'<<8) | 'M';
    int c;
    c = ReadWord();
    fseek(ifp, -2, 1);
    
    return t == c;
  }
  
  void ReadBmpHeader()
  {
    int i, count;

    if (!IsBmpFile()) {
      error("This file is not compatible with BMP format.\n");
    }

    /* BMP file header */
    BmpFileHeader.bfType = ReadWord();
    BmpFileHeader.bfSize = ReadRevDWord();
    BmpFileHeader.bfReserved1 = ReadRevWord();
    BmpFileHeader.bfReserved2 = ReadRevWord();
    BmpFileHeader.bfOffBits = ReadRevDWord();

    /* BMP core info */
    BmpInfoHeader.biSize = ReadRevDWord();
    BmpInfoHeader.biWidth = ReadRevDWord();
    BmpInfoHeader.biHeight = ReadRevDWord();
    BmpInfoHeader.biPlanes = ReadRevWord();
    BmpInfoHeader.biBitCount = ReadRevWord();

    if (BmpInfoHeader.biSize > 12) {
      BmpInfoHeader.biCompression = ReadRevDWord();
      BmpInfoHeader.biSizeImage = ReadRevDWord();
      BmpInfoHeader.biXPelsPerMeter = ReadRevDWord();
      BmpInfoHeader.biYPelsPerMeter = ReadRevDWord();
      BmpInfoHeader.biClrUsed = ReadRevDWord();
      BmpInfoHeader.biClrImportant = ReadRevDWord();

      /* read RGBQUAD */
      count = BmpFileHeader.bfOffBits - ftell(ifp);
      count >>= 2;
      
      BmpColors = (RGBTRIPLE*) calloc(sizeof(RGBTRIPLE), count);
      
      for (i=0; i<count; i++) {
	BmpColors[i].B = ReadByte();
	BmpColors[i].G = ReadByte();
	BmpColors[i].R = ReadByte();
	ReadByte();
      }
    }
    else {
      /* read RGBTRIPLE */
      count = BmpFileHeader.bfOffBits - ftell(ifp);
      count /= 3;
      
      BmpColors = (RGBTRIPLE*) calloc(sizeof(RGBTRIPLE), count);
      
      for (i=0; i<count; i++) {
	BmpColors[i].B = ReadByte();
	BmpColors[i].G = ReadByte();
	BmpColors[i].R = ReadByte();
      }
    }

    /* BMP scan line is aligned by LONG boundary */
    if (BmpInfoHeader.biBitCount == 24) {
      BmpScanWidth = ((BmpInfoHeader.biWidth*3 + 3) >> 2) << 2;
      BmpScanHeight = BmpInfoHeader.biHeight;
    }
    else {
      BmpScanWidth = ((BmpInfoHeader.biWidth + 3) >> 2) << 2;
      BmpScanHeight = BmpInfoHeader.biHeight;
    }
  }
  
 public:

  Input(osss::osss_behaviour_name name_,
        const std::string& in_file_name_) :
    osss::osss_behaviour(name_),
    in_file_name(in_file_name_),
    ifp(NULL),
    BmpFileHeader(),
    BmpInfoHeader(),
    BmpColors(NULL),
    BmpScanWidth(0), 
    BmpScanHeight(0)
  { }
  
  void main() 
  {
   
    char buf;
    
    // Open file
    ifp = fopen(in_file_name.c_str(), "rb");
    if (!ifp) {
      std::cout << in_file_name << std::endl;
      error("Cannot open input file %s\n", in_file_name.c_str());
    }

    // Read BMP file header
    ReadBmpHeader();

#ifdef JPEG_DEBUG    
    msg("Input: Read BMP header, sending width & height...\n");
#endif    
    
    // Send header (size) information
    Header->send(BmpInfoHeader.biWidth);
    Header->send(BmpInfoHeader.biHeight);
    
#ifdef JPEG_DEBUG    
    msg("Input: Sent header width=%ld, height=%ld\n", 
	BmpInfoHeader.biWidth, BmpInfoHeader.biHeight);
#endif    
    
    // Loop over rows
    for (int r = BmpInfoHeader.biHeight - 1; r >= 0; r--) 
    {
      // Position file pointer to corresponding row
      fseek (ifp, BmpFileHeader.bfOffBits + r * BmpScanWidth, 0);
      
      // Loop over columns
      for (int c = 0; c < BmpInfoHeader.biWidth; c++) {
	// Read pixel
	if (ferror(ifp) || (fread(&buf, 1, 1, ifp) != 1)) {
	  error("Error reading data from file %s\n", in_file_name.c_str());
	}
	
#ifdef JPEG_DEBUG	
	msg("Input: Read pixel(%d,%d) = %x, sending it...\n", c, r, buf);
#endif	
	
	// Send off 
	Pixel->send(buf);
	
#ifdef JPEG_DEBUG	
	msg("Input: Sent pixel(%d,%d) = %x\n", c, r, buf);
#endif	
      }
    }   
    
    fclose (ifp);
  }
};


// ----------------------------------------------------------------------
// Output monitor.

// End of image marker
#define MARKER     0xFF
#define MARK_EOI   0xD9


// Output behavior
// Listen to JPEG output and write it into a file.
OSSS_BEHAVIOUR(Output)
{
 public:
  osss::osss_port<osss::osss_receiver_if<char> > Data;

 protected:
  std::string out_file_name;
  FILE* ofp;
    
 public:

  Output(osss::osss_behaviour_name name_,
         const std::string& out_file_name_) :
    osss::osss_behaviour(name_),
    out_file_name(out_file_name_),
    ofp(NULL)
  {}

  void main() 
  {
    bool running, marker;
    char buf;
    
    // Open file
    if (out_file_name.c_str()) {
      ofp = fopen(out_file_name.c_str(), "wb");
      if (!ofp) {
	error("Cannot open output file %s\n", out_file_name.c_str());
      }
    }
    else {
      ofp = stdOut();
    }

    // Read JPEG data, write to file
    running = true;
    marker = false;
    while (running)
    {
#ifdef JPEG_DEBUG	
      msg("Output: receiving data...\n");
#endif
      
      Data->receive(buf);
      
#ifdef JPEG_DEBUG	
      msg("Output: got data = %x\n", buf);
#endif
      
      if ((fwrite(&buf, 1, 1, ofp) != 1) || ferror(ofp)) {
	error("Error writing to file %s\n", out_file_name.c_str());
      }
      
      // Is it a marker?
      if (marker) {
	// End of image?
	running = ((unsigned char)buf != MARK_EOI);
	marker = false;
      }
      else {
	marker = ((unsigned char)buf == MARKER);
      }
      
    }
    
    fclose (ofp);
    
    // End of simulation
    wait(1000000);
    //exit(0);
  }
};

#endif
