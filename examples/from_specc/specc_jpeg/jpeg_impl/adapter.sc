/**************************************************************************
**	Adapter channel for output of bit stream over byte channel
**************************************************************************/

import "global";
import "chann";


interface ISendBits
{
  int WriteWord(int code);
  int WriteByte(int code);
  void WriteMarker(int m);
  int WriteBits(int n, int code);
};


channel CBitAdapter(i_byte_sender chData)
  implements ISendBits
{
  unsigned char write_byte = 0;
  int left_bits = 8;

  int WriteWord(int code)
  {
    chData.send((char)(code>>8));
    chData.send((char)(code & 0xff));
    return 2;
  }

  int WriteByte(int code)
  {
    chData.send((char)code);
    return 0;
  }
  
  void WriteMarker(int m)
  {
    chData.send(0xff);
    chData.send((char)m);
  }

  void FlushBits(void)
  {
#ifdef DEBUG
    msg("WriteBits: Sending data = %x...\n", write_byte);
#endif		  
    chData.send(write_byte);
    if (write_byte == 0xff)	{
#ifdef DEBUG		  
      msg("WriteBits: Sending padding data = 0...\n");
#endif		  
      chData.send(0);
    }
#ifdef DEBUG		  
    msg("WriteBits: Data sent.\n");
#endif
  }
  
    
  int WriteBits(int n, int code)
  {
    int p;

    unsigned lmask[] = {
        0x0000, 
	0x0001, 0x0003, 0x0007, 0x000f,
	0x001f, 0x003f, 0x007f, 0x00ff,
	0x01ff, 0x03ff, 0x07ff, 0x0fff,
	0x1fff, 0x3fff, 0x7fff, 0xffff
    };

    waitfor(4000);
    
    // synchronize buffer value
    if (n < 0) 
    {
      if (left_bits < 8) {
	n = left_bits;
	FlushBits();
	write_byte = 0;
	left_bits  = 8;
      }
      else	
	n = 0;
      
      return n;
    }

    code &= lmask[n];
    p = n - left_bits;

    if (n == left_bits) {
      write_byte |= code;
      FlushBits();
      write_byte = 0;
      left_bits  = 8;
    }
    else if (n > left_bits) {
      write_byte |= (code >> p);
      FlushBits();
      if (p > 8) {
	write_byte = (0xff & (code >> (p - 8)));
	FlushBits();
	p -= 8;
      }

      write_byte = (code & lmask[p]) << (8 - p);
      left_bits = 8 - p;
    }
    else {
      write_byte |= (code << -p);
      left_bits -= n;
    }

    return n;
  }
};

