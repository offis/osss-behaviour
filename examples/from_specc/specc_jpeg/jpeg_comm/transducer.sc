/************	JPEG encoder Transducer 	*******/
/************	transducer.sc	*******/
/************	A. Gerstlauer	*******/
/************	06/07/2001	*******/


import "global";
import "bus";


behavior Transducer(in  signal bit[31:0] MWDATA,
		    out signal bit[31:0] MRDATA,
		    in  signal bit[1]    MTSB,
		    out signal bit[1]    MTAB,
		    in  signal bit[1]    MWDATAOE,
		        signal bit[31:0] DB,
		    out signal bit[1]    DBOE,
		    out signal bit[1]    TSB,
		    in  signal bit[1]    TAB,
		    in  signal bit[1]    MRWB
		    )
{
  void main(void) 
  {
    bit[31:0] data;
    
    // T0
    t1: MTAB = 1;
	TSB = 1;
    
        wait(falling MTSB);
	
	if(MRWB)
	{
	  // TR1
	  MTAB = 1;
	  TSB = 0;
	  waitfor(4);

	  // TR2
	  MTAB = 1;
	  TSB = 1;
	  waitfor(4);

	  // TR3
	  wait(falling TAB);
	  MTAB = 1;
	  data = DB;
	  waitfor(4);

	  // TR4
	  MRDATA = data;
	  MTAB = 0;
	  waitfor(4);

	  // TR5
	  MRDATA = data;
	  MTAB = 0;
	  waitfor(4);

	  goto t1;
	}
        else
        {
	  // TW1
	  MTAB = 1;
	  waitfor(4);
	
	  // TW2
	  while(!MWDATAOE)
	  {	
	    MTAB = 1;
	    waitfor(4) ;
	  }
		
	  // TW3
	  TSB = 0;
	  MTAB = 1;
	  DBOE = 0;
	  data = MWDATA;
	  waitfor(4);
		
	  // TW4
	  TSB = 1;	
	  MTAB = 1;
	  DB = data;
	  DBOE = 1;
	  waitfor(4);
		
	  // WS5
	  MTAB = 0;
	  waitfor(4);
	  
	  // WS6
	  MTAB = 0;
	  waitfor(4);
	  
	  goto t1;
	}
  }
};

