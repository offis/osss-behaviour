/************	JPEG encode 	*******/
/************	DCT.sc  	*******/
/************	Hanyu Yin	*******/
/************	05/11/2000	*******/

// 01/09/02 RD	fixed port access violations


#include "const.sh"

import "global";
import "chann";

#define LS(r,s) ((r) << (s))
#define RS(r,s) ((r) >> (s))


behavior DCT(int HData[BLOCKSIZE], int DData[BLOCKSIZE])
{  
  int i = 0;
  int j, k;

  int r_ram1, r_ram2 ;
  int r_alu, r_sh, r_mul, r_div ;	
  int rf[32] ;
  
  void main(void) 
  {
          fsmd(4u) 
          {
			////////////////////////////////////////////
			//	state PD_S1
			////////////////////////////////////////////
			PD_S1: {
				if (i < 64) 
					goto PD_S2;
				else
					goto PD_S5;
				}
			////////////////////////////////////////////
			//	state PD_S2
			////////////////////////////////////////////
			PD_S2: {
				r_ram1 = HData[i];
				goto PD_S3;
				}
			////////////////////////////////////////////
			//	state PD_S3
			////////////////////////////////////////////
			PD_S3: {
				r_alu = r_ram1 - 128;
				goto PD_S4;
				}
			////////////////////////////////////////////
			//	state PD_S4
			////////////////////////////////////////////
			PD_S4: {
				HData[i] = r_alu;
				i = i+1;
				goto PD_S1;
				}
			////////////////////////////////////////////
			//	state PD_S5
			////////////////////////////////////////////
			PD_S5: {
				i = 0;
				goto CDCT_S1;
				}
			////////////////////////////////////////////
			//	state CDCT_S1
			////////////////////////////////////////////
			CDCT_S1: {
				if (i<8)
					goto CDCT_S2; 
				else
					goto CDCT_S72; 
				}

			////////////////////////////////////////////
			//	state CDCT_S2
			////////////////////////////////////////////
			CDCT_S2: {
				rf[0]=i;
				r_alu = i+56;
				goto CDCT_S3; 
				} 

			////////////////////////////////////////////
			//	state CDCT_S3
			////////////////////////////////////////////
			CDCT_S3: {
				rf[1]=r_alu;
				goto CDCT_S4; 
				}


			////////////////////////////////////////////
			//	state CDCT_S4
			////////////////////////////////////////////
			CDCT_S4: {
				r_ram1 = HData[rf[0]];
				r_ram2 = HData[rf[1]];	
				goto CDCT_S5;
				}
			////////////////////////////////////////////
			//	state CDCT_S5
			////////////////////////////////////////////
			CDCT_S5: {
				r_alu = r_ram1+r_ram2;
				goto CDCT_S6;
				}
				
			////////////////////////////////////////////
			//	state CDCT_S6
			////////////////////////////////////////////
			CDCT_S6: {
				r_sh = LS(r_alu,2);
				r_alu = r_ram1-r_ram2;
				goto CDCT_S7;
				}

			////////////////////////////////////////////
			//	state CDCT_S7
			////////////////////////////////////////////
			CDCT_S7: {
				rf[2]=r_sh;
				r_sh = LS(r_alu,2);
				goto CDCT_S8;
				} 

			////////////////////////////////////////////
			//	state CDCT_S8
			////////////////////////////////////////////
			CDCT_S8: {
				rf[13]=r_sh;
				goto CDCT_S9;
				}
			////////////////////////////////////////////
			//	state CDCT_S9
			////////////////////////////////////////////
			CDCT_S9: {
				rf[0]=rf[0]+8;
				goto CDCT_S10;
				}
			////////////////////////////////////////////
			//	state CDCT_S10
			////////////////////////////////////////////
			CDCT_S10: {
				rf[1]=rf[1]-8;
				goto CDCT_S11;
				}

			////////////////////////////////////////////-
			////////////////////////////////////////////-
			////////////////////////////////////////////-

			////////////////////////////////////////////
			//	state CDCT_S11
			////////////////////////////////////////////
			CDCT_S11: {
				r_ram1 = HData[rf[0]];
				r_ram2 = HData[rf[1]];	
				goto CDCT_S12;
				}

			////////////////////////////////////////////
			//	state CDCT_S12
			////////////////////////////////////////////
			CDCT_S12: {
				r_alu = r_ram1+r_ram2;
				goto CDCT_S13;
				}
				
			////////////////////////////////////////////
			//	state CDCT_S13
			////////////////////////////////////////////
			CDCT_S13: {
				r_sh = LS(r_alu,2);
				r_alu = r_ram1-r_ram2;
				goto CDCT_S14;
				}

			////////////////////////////////////////////
			//	state CDCT_S14
			////////////////////////////////////////////
			CDCT_S14: {
				rf[3]=r_sh;
				r_sh = LS(r_alu,2);
				goto CDCT_S15;
				}
			////////////////////////////////////////////
			//	state CDCT_S15
			////////////////////////////////////////////
			CDCT_S15: {
				rf[12]=r_sh;
				goto CDCT_S16;
				}

			////////////////////////////////////////////
			//	state CDCT_S16
			////////////////////////////////////////////
			CDCT_S16: {
				rf[0]=rf[0]+8;
				goto CDCT_S17;
				}
	
			////////////////////////////////////////////
			//	state CDCT_S17
			////////////////////////////////////////////
			CDCT_S17: {
				rf[1]=rf[1]-8;
				goto CDCT_S18;
				}

			////////////////////////////////////////////
			////////////////////////////////////////////
			////////////////////////////////////////////
			//	state CDCT_S18
			////////////////////////////////////////////
			CDCT_S18: {
				r_ram1 = HData[rf[0]];
				r_ram2 = HData[rf[1]];	
				goto CDCT_S19;
				}
	
			////////////////////////////////////////////
			//	state CDCT_S19
			////////////////////////////////////////////
			CDCT_S19: {
				r_alu = r_ram1+r_ram2;
				goto CDCT_S20;
				}
				
			////////////////////////////////////////////
			//	state CDCT_S20
			////////////////////////////////////////////
			CDCT_S20: {
				r_sh = LS(r_alu,2);
				r_alu = r_ram1-r_ram2;
				goto CDCT_S21;
				}

			////////////////////////////////////////////
			//	state CDCT_S21
			////////////////////////////////////////////
			CDCT_S21: {
				rf[4]=r_sh;
				r_sh = LS(r_alu,2);
				goto CDCT_S22;
				}

			////////////////////////////////////////////
			//	state CDCT_S22
			////////////////////////////////////////////
			CDCT_S22: {
				rf[11]=r_sh;
				goto CDCT_S23;
				}

			////////////////////////////////////////////
			//	state CDCT_S23
			////////////////////////////////////////////
			CDCT_S23: {
				rf[0]=rf[0]+8;
				goto CDCT_S24;
				}

			////////////////////////////////////////////
			//	state CDCT_S24
			////////////////////////////////////////////
			CDCT_S24: {
				rf[1] = rf[1] - 8 ;
				goto CDCT_S25;
				}

			////////////////////////////////////////////
			////////////////////////////////////////////
			////////////////////////////////////////////
			//	state CDCT_S25
			////////////////////////////////////////////
			CDCT_S25: {
				r_ram1 = HData[rf[0]];
				r_ram2 = HData[rf[1]];	
				goto CDCT_S26;
				}

			////////////////////////////////////////////
			//	state CDCT_S26
			////////////////////////////////////////////
			CDCT_S26: {
				r_alu = r_ram1+r_ram2;
				goto CDCT_S27;
				}
				
			////////////////////////////////////////////
			//	state CDCT_S27
			////////////////////////////////////////////
			CDCT_S27: {
				r_sh = LS(r_alu,2);
				r_alu = r_ram1-r_ram2;
				goto CDCT_S28;
				}

			////////////////////////////////////////////
			//	state CDCT_S28
			////////////////////////////////////////////
			CDCT_S28: {
				rf[5]=r_sh;
				r_sh = LS(r_alu,2);
				goto CDCT_S29;
				}

			////////////////////////////////////////////
			//	state CDCT_S29
			////////////////////////////////////////////
			CDCT_S29: {
				rf[10]=r_sh;
				r_alu = rf[2]+rf[5];
				goto CDCT_S30;
				}

			////////////////////////////////////////////
			//	state CDCT_S30
			////////////////////////////////////////////
			CDCT_S30: {
				rf[6]=r_alu;
				r_alu = rf[3]+rf[4];
				goto CDCT_S31;
				}

			////////////////////////////////////////////
			//	state CDCT_S31
			////////////////////////////////////////////
			CDCT_S31: {
				rf[7]=r_alu;
				r_alu = rf[3]-rf[4];
				goto CDCT_S32;
				}
			////////////////////////////////////////////
			//	state CDCT_S32
			////////////////////////////////////////////
			CDCT_S32: {
				rf[8]=r_alu;
				r_alu = rf[2]-rf[5];
				goto CDCT_S33;
				}
			////////////////////////////////////////////
			//	state CDCT_S33
			////////////////////////////////////////////
			CDCT_S33: {
				rf[9]=r_alu;
				goto CDCT_S34;
				}
			////////////////////////////////////////////
			//	state CDCT_S34
			////////////////////////////////////////////
			CDCT_S34: {
				rf[0]=i;	
				goto CDCT_S35;
				}

			////////////////////////////////////////////
			//	state CDCT_S35
			////////////////////////////////////////////
			CDCT_S35: {
				r_alu = rf[6]+rf[7];
				goto CDCT_S36;
				}

			////////////////////////////////////////////
			//	state CDCT_S36
			////////////////////////////////////////////
			CDCT_S36: {
				r_mul = 362*r_alu;
				r_alu = rf[6]-rf[7];
				goto CDCT_S37;
				}

			////////////////////////////////////////////
			//	state CDCT_S37
			////////////////////////////////////////////
			CDCT_S37: {
				r_sh = RS(r_mul,9);
				r_mul = 362*r_alu;
				goto CDCT_S38;
				}

			////////////////////////////////////////////
			//	state CDCT_S38
			////////////////////////////////////////////
			CDCT_S38: {
				DData[rf[0]] =r_sh;	
				r_sh = RS(r_mul,9);
				goto CDCT_S39;
				}

			////////////////////////////////////////////
			//	state CDCT_S39
			////////////////////////////////////////////
			CDCT_S39: {
				r_alu = i+32;
				goto CDCT_S40;
				}

			////////////////////////////////////////////
			//	state CDCT_S40
			////////////////////////////////////////////
			CDCT_S40: {
				DData[r_alu] =r_sh;
				goto CDCT_S41;
				}

			////////////////////////////////////////////
			//	state CDCT_S41
			////////////////////////////////////////////
			CDCT_S41: {
				r_mul = 196*rf[8];   
				goto CDCT_S42;
				}

			////////////////////////////////////////////
			//	state CDCT_S42
			////////////////////////////////////////////
			CDCT_S42: {
				rf[15]=r_mul;
				r_mul = 473*rf[9];
				goto CDCT_S43;
				}

			////////////////////////////////////////////
			//	state CDCT_S43
			////////////////////////////////////////////
			CDCT_S43: {
				r_alu = r_mul+rf[15];
				r_mul = 196*rf[9];
				goto CDCT_S44;
				}

			////////////////////////////////////////////
			//	state CDCT_S44
			////////////////////////////////////////////
			CDCT_S44: {
				r_sh = RS(r_alu,9);
				rf[14]=r_mul;
				r_mul = 473*rf[8];
				goto CDCT_S45;
				}

			////////////////////////////////////////////
			//	state CDCT_S45
			////////////////////////////////////////////
			CDCT_S45: {
				rf[15]=r_sh;
				r_alu = rf[14]-r_mul;
				goto CDCT_S46;
				}

			////////////////////////////////////////////
			//	state CDCT_S46
			////////////////////////////////////////////
			CDCT_S46: {
				r_sh = RS(r_alu,9);
				r_alu = i+16;
				goto CDCT_S47;
				}


			////////////////////////////////////////////
			//	state CDCT_S47
			////////////////////////////////////////////
			CDCT_S47: {
				DData[r_alu] =rf[15]; 
				r_alu = i+48;
				goto CDCT_S48;
				}

			////////////////////////////////////////////
			//	state CDCT_S48
			////////////////////////////////////////////
			CDCT_S48: {
				DData[r_alu]=r_sh;
				goto CDCT_S49;
				}

			////////////////////////////////////////////
			//	state CDCT_S49
			////////////////////////////////////////////
			CDCT_S49: {
				r_alu = rf[12]-rf[11];
				goto CDCT_S50;
				}

			////////////////////////////////////////////
			//	state CDCT_S50
			////////////////////////////////////////////
			CDCT_S50: {
				r_mul = r_alu*362;
				r_alu = rf[12]+rf[11];
				goto CDCT_S51;
				}

			////////////////////////////////////////////
			//	state CDCT_S51
			////////////////////////////////////////////
			CDCT_S51: {
				r_sh = RS(r_mul, 9);
				r_mul = r_alu*362;
				goto CDCT_S52;
				}

			////////////////////////////////////////////
			//	state CDCT_S52
			////////////////////////////////////////////
			CDCT_S52: {
				rf[6]=r_sh;
				r_sh = RS(r_mul, 9);
				goto CDCT_S53;
				}

			////////////////////////////////////////////
			//	state CDCT_S53
			////////////////////////////////////////////
			CDCT_S53: {
				rf[7]=r_sh;
				r_alu = rf[10]+rf[6];
				goto CDCT_S54;
				}

			////////////////////////////////////////////
			//	state CDCT_S54
			////////////////////////////////////////////
			CDCT_S54: {
				rf[2] = r_alu ;
				r_alu = rf[10]-rf[6];
				goto CDCT_S55;
				}

			////////////////////////////////////////////
			//	state CDCT_S55
			////////////////////////////////////////////
			CDCT_S55: {	
				rf[3] = r_alu ;
				r_alu = rf[13]-rf[7];
				goto CDCT_S56;
				}

			////////////////////////////////////////////
			//	state CDCT_S56
			////////////////////////////////////////////
			CDCT_S56: {
				rf[4] = r_alu ;
				r_alu = rf[13]+rf[7];
				goto CDCT_S57;
				}

			////////////////////////////////////////////
			//	state CDCT_S57
			////////////////////////////////////////////
			CDCT_S57: {
				rf[5] = r_alu;
				r_mul = 100*rf[2];   
				goto CDCT_S58;
				}
			////////////////////////////////////////////
			////////////////////////////////////////////
			////////////////////////////////////////////

			////////////////////////////////////////////
			//	state CDCT_S58
			////////////////////////////////////////////
			CDCT_S58: {
				rf[15] = r_mul ;
				r_mul = 502*rf[5];
				goto CDCT_S59;
				}

			////////////////////////////////////////////
			//	state CDCT_S59
			////////////////////////////////////////////
			CDCT_S59: {
				r_alu = r_mul+rf[15];
				r_mul = 426*rf[4];
				goto CDCT_S60;
				}

			////////////////////////////////////////////
			//	state CDCT_S60
			////////////////////////////////////////////
			CDCT_S60: {
				r_sh = RS(r_alu,9);
				rf[14] = r_mul ;
				r_mul = 284*rf[3];
				goto CDCT_S61;
				}

			////////////////////////////////////////////
			//	state CDCT_S61
			////////////////////////////////////////////
			CDCT_S61: {
				rf[15] = r_sh;
				r_alu = rf[14]-r_mul;
				goto CDCT_S62;
				}

			////////////////////////////////////////////
			//	state CDCT_S62
			////////////////////////////////////////////
			CDCT_S62: {
				r_sh = RS(r_alu,9);
				r_alu = i+8 ;
				goto CDCT_S63;
				}


			////////////////////////////////////////////
			//	state CDCT_S63
			////////////////////////////////////////////
			CDCT_S63: {
				DData[r_alu] =rf[15]; 
				r_alu = i+24;
				goto CDCT_S64;
				}

			////////////////////////////////////////////
			//	state CDCT_S64
			////////////////////////////////////////////
			CDCT_S64: {
				DData[r_alu] = r_sh;
				r_mul = 426*rf[3];   
				goto CDCT_S65;
				}
			////////////////////////////////////////////
			////////////////////////////////////////////
			////////////////////////////////////////////
			////////////////////////////////////////////
			//	state CDCT_S65
			////////////////////////////////////////////
			CDCT_S65: {
				rf[15] = r_mul ;
				r_mul = 284*rf[4];
				goto CDCT_S66;
				}

			////////////////////////////////////////////
			//	state CDCT_S66
			////////////////////////////////////////////
			CDCT_S66: {
				r_alu = r_mul+rf[15];
				r_mul = 100*rf[5];
				goto CDCT_S67;
				}

			////////////////////////////////////////////
			//	state CDCT_S67
			////////////////////////////////////////////
			CDCT_S67: {
				r_sh = RS(r_alu,9);
				rf[14] = r_mul ;
				r_mul = 502*rf[2];
				goto CDCT_S68;
				}

			////////////////////////////////////////////
			//	state CDCT_S68
			////////////////////////////////////////////
			CDCT_S68: {
				rf[15] = r_sh;
				r_alu = rf[14]-r_mul;
				goto CDCT_S69;
				}

			////////////////////////////////////////////
			//	state CDCT_S69
			////////////////////////////////////////////
			CDCT_S69: {
				r_sh = RS(r_alu,9);
				r_alu = i+40 ;
				goto CDCT_S70;
				}

			////////////////////////////////////////////
			//	state CDCT_S70
			////////////////////////////////////////////
			CDCT_S70: {
				DData[r_alu] =rf[15]; 
				r_alu = i+56;
				goto CDCT_S71;
				}

			////////////////////////////////////////////
			//	state CDCT_S71
			////////////////////////////////////////////
			CDCT_S71: {
				DData[r_alu]=r_sh;
				i = i+1;
				goto CDCT_S1;
				}

			////////////////////////////////////////////
			//	state CDCT_S72
			////////////////////////////////////////////
			CDCT_S72: {
				i = 0;
				goto CDCT_S73;
				}

			////////////////////////////////////////////
			//	state CDCT_S73
			////////////////////////////////////////////
			CDCT_S73: {
				if (i<8) 
					goto CDCT_S74;
				else
					goto CDCT_S139; 
				}
	
			////////////////////////////////////////////
			//	state CDCT_S74
			////////////////////////////////////////////
			CDCT_S74: {
				r_sh = LS(i,3);
				goto CDCT_S75;
				}

			////////////////////////////////////////////
			//	state CDCT_S75
			////////////////////////////////////////////
			CDCT_S75: {
				j = r_sh;
				r_alu = r_sh+7;
				goto CDCT_S76;
				}

			////////////////////////////////////////////
			//	state CDCT_S76
			////////////////////////////////////////////
			CDCT_S76: {
				k = r_alu;
				goto CDCT_S77;
				}

			////////////////////////////////////////////
			//	state CDCT_S77
			////////////////////////////////////////////
			CDCT_S77: {
				r_ram1 = DData[j];
				r_ram2 = DData[k];
				goto CDCT_S78;
				}

			////////////////////////////////////////////
			//	state CDCT_S78
			////////////////////////////////////////////
			CDCT_S78: {
				r_alu = r_ram1-r_ram2;
				goto CDCT_S79;
				}

			////////////////////////////////////////////
			//	state CDCT_S79
			////////////////////////////////////////////
			CDCT_S79: {
				r_sh = RS(r_alu,1);
				r_alu = r_ram1 + r_ram2 ;
				j = j+1;
				k = k-1;
				goto CDCT_S80;
				}

			////////////////////////////////////////////
			//	state CDCT_S80
			////////////////////////////////////////////
			CDCT_S80: {
				rf[13] = r_sh;
				r_sh = RS(r_alu,1);
				goto CDCT_S81;
				}

			////////////////////////////////////////////
			//	state CDCT_S81
			////////////////////////////////////////////
			CDCT_S81: {
				rf[2]=r_sh;
				goto CDCT_S82;
				}
		        ////////////////////////////////////////////
		        ////////////////////////////////////////////

			////////////////////////////////////////////
			//	state CDCT_S82
			////////////////////////////////////////////
			CDCT_S82: {
				r_ram1 = DData[j];
				r_ram2 = DData[k];
				goto CDCT_S83;
				}

			////////////////////////////////////////////
			//	state CDCT_S83
			////////////////////////////////////////////
			CDCT_S83: {
				r_alu = r_ram1-r_ram2;
				goto CDCT_S84;
				}

			////////////////////////////////////////////
			//	state CDCT_S84
			////////////////////////////////////////////
			CDCT_S84: {
				r_sh = RS(r_alu, 1) ;
				r_alu = r_ram1+r_ram2;
				j = j+1;
				k = k-1;
				goto CDCT_S85;
				}

			////////////////////////////////////////////
			//	state CDCT_S85
			////////////////////////////////////////////
			CDCT_S85: {
				rf[12] = r_sh;
				r_sh = RS(r_alu,1);
				goto CDCT_S86;
				}

			////////////////////////////////////////////
			//	state CDCT_S86
			////////////////////////////////////////////
			CDCT_S86: {
				rf[3] = r_sh;
				goto CDCT_S87;
				}

			////////////////////////////////////////////
			////////////////////////////////////////////
			//	state CDCT_S87
			////////////////////////////////////////////
			CDCT_S87: {
				r_ram1 = DData[j];
				r_ram2 = DData[k];
				goto CDCT_S88;
				}

			////////////////////////////////////////////
			//	state CDCT_S88
			////////////////////////////////////////////
			CDCT_S88: {
				r_alu = r_ram1-r_ram2;
				goto CDCT_S89;
				}

			////////////////////////////////////////////
			//	state CDCT_S89
			////////////////////////////////////////////
			CDCT_S89: {
				r_sh = RS(r_alu, 1);
				r_alu = r_ram1+r_ram2;
				j = j+1;
				k = k-1;
				goto CDCT_S90;
				}

			////////////////////////////////////////////
			//	state CDCT_S90
			////////////////////////////////////////////
			CDCT_S90: {
				rf[11] = r_sh;
				r_sh = RS(r_alu,1);
				goto CDCT_S91;
				}

			////////////////////////////////////////////
			//	state CDCT_S91
			////////////////////////////////////////////
			CDCT_S91: {
				rf[4] = r_sh;
				goto CDCT_S92;
				}

			////////////////////////////////////////////
			//	state CDCT_S92
			////////////////////////////////////////////
			CDCT_S92: {
				r_ram1 = DData[j];
				r_ram2 = DData[k];
				goto CDCT_S93;
				}
    
			////////////////////////////////////////////
			//	state CDCT_S93
			////////////////////////////////////////////
			CDCT_S93: {
				r_alu = r_ram1-r_ram2;
				goto CDCT_S94;
				}

			////////////////////////////////////////////
			//	state CDCT_S94
			////////////////////////////////////////////
			CDCT_S94: {
				r_sh = RS(r_alu, 1) ;
				r_alu = r_ram1+r_ram2;
				j = j+1;
				k = k-1;
				goto CDCT_S95;
				}

			////////////////////////////////////////////
			//	state CDCT_S95
			////////////////////////////////////////////
			CDCT_S95: {
				rf[10] = r_sh;
				r_sh = RS(r_alu,1);
				goto CDCT_S96;
				}

			////////////////////////////////////////////
			//	state CDCT_S96
			////////////////////////////////////////////
			CDCT_S96: {
				rf[5] = r_sh;
				r_alu = rf[2] + r_sh;
				goto CDCT_S97;
				}

			////////////////////////////////////////////
			//	state CDCT_S97
			////////////////////////////////////////////
			CDCT_S97: {
				rf[6] = r_alu;
				r_alu = rf[3]+rf[4];
				goto CDCT_S98;
				}


			////////////////////////////////////////////
			//	state CDCT_S98
			////////////////////////////////////////////
			CDCT_S98: {
				rf[7] = r_alu;
				r_alu = rf[3]-rf[4];
				goto CDCT_S99;
				}

			////////////////////////////////////////////
			//	state CDCT_S99
			////////////////////////////////////////////
			CDCT_S99: {
				rf[8] = r_alu;
				r_alu = rf[2]-rf[5];
				goto CDCT_S100;
				}


			////////////////////////////////////////////
			//	state CDCT_S100
			////////////////////////////////////////////
			CDCT_S100: {
				rf[9] = r_alu;
				r_sh = LS(i,3);
				goto CDCT_S101;
				}

			////////////////////////////////////////////
			//	state CDCT_S101
			////////////////////////////////////////////
			CDCT_S101: {
				rf[0] = r_sh;
				goto CDCT_S102;
				}

			////////////////////////////////////////////
			//	state CDCT_S102
			////////////////////////////////////////////
			CDCT_S102: {
				r_alu = rf[6]+rf[7];
				goto CDCT_S103;
				}

			////////////////////////////////////////////
			//	state CDCT_S103
			////////////////////////////////////////////
			CDCT_S103: {
				r_mul = 362 * r_alu ;
				r_alu = rf[6]-rf[7];
				goto CDCT_S104;
				}

			////////////////////////////////////////////
			//	state CDCT_S104
			////////////////////////////////////////////
			CDCT_S104: {
				r_sh = RS(r_mul,9);
				r_mul = 362*r_alu;
				goto CDCT_S105;
				}


			////////////////////////////////////////////
			//	state CDCT_S105
			////////////////////////////////////////////
			CDCT_S105: {
				DData[rf[0]] = r_sh ;
				r_sh = RS(r_mul,9);
				goto CDCT_S106;
				}

			////////////////////////////////////////////
			//	state CDCT_S106
			////////////////////////////////////////////
			CDCT_S106: {
				r_alu = rf[0]+4;
				goto CDCT_S107;
				}

			////////////////////////////////////////////
			//	state CDCT_S107
			////////////////////////////////////////////
			CDCT_S107: {
				DData[r_alu] =r_sh;
				goto CDCT_S108;
				}

			////////////////////////////////////////////
			//	state CDCT_S108
			////////////////////////////////////////////
			CDCT_S108: {
				r_mul = 196*rf[8];   
				goto CDCT_S109;
				}

			////////////////////////////////////////////
			//	state CDCT_S109
			////////////////////////////////////////////
			CDCT_S109: {
				rf[15] = r_mul ;
				r_mul = 473*rf[9];
				goto CDCT_S110;
				}

			////////////////////////////////////////////
			//	state CDCT_S110
			////////////////////////////////////////////
			CDCT_S110: {
				r_alu = r_mul+rf[15];
				r_mul = 196*rf[9];
				goto CDCT_S111;
				}

			////////////////////////////////////////////
			//	state CDCT_S111
			////////////////////////////////////////////
			CDCT_S111: {
				r_sh = RS(r_alu,9);
				rf[14] = r_mul ;
				r_mul = 473*rf[8];
				goto CDCT_S112;
				}

			////////////////////////////////////////////
			//	state CDCT_S112
			////////////////////////////////////////////
			CDCT_S112: {
				rf[15] = r_sh;
				r_alu = rf[14]-r_mul;
				goto CDCT_S113;
				}

			////////////////////////////////////////////
			//	state CDCT_S113
			////////////////////////////////////////////
			CDCT_S113: {
				r_sh = RS(r_alu, 9) ;
				r_alu = rf[0]+2;
				goto CDCT_S114;
				}


			////////////////////////////////////////////
			//	state CDCT_S114
			////////////////////////////////////////////
			CDCT_S114: {
				DData[r_alu] =rf[15]; 
				r_alu = rf[0]+6;
				goto CDCT_S115;
				}

			////////////////////////////////////////////
			//	state CDCT_S115
			////////////////////////////////////////////
			CDCT_S115: {
				DData[r_alu]=r_sh;
				goto CDCT_S116;
				}


			////////////////////////////////////////////
			//	state CDCT_S116
			////////////////////////////////////////////
			CDCT_S116: {
				r_alu = rf[12]-rf[11];
				goto CDCT_S117;
				}

			////////////////////////////////////////////
			//	state CDCT_S117
			////////////////////////////////////////////
			CDCT_S117: {
				r_mul = r_alu * 362 ;
				r_alu = rf[12]+rf[11];
				goto CDCT_S118;
				}

			////////////////////////////////////////////
			//	state CDCT_S118
			////////////////////////////////////////////
			CDCT_S118: {
				r_sh = RS(r_mul, 9);
				r_mul = r_alu*362;
				goto CDCT_S119;
				}

			////////////////////////////////////////////
			//	state CDCT_S119
			////////////////////////////////////////////
			CDCT_S119: {
				rf[6] = r_sh ;
				r_sh = RS(r_mul, 9);
				goto CDCT_S120;
				}

			////////////////////////////////////////////
			//	state CDCT_S120
			////////////////////////////////////////////
			CDCT_S120: {
				rf[7]=r_sh;
				r_alu = rf[10]+rf[6];
				goto CDCT_S121;
				}

			////////////////////////////////////////////
			//	state CDCT_S121
			////////////////////////////////////////////
			CDCT_S121: {
				rf[2] = r_alu ;
				r_alu = rf[10]-rf[6];
				goto CDCT_S122;
				}

			////////////////////////////////////////////
			//	state CDCT_S122
			////////////////////////////////////////////
			CDCT_S122: {
				rf[3] = r_alu ;
				r_alu = rf[13]-rf[7];
				goto CDCT_S123;
				}

			////////////////////////////////////////////
			//	state CDCT_S123
			////////////////////////////////////////////
			CDCT_S123: {
				rf[4] = r_alu ;
				r_alu = rf[13]+rf[7];
				goto CDCT_S124;
				}

			////////////////////////////////////////////
			//	state CDCT_S124
			////////////////////////////////////////////
			CDCT_S124: {
				rf[5]=r_alu;
				r_mul = 100*rf[2];   
				goto CDCT_S125;
				}

			////////////////////////////////////////////
			//	state CDCT_S125
			////////////////////////////////////////////
			CDCT_S125: {
				rf[15] = r_mul ;
				r_mul = 502*rf[5];
				goto CDCT_S126;
				}

			////////////////////////////////////////////
			//	state CDCT_S126
			////////////////////////////////////////////
			CDCT_S126: {
				r_alu = r_mul+rf[15];
				r_mul = 426*rf[4];
				goto CDCT_S127;
				}

			////////////////////////////////////////////
			//	state CDCT_S127
			////////////////////////////////////////////
			CDCT_S127: {
				r_sh = RS(r_alu,9);
				rf[14] = r_mul ;
				r_mul = 284*rf[3];
				goto CDCT_S128;
				}

			////////////////////////////////////////////
			//	state CDCT_S128
			////////////////////////////////////////////
			CDCT_S128: {
				rf[15] = r_sh;
				r_alu = rf[14]-r_mul;
				goto CDCT_S129;
				}

			////////////////////////////////////////////
			//	state CDCT_S129
			////////////////////////////////////////////
			CDCT_S129: {
				r_sh = RS(r_alu,9);
				r_alu = rf[0] + 1 ;
				goto CDCT_S130;
				}

			////////////////////////////////////////////
			//	state CDCT_S130
			////////////////////////////////////////////
			CDCT_S130: {
				DData[r_alu] =rf[15]; 
				r_alu = rf[0]+3;
				goto CDCT_S131;
				}

			////////////////////////////////////////////
			//	state CDCT_S131
			////////////////////////////////////////////
			CDCT_S131: {
				DData[r_alu]=r_sh;
				r_mul = 426*rf[3];   
				goto CDCT_S132;
				}
			////////////////////////////////////////////
			////////////////////////////////////////////
			////////////////////////////////////////////
			////////////////////////////////////////////
			//	state CDCT_S132
			////////////////////////////////////////////
			CDCT_S132: {
				rf[15] = r_mul ;
				r_mul = 284*rf[4];
				goto CDCT_S133;
				}

			////////////////////////////////////////////
			//	state CDCT_S133
			////////////////////////////////////////////
			CDCT_S133: {
				r_alu = r_mul+rf[15];
				r_mul = 100*rf[5];
				goto CDCT_S134;
				}

			////////////////////////////////////////////
			//	state CDCT_S134
			////////////////////////////////////////////
			CDCT_S134: {
				r_sh = RS(r_alu,9);
				rf[14] = r_mul ;
				r_mul = 502*rf[2];
				goto CDCT_S135;
				}

			////////////////////////////////////////////
			//	state CDCT_S135
			////////////////////////////////////////////
			CDCT_S135: {
				rf[15] = r_sh;
				r_alu = rf[14]-r_mul;
				goto CDCT_S136;
				}

			////////////////////////////////////////////
			//	state CDCT_S136
			////////////////////////////////////////////
			CDCT_S136: {
				r_sh = RS(r_alu, 9) ;
				r_alu = rf[0]+5;
				goto CDCT_S137;
				}

			////////////////////////////////////////////
			//	state CDCT_S137
			////////////////////////////////////////////
			CDCT_S137: {
				DData[r_alu] =rf[15]; 
				r_alu = rf[0]+7;
				goto CDCT_S138;
				}

			////////////////////////////////////////////
			//	state CDCT_S138
			////////////////////////////////////////////
			CDCT_S138: {
				DData[r_alu]=r_sh;
				i = i+1;
				goto CDCT_S73;
				}

			////////////////////////////////////////////
			//	state CDCT_S139
			////////////////////////////////////////////
			CDCT_S139: {
				i = 0;
				goto CDCT_S140;
				}

			////////////////////////////////////////////
			//	state CDCT_S140
			////////////////////////////////////////////
			CDCT_S140: {
				if(i<64) 
					goto CDCT_S141;
				else
					goto BD_S0;
				}

			////////////////////////////////////////////
			//	state CDCT_S141
			////////////////////////////////////////////
			CDCT_S141: {
				r_ram1 = DData[i];
				goto CDCT_S142;
				}


			////////////////////////////////////////////
			//	state CDCT_S142
			////////////////////////////////////////////
			CDCT_S142: {
				if(r_ram1<0) 
					goto CDCT_S146;
				else
					goto CDCT_S147;
				}
				
			////////////////////////////////////////////
			//	state CDCT_S146
			////////////////////////////////////////////
			CDCT_S146: {
				r_alu = r_ram1-4;
				goto CDCT_S143;
				}

			////////////////////////////////////////////
			//	state CDCT_S147
			////////////////////////////////////////////
			CDCT_S147: {
				r_alu = r_ram1+4;
				goto CDCT_S143;
				}
				
			////////////////////////////////////////////

			//	state CDCT_S143
			////////////////////////////////////////////
			CDCT_S143: {
				r_div = r_alu/8;
				goto CDCT_S144;
				}

			////////////////////////////////////////////
			//	state CDCT_S144
			////////////////////////////////////////////
			CDCT_S144: {
				DData[i]=r_div;
				i = i+1;
				goto CDCT_S140;
				}
			
			////////////////////////////////////////////
			//	state BD_S0
			////////////////////////////////////////////
			BD_S0: {
				i = 0;
				goto BD_S1;
				}

			////////////////////////////////////////////
			//	state BD_S1
			////////////////////////////////////////////
			BD_S1: {
				if (i < 64) 
					goto BD_S2;
				else
					goto CDCT_S145;
				}

			////////////////////////////////////////////
			//	state BD_S2
			////////////////////////////////////////////
			BD_S2: {
				r_ram1 = DData[i];
				goto BD_S3;
				}

			////////////////////////////////////////////
			//	state BD_S3
			////////////////////////////////////////////
			BD_S3: {
				if (r_ram1 < -1023)  goto BD_S4;
				else goto BD_S5;
				}

			////////////////////////////////////////////
			//	state BD_S4
			////////////////////////////////////////////
			BD_S4: {
				DData[i] = -1023;
				i = i+1;
				goto BD_S1;
				}

			////////////////////////////////////////////
			//	state BD_S5
			////////////////////////////////////////////
			BD_S5: {
				if (r_ram1 > 1023)  goto BD_S6;
				else goto BD_S7;
				}

			////////////////////////////////////////////
			//	state BD_S6
			////////////////////////////////////////////
			BD_S6: {
				DData[i] = 1023;
				goto BD_S7;
				}

			////////////////////////////////////////////
			//	state BD_S7
			////////////////////////////////////////////
			BD_S7: {
				i = i+1;
				goto BD_S1;
				}

			////////////////////////////////////////////
			//	state CDCT_S145
			////////////////////////////////////////////
			CDCT_S145: {
				i = 0;
                                break;
				}                        
    
          }
  }
};
