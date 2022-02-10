/*!
    LTC6946: Ultralow Noise and Spurious 0.37GHz to 6.39GHz IntegerN Synthesizer with Integrated VCO

@verbatim

The LTC®6946 is a high performance, low noise, 6.39GHz
phase-locked loop (PLL) with a fully integrated VCO,
including a reference divider, phase-frequency detector
(PFD) with phase-lock indicator, ultralow noise charge
pump, integer feedback divider, and VCO output divider.
The charge pump contains selectable high and low voltage
clamps useful for VCO monitoring.

The integrated low noise VCO uses no external components.
It is internally calibrated to the correct output frequency
with no external system support.

The part features a buffered, programmable VCO output
divider with a range of 1 through 6, providing a wide
frequency range.

@endverbatim


http://www.linear.com/product/LTC6946

http://www.linear.com/product/LTC6946#demoboards

REVISION HISTORY
$Revision: 3018 $
$Date: 2014-12-01 15:53:20 -0800 (Mon, 01 Dec 2014) $

Copyright (c) 2013, Linear Technology Corp.(LTC)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of Linear Technology Corp.

The Linear Technology Linduino is not affiliated with the official Arduino team.
However, the Linduino is only possible because of the Arduino team's commitment
to the open-source community.  Please, visit http://www.arduino.cc and
http://store.arduino.cc , and consider a purchase that will help fund their
ongoing work.
*/

//! @defgroup LTC6946 LTC6946: Ultralow Noise and Spurious 0.37GHz to 6.39GHz IntegerN Synthesizer with Integrated VCO

/*! @file
    @ingroup LTC6946
    Library for LTC6946: Ultralow Noise and Spurious 0.37GHz to 6.39GHz IntegerN Synthesizer with Integrated VCO
*/

#include <stdint.h>
//include <Arduino.h>
#include "Linduino.h"
//#include "UserInterface.h"
#include "LT_SPI.h"
#include "LTC6946.h"
//#include <SPI.h>

uint8_t LTC6946_reg[LTC6946_NUM_REGADDR];               //!< number of LTC6946 spi addresses  
uint8_t LTC6946_spi_map[(LTC6946_NUM_REGFIELD+1)][4];   //!< LTC6946 spi map, stores MSB address location, MSB bit location, field length in bits, and R or RW capability

unsigned long LTC6946_Fref_MHz = 100;                   //!< Default Fref frequency - MHz portion (xxx);  Fref = xxx, yyy,yyy
unsigned long LTC6946_Fref_Hz = 0;                      //!< Default Fref frequency - Hz portion (yyy,yyy);  Fref = x,xxx, yyy,yyy
unsigned long LTC6946_Frf_MHz = 900;                    //!< Default Frf frequency - MHz portion (xxxx);  Frf = x,xxx, yyy,yyy
unsigned long LTC6946_Frf_Hz  = 0;                      //!< Default Frf frequency - Hz portion (yyy,yyy);  Frf = x,xxx, yyy,yyy

//unsigned long zero64[]={0,0}; //!< for 64bit math functions

/* -------------------------------------------------------------------------
  FUNCTION: LTC6946_read
  - reads 8 bit Data field to LTC6946.
  - has to shift data by one bit to account for RW bit
 -------------------------------------------------------------------------- */
uint8_t LTC6946_read(int8_t address) {
int8_t address_shift;
//LT_union_int16_2bytes rx;
	  union
  {
    uint8_t b[2];
    uint16_t w;
  } rx;
  
address_shift =(address << 1) | 0x01; // shift to left to account for R/W bit, set bit high for read
spi_transfer_word(address_shift<<8 , &rx.w);
  
LTC6946_reg[address]=rx.b[0];
return(rx.b[0]);
}


/* -------------------------------------------------------------------------
  FUNCTION: LTC6946_read_field
  For SPI FIELDS located in 1 or multiple address location
  - reads specific address locations
  - identifies and returns specific field in question
    - can handle SPI fields in multiple addresses, if MSB bit is in the lower number address
--------------------------------------------------------------------------- */
 long LTC6946_read_field( uint8_t address, uint8_t MSB_loc, uint8_t numbits){
 int bit_shift, i, num_reg;
 long field_val, maskbits, pow2;

num_reg=0;
field_val=0;
// determines how many register are used
do {
   bit_shift = (MSB_loc+1)- (numbits-num_reg*8);   // determines bit_shift for last register location
   field_val=LTC6946_read((address+num_reg))+(field_val<<8);  // reads current address locations, shifts previous address location 8 bits
   num_reg++;
}while ((bit_shift<0) && (num_reg<4));
  
// creates a bit mask for complete word, 
maskbits = 1;
pow2=1;
for(i=1, maskbits=1;i<numbits; i++) {
   pow2=pow2*2;
   maskbits = maskbits+pow2;
   }
 
field_val=(field_val >>bit_shift) &maskbits; 
return field_val;
}

/* -------------------------------------------------------------------------
  FUNCTION: get_LTC6946_SPI_FIELD
  For SPI FIELDS
  - reads specific address locations
  - identifies and returns specific field in question
    - can handle SPI fields in multiple addresses, if MSB bit is in the lower number address
--------------------------------------------------------------------------- */
long get_LTC6946_SPI_FIELD(uint8_t f) {
 
return LTC6946_read_field(LTC6946_spi_map[f][ADDRx], LTC6946_spi_map[f][DxMSB], LTC6946_spi_map[f][NUMBITS]);
}
 
/* -------------------------------------------------------------------------
  FUNCTION: LTC6946_write
  - writes 8 bit Data field to LTC6946.
  - has to shift data by one bit to account for RW bit
--------------------------------------------------------------------------- */
void LTC6946_write( uint8_t address, uint8_t Data) {
//LT_union_int16_2bytes rx;
	
  union
  {
    uint8_t b[2];
    uint16_t w;
  } rx;	
  
address=address << 1; // shift to left to account for R/W bit
spi_transfer_word((address<<8) | Data, &rx.w);
}


 /* -------------------------------------------------------------------------
  FUNCTION: LTC6946_write_field
  For SPI FIELDS
  - reads specific address location
  - identifies and returns specific field in question
    - can handle SPI fields in multiple addresses, if MSB bit is in the lower number address
---------------------------------------------------------------------------- */
uint8_t LTC6946_write_field( long field_data, uint8_t address, uint8_t MSB_loc, uint8_t numbits){
long current_content, desired_content, reg_val;
int LSB_loc, i, j, num_reg, bit_shift;
long temp_arr[32];
  
for(i=0; i<32 ; i++) temp_arr[i]=0;   // init temp_arr
  
// read data in current address location and put in a bit array
num_reg=0;
current_content=0;
do {
   bit_shift=(MSB_loc+1)-(numbits-num_reg*8);
   current_content=LTC6946_read((address+num_reg)) + (current_content<<8);

   num_reg++;
} while((bit_shift<0) && (num_reg<4));
for(i=0; i<(8*num_reg); i++) {temp_arr[i]=(current_content>>i) & 1; }
  
// exchange current bits with desired bits
LSB_loc = 8*(num_reg-1)+MSB_loc-numbits+1;
for(i=LSB_loc, j=0; i<=(MSB_loc+(num_reg-1)*8); i++, j++){
   temp_arr[i] = (field_data>>j) &1;
   } // end of for loop
    
// reconstruct bits into an integer
desired_content = 0;
for(i=0; i<(8*num_reg); i++) {
   desired_content = desired_content | (temp_arr[i]<<i);
   } // end of for loop
    
// write new field value to part
for(i=0; i<num_reg; i++) {
   reg_val = (desired_content >> 8*(num_reg-1-i)) & 0xff;
   LTC6946_write((address+i), reg_val);
   } // end of for loop
return 1;
} // end of LTC6946_write_field
    
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
 
/* -------------------------------------------------------------------------
   FUNCTION: get_LTC6946_REGSIZE
   - returns # of addresses in parts register map (array size)
---------------------------------------------------------------------------- */
uint8_t get_LTC6946_REGSIZE(){
return sizeof(LTC6946_reg);
}
 
 
/* -------------------------------------------------------------------------
   FUNCTION: get_LTC6946_SPI_FIELD_NUMBITS
   - returns the number of bits for a given field name in the SPI map
---------------------------------------------------------------------------- */
uint8_t get_LTC6946_SPI_FIELD_NUMBITS(uint8_t f) {
return LTC6946_spi_map[f][NUMBITS];
}
 
 
/* -------------------------------------------------------------------------
   FUNCTION: get_LTC6946_SPI_FIELD_RW
   - returns if the given field name is (0)read/write or (1)read_only field
---------------------------------------------------------------------------- */
uint8_t get_LTC6946_SPI_FIELD_RW(uint8_t f) {
return LTC6946_spi_map[f][R_ONLY];
}
 
 
/* -------------------------------------------------------------------------
   FUNCTION: set_LTC6946_SPI_FIELD
   For SPI FIELDS 
   - reads specific address location
   - identifies and returns specific field in question
   - can handle SPI fields in multiple addresses, if MSB bit is in the lower number address
---------------------------------------------------------------------------- */
void set_LTC6946_SPI_FIELD(uint8_t f, long field_data) {
LTC6946_write_field( field_data, LTC6946_spi_map[f][ADDRx], LTC6946_spi_map[f][DxMSB], LTC6946_spi_map[f][NUMBITS]);
}
 

/* -------------------------------------------------------------------------
   FUNCTION: set_LTC6946_ALLREGS
   - writes data to all registers at once
--------------------------------------------------------------------------- */
void set_LTC6946_ALLREGS( uint8_t reg01, uint8_t reg02, uint8_t reg03, uint8_t reg04, uint8_t reg05, uint8_t reg06, uint8_t reg07, uint8_t reg08, uint8_t reg09, uint8_t reg0A) {
uint8_t i;
 
	LTC6946_reg[1] = reg01;
	LTC6946_reg[2] = reg02;
	LTC6946_reg[3] = reg03;
	LTC6946_reg[4] = reg04;
	LTC6946_reg[5] = reg05;
	LTC6946_reg[6] = reg06; 
	LTC6946_reg[7] = reg07;
	LTC6946_reg[8] = reg08;
	LTC6946_reg[9] = reg09;
	LTC6946_reg[10] = reg0A;
		 
	//for(i=1; i<11; i++)  LTC6946_write( i, LTC6946_reg[i]);

	// Adrian McKernan Block transfer intead of single registers	
		
	uint8_t LTC6946_tx[LTC6946_NUM_REGADDR-1];	
	LTC6946_tx[0] = 1<<1 | 0; // shift regioster 1 for 7 bit address and 0 for write
	for 	(i=1; i<11; i++) {
		LTC6946_tx[i] = LTC6946_reg[i];
	}
	uint8_t LTC6946_rx[LTC6946_NUM_REGADDR];
	
	spi_transfer_block(LTC6946_tx, LTC6946_rx, LTC6946_NUM_REGADDR-1);

} // end of set_LTC6946_ALLREGS
 

void LTC6946_initRegs(void){

	set_LTC6946_ALLREGS( H01, H02, H03, H04, H05, H06, H07, H08, H09, H0A);
}
 
/* -------------------------------------------------------------------------
   FUNCTION: LTC6946_init
   - initializes the SPI MAP
   - for ease of programming there is spreadsheet that automates this some.
----------------------------------------------------------------------------*/
void LTC6946_init() { 
   
// spi map    
LTC6946_spi_map[LTC6946_ALCCAL][ADDRx]=0x07;   LTC6946_spi_map[LTC6946_ALCCAL][DxMSB]= 5;   LTC6946_spi_map[LTC6946_ALCCAL][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_ALCEN][ADDRx]=0x07;    LTC6946_spi_map[LTC6946_ALCEN][DxMSB]= 7;    LTC6946_spi_map[LTC6946_ALCEN][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_ALCHI][ADDRx]=0x00;    LTC6946_spi_map[LTC6946_ALCHI][DxMSB]= 4;    LTC6946_spi_map[LTC6946_ALCHI][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_ALCLO][ADDRx]=0x00;    LTC6946_spi_map[LTC6946_ALCLO][DxMSB]= 3;    LTC6946_spi_map[LTC6946_ALCLO][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_ALCMON][ADDRx]=0x07;   LTC6946_spi_map[LTC6946_ALCMON][DxMSB]= 6;   LTC6946_spi_map[LTC6946_ALCMON][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_ALCULOK][ADDRx]=0x07;  LTC6946_spi_map[LTC6946_ALCULOK][DxMSB]= 4;  LTC6946_spi_map[LTC6946_ALCULOK][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_BD][ADDRx]=0x03;       LTC6946_spi_map[LTC6946_BD][DxMSB]= 7;       LTC6946_spi_map[LTC6946_BD][NUMBITS]= 4;
LTC6946_spi_map[LTC6946_BST][ADDRx]=0x08;      LTC6946_spi_map[LTC6946_BST][DxMSB]= 7;      LTC6946_spi_map[LTC6946_BST][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_CAL][ADDRx]=0x07;      LTC6946_spi_map[LTC6946_CAL][DxMSB]= 1;      LTC6946_spi_map[LTC6946_CAL][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_CP][ADDRx]=0x09;       LTC6946_spi_map[LTC6946_CP][DxMSB]= 3;       LTC6946_spi_map[LTC6946_CP][NUMBITS]= 4;
LTC6946_spi_map[LTC6946_CPCHI][ADDRx]=0x0a;    LTC6946_spi_map[LTC6946_CPCHI][DxMSB]= 7;    LTC6946_spi_map[LTC6946_CPCHI][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_CPCLO][ADDRx]=0x0a;    LTC6946_spi_map[LTC6946_CPCLO][DxMSB]= 6;    LTC6946_spi_map[LTC6946_CPCLO][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_CPDN][ADDRx]=0x0a;     LTC6946_spi_map[LTC6946_CPDN][DxMSB]= 0;     LTC6946_spi_map[LTC6946_CPDN][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_CPINV][ADDRx]=0x0a;    LTC6946_spi_map[LTC6946_CPINV][DxMSB]= 4;    LTC6946_spi_map[LTC6946_CPINV][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_CPMID][ADDRx]=0x0a;    LTC6946_spi_map[LTC6946_CPMID][DxMSB]= 5;    LTC6946_spi_map[LTC6946_CPMID][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_CPRST][ADDRx]=0x0a;    LTC6946_spi_map[LTC6946_CPRST][DxMSB]= 2;    LTC6946_spi_map[LTC6946_CPRST][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_CPUP][ADDRx]=0x0a;     LTC6946_spi_map[LTC6946_CPUP][DxMSB]= 1;     LTC6946_spi_map[LTC6946_CPUP][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_CPWIDE][ADDRx]=0x0a;   LTC6946_spi_map[LTC6946_CPWIDE][DxMSB]= 3;   LTC6946_spi_map[LTC6946_CPWIDE][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_FILT][ADDRx]=0x08;     LTC6946_spi_map[LTC6946_FILT][DxMSB]= 6;     LTC6946_spi_map[LTC6946_FILT][NUMBITS]= 2;
LTC6946_spi_map[LTC6946_LKCT][ADDRx]=0x09;     LTC6946_spi_map[LTC6946_LKCT][DxMSB]= 5;     LTC6946_spi_map[LTC6946_LKCT][NUMBITS]= 2;
LTC6946_spi_map[LTC6946_LKEN][ADDRx]=0x07;     LTC6946_spi_map[LTC6946_LKEN][DxMSB]= 0;     LTC6946_spi_map[LTC6946_LKEN][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_LKWIN][ADDRx]=0x09;    LTC6946_spi_map[LTC6946_LKWIN][DxMSB]= 7;    LTC6946_spi_map[LTC6946_LKWIN][NUMBITS]= 2;
LTC6946_spi_map[LTC6946_LOCK][ADDRx]=0x00;     LTC6946_spi_map[LTC6946_LOCK][DxMSB]= 2;     LTC6946_spi_map[LTC6946_LOCK][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_MTCAL][ADDRx]=0x02;    LTC6946_spi_map[LTC6946_MTCAL][DxMSB]= 2;    LTC6946_spi_map[LTC6946_MTCAL][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_ND][ADDRx]=0x05;       LTC6946_spi_map[LTC6946_ND][DxMSB]= 7;       LTC6946_spi_map[LTC6946_ND][NUMBITS]= 16;
LTC6946_spi_map[LTC6946_OD][ADDRx]=0x08;       LTC6946_spi_map[LTC6946_OD][DxMSB]= 2;       LTC6946_spi_map[LTC6946_OD][NUMBITS]= 3;
LTC6946_spi_map[LTC6946_OMUTE][ADDRx]=0x02;    LTC6946_spi_map[LTC6946_OMUTE][DxMSB]= 1;    LTC6946_spi_map[LTC6946_OMUTE][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_PART][ADDRx]=0x0b;     LTC6946_spi_map[LTC6946_PART][DxMSB]= 4;     LTC6946_spi_map[LTC6946_PART][NUMBITS]= 5;
LTC6946_spi_map[LTC6946_PDALL][ADDRx]=0x02;    LTC6946_spi_map[LTC6946_PDALL][DxMSB]= 7;    LTC6946_spi_map[LTC6946_PDALL][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_PDOUT][ADDRx]=0x02;    LTC6946_spi_map[LTC6946_PDOUT][DxMSB]= 4;    LTC6946_spi_map[LTC6946_PDOUT][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_PDPLL][ADDRx]=0x02;    LTC6946_spi_map[LTC6946_PDPLL][DxMSB]= 6;    LTC6946_spi_map[LTC6946_PDPLL][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_PDREFO][ADDRx]=0x02;   LTC6946_spi_map[LTC6946_PDREFO][DxMSB]= 3;   LTC6946_spi_map[LTC6946_PDREFO][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_PDVCO][ADDRx]=0x02;    LTC6946_spi_map[LTC6946_PDVCO][DxMSB]= 5;    LTC6946_spi_map[LTC6946_PDVCO][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_POR][ADDRx]=0x02;      LTC6946_spi_map[LTC6946_POR][DxMSB]= 0;      LTC6946_spi_map[LTC6946_POR][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_RD][ADDRx]=0x03;       LTC6946_spi_map[LTC6946_RD][DxMSB]= 1;       LTC6946_spi_map[LTC6946_RD][NUMBITS]= 10;
LTC6946_spi_map[LTC6946_REV][ADDRx]=0x0b;      LTC6946_spi_map[LTC6946_REV][DxMSB]= 7;      LTC6946_spi_map[LTC6946_REV][NUMBITS]= 3;
LTC6946_spi_map[LTC6946_RFO][ADDRx]=0x08;      LTC6946_spi_map[LTC6946_RFO][DxMSB]= 4;      LTC6946_spi_map[LTC6946_RFO][NUMBITS]= 2;
LTC6946_spi_map[LTC6946_THI][ADDRx]=0x00;      LTC6946_spi_map[LTC6946_THI][DxMSB]= 1;      LTC6946_spi_map[LTC6946_THI][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_TLO][ADDRx]=0x00;      LTC6946_spi_map[LTC6946_TLO][DxMSB]= 0;      LTC6946_spi_map[LTC6946_TLO][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_UNLOK][ADDRx]=0x00;    LTC6946_spi_map[LTC6946_UNLOK][DxMSB]= 5;    LTC6946_spi_map[LTC6946_UNLOK][NUMBITS]= 1;
LTC6946_spi_map[LTC6946_x][ADDRx]=0x01;        LTC6946_spi_map[LTC6946_x][DxMSB]= 5;        LTC6946_spi_map[LTC6946_x][NUMBITS]= 6;
  
LTC6946_spi_map[LTC6946_ALCCAL][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_ALCEN][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_ALCHI][R_ONLY]= 1;
LTC6946_spi_map[LTC6946_ALCLO][R_ONLY]= 1;
LTC6946_spi_map[LTC6946_ALCMON][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_ALCULOK][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_BD][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_BST][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_CAL][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_CP][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_CPCHI][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_CPCLO][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_CPDN][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_CPINV][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_CPMID][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_CPRST][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_CPUP][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_CPWIDE][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_FILT][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_LKCT][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_LKEN][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_LKWIN][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_LOCK][R_ONLY]= 1;
LTC6946_spi_map[LTC6946_MTCAL][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_ND][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_OD][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_OMUTE][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_PART][R_ONLY]= 1;
LTC6946_spi_map[LTC6946_PDALL][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_PDOUT][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_PDPLL][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_PDREFO][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_PDVCO][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_POR][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_RD][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_REV][R_ONLY]= 1;
LTC6946_spi_map[LTC6946_RFO][R_ONLY]= 0;
LTC6946_spi_map[LTC6946_THI][R_ONLY]= 1;
LTC6946_spi_map[LTC6946_TLO][R_ONLY]= 1;
LTC6946_spi_map[LTC6946_UNLOK][R_ONLY]= 1;
LTC6946_spi_map[LTC6946_x][R_ONLY]= 0;


} // end of LTC6946_init




