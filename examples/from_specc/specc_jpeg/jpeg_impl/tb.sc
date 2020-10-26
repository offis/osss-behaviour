/************	JPEG encoder testbench 	*******/
/************	tb.sc		*******/
/************	A. Gerstlauer	*******/
/************	06/06/2001	*******/

import "io";
import "design";


behavior Main
{
  char* ifname;
  char* ofname;
  
  // Channels
  c_int_double_handshake  header;
  c_byte_double_handshake pixel;
  c_byte_double_handshake data;
    
  Input  input(ifname, header, pixel);
  Design design(header, pixel, data); 
  Output output(ofname, data);
  
  
  int main (int argc, char** argv)
  {
    // Command line arguments
    if (argc < 2) {
      error("Usage: %s infile [ outfile ]\n", argv[0]);
    }
    ifname = argv[1];
    if (argc >= 3) {
      ofname = argv[2];
    } else {
      ofname = 0;
    }
    
    // And now run the stuff...
    par {
      input.main();
      design.main();
      output.main();
    }
    
    return 0;
  }
  
};
