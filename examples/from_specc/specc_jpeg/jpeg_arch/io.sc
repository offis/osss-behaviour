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

import "global";
import "chann";


// ----------------------------------------------------------------------
// Input stimuli generator

// Type definitions for BMP file format

typedef short WORD;
typedef long DWORD;
typedef byte BYTE;

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
behavior Input (in char*      ifname,
		i_int_sender  Header,
		i_byte_sender Pixel)
{
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
    
  
  void main(void) 
  {
    int c, r;
    char buf;
    
    // Open file
    ifp = fopen(ifname, "rb");
    if (!ifp) {
      error("Cannot open input file %s\n", ifname);
    }

    // Read BMP file header
    ReadBmpHeader();

#ifdef DEBUG    
    msg("Input: Read BMP header, sending width & height...\n");
#endif    
    
    // Send header (size) information
    Header.send (BmpInfoHeader.biWidth);
    Header.send (BmpInfoHeader.biHeight);
    
#ifdef DEBUG    
    msg("Input: Sent header width=%ld, height=%ld\n", 
	BmpInfoHeader.biWidth, BmpInfoHeader.biHeight);
#endif    
    
    // Loop over rows
    for (r = BmpInfoHeader.biHeight - 1; r >= 0; r--) 
    {
      // Position file pointer to corresponding row
      fseek (ifp, BmpFileHeader.bfOffBits + r * BmpScanWidth, 0);
      
      // Loop over columns
      for (c = 0; c < BmpInfoHeader.biWidth; c++) {
	// Read pixel
	if (ferror(ifp) || (fread(&buf, 1, 1, ifp) != 1)) {
	  error("Error reading data from file %s\n", ifname);
	}
	
#ifdef DEBUG	
	msg("Input: Read pixel(%d,%d) = %x, sending it...\n", c, r, buf);
#endif	
	
	// Send off 
	Pixel.send(buf);
	
#ifdef DEBUG	
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
behavior Output (in char*        ofname,
	 	 i_byte_receiver Data)
{
  FILE* ofp;
    
  void main(void) 
  {
    bool running, marker;
    byte buf;
    
    // Open file
    if (ofname) {
      ofp = fopen(ofname, "wb");
      if (!ofp) {
	error("Cannot open output file %s\n", ofname);
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
#ifdef DEBUG	
      msg("Output: receiving data...\n");
#endif
      
      Data.receive(&buf);
      
#ifdef DEBUG	
      msg("Output: got data = %x\n", buf);
#endif
      
      if ((fwrite(&buf, 1, 1, ofp) != 1) || ferror(ofp)) {
	error("Error writing to file %s\n", ofname);
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
    waitfor(1000000);    
    exit(0);
  }
};

