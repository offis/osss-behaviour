#ifndef TB_INCLUDED
#define TB_INCLUDED

/************	JPEG encoder testbench 	*******/
/************	tb.sc		*******/
/************	A. Gerstlauer	*******/
/************	06/06/2001	*******/

#include "io.h"
#include "design.h"

OSSS_BEHAVIOUR(Main)
{
 protected:
  osss::osss_double_handshake_channel<int>  header_ch;
  osss::osss_double_handshake_channel<char> pixel_ch;
  osss::osss_double_handshake_channel<char> data_ch;
  
  Input   input;
  Design  design; 
  Output  output;
  
 public:
  
  Main(osss::osss_behaviour_name name, 
       const std::string& in_file_name, 
       const std::string& out_file_name) :
    osss::osss_behaviour(name),
    header_ch("header_ch"),
    pixel_ch("pixel_ch"),
    data_ch("data_ch"),
    input("input", in_file_name),
    design("design"),
    output("output", out_file_name)
  {
    input.Header(header_ch);
    input.Pixel(pixel_ch);

    design.chHeader(header_ch);
    design.chPixel(pixel_ch);
    design.chData(data_ch);

    output.Data(data_ch);
    
    // And now run the stuff...
    osss_par( input | design | output );
  }
  
};

#endif
