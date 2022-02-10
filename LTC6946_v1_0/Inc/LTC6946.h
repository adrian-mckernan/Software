/*!
 LTC6946: Ultralow Noise and Spurious 0.37GHz to 6.39GHz IntegerN Synthesizer with Integrated VCO

@verbatim
 SPI DATA FORMAT (MSB First):

 Write Sequence:
       Byte #1                    Byte #2
 MOSI: A6 A5 A4 A3 A2 A1 A0 W   D7 D6 D5 D4 D3 D2 D1 D0
 MISO: X  X  X  X  X  X  X  X   X  X  X  X  X  X  X  X

 Read Sequence:
       Byte #1                    Byte #2
 MOSI: A6 A5 A4 A3 A2 A1 A0 R   X  X  X  X  X  X  X  X
 MISO: X  X  X  X  X  X  X  X   D7 D6 D5 D4 D3 D2 D1 D0

 W    : SPI Write (0)
 R    : SPI Read  (1)
 Ax   : Address
 Dx   : Data Bits
 X    : Don't care

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

/*! @file
    @ingroup LTC6946
    Header for LTC6946: Ultralow Noise and Spurious 0.37GHz to 6.39GHz IntegerN Synthesizer with Integrated VCO
*/

#ifndef LTC6946_H

#define LTC6946_H


//! Define the SPI CS pin
#ifndef LTC6946_CS
#define LTC6946_CS QUIKEVAL_CS  //! SPI Chip Select Pin
#endif


/*! @name LTC6946 Registers Fields in Alphabetical Order */
#define LTC6946_ALCCAL 1   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_ALCEN 2   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_ALCHI 3   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_ALCLO 4   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_ALCMON 5   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_ALCULOK 6   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_BD 7   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_BST 8   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_CAL 9   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_CP 10   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_CPCHI 11   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_CPCLO 12   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_CPDN 13   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_CPINV 14   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_CPMID 15   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_CPRST 16   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_CPUP 17   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_CPWIDE 18   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_FILT 19   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_LKCT 20   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_LKEN 21   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_LKWIN 22   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_LOCK 23   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_MTCAL 24   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_ND 25   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_OD 26   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_OMUTE 27   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_PART 28   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_PDALL 29   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_PDOUT 30   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_PDPLL 31   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_PDREFO 32   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_PDVCO 33   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_POR 34   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_RD 35   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_REV 36   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_RFO 37   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_THI 38   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_TLO 39   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_UNLOK 40   //!<  for spi_map array, defines location for field specific information used to create the spi map
#define LTC6946_x 41   //!<  for spi_map array, defines location for field specific information used to create the spi map


#define LTC6946_NUM_REGADDR 12  //!< Defines number of LTC6946 SPI registers, used in spi_map array
#define LTC6946_NUM_REGFIELD 41 //!< Defines number of LTC6946 SPI fields, used in spi_map array

#define ADDRx 0                 //!< used for 2nd dim of 2d spi_map array
#define DxMSB 1                 //!< used for 2nd dim of 2d spi_map array
#define NUMBITS 2               //!< used for 2nd dim of 2d spi_map array
#define R_ONLY 3                //!< used for 2nd dim of 2d spi_map array

#define OneMHz 1000000L         //!< 1MHz in long format, used in 64 bit math

#define LTC6946_1_MAXFREQ 3740  //!< LTC6946-1 upper freq limit
#define LTC6946_2_MAXFREQ 4910  //!< LTC6946-2 upper freq limit
#define LTC6946_3_MAXFREQ 5790  //!< LTC6946-3 upper freq limit
#define LTC6946_4_MAXFREQ 6390  //!< LTC6946-4 upper freq limit

#define LTC6946_1_MINFREQ 2240  //!< LTC6946-1 lower freq limit
#define LTC6946_2_MINFREQ 3080  //!< LTC6946-2 lower freq limit
#define LTC6946_3_MINFREQ 3840  //!< LTC6946-3 lower freq limit
#define LTC6946_4_MINFREQ 4200  //!< LTC6946-4 lower freq limit

#define LTC6946_MIN_REF_FREQ 10  //!< LTC6946 lower reference frequency limit
#define LTC6946_MAX_REF_FREQ 250 //!< LTC6946 upper reference frequency limit


//#define H00 0x00
//#define H01 0x04
//#define H02 0x00
//#define H03 0x00
//#define H04 064
//#define H05 0xBB
//#define H06 0x80
//#define H07 0x7D
//#define H08 0x02
//#define H09 0x1B
//#define H0A 0x40
//#define H0B 0x04


#define H00 0x00
#define H01 0x04
#define H02 0x0C
#define H03 0x00
#define H04 0x0A
#define H05 0x12
#define H06 0xC0
#define H07 0x33  /// CAL bit set
#define H08 0xFA
#define H09 0x8B
#define H0A 0x00
#define H0B 0x64



//! @} */

/* ------------------------------------------------------------------------- */
//! LTC6946 Read Single Address
//!  reads 8 bit Data field to LTC6946.
//!  has to shift data by one bit to account for RW bit
//! @return data that was read from address
uint8_t LTC6946_read(int8_t address //!< Register address for the LTC6946.
                    );

                    
/* ------------------------------------------------------------------------- */
//! LTC6946 Read Single Field
//! For SPI FIELDS located in 1 or multiple address locations
//!  reads specific address locations
//!  identifies and returns specific field in question
//!  can handle SPI fields in multiple addresses, if MSB bit is in the lower number address
//! @return data that was read from field
 long LTC6946_read_field(uint8_t address,  //!< Register address for the LTC6946.
                         uint8_t MSB_loc,  //!< MSB bit location of field
                         uint8_t numbits   //!< length of field (i.e. number of bits in field)                    
                        );
 
 
/* ------------------------------------------------------------------------- */
//! Gets the LTC6946 SPI field value
//! calls function LTC6946_read_field, which
//!  reads specific address locations
//!  identifies and returns specific field in question
//!  can handle SPI fields in multiple addresses, if MSB bit is in the lower number address
//! @return data that was read from field
long get_LTC6946_SPI_FIELD(uint8_t f            //!< SPI field number
                          );
        
        
/* ------------------------------------------------------------------------- */
//! LTC6946 Write Single Address
//!  writes 8 bit Data field to LTC6946.
//!  has to shift data by one bit to account for RW bit
//! @return void
void LTC6946_write(uint8_t address,             //!< Register address for the LTC6946.
                   uint8_t Data                 //!< 8-bit data to be written to register
                  );

                  
/* ------------------------------------------------------------------------- */
//! LTC6946 Write Single Field
//!  For SPI FIELDS in 1 or multiple address locations
//!  reads specific address/field location then writes to specific field
//!  can handle SPI fields in multiple addresses, if MSB bit is in the lower number address
//! @return void
 uint8_t LTC6946_write_field(long field_data,   //!< Value field value to be set to
                             uint8_t address,   //!< Register address for the LTC6946.
                             uint8_t MSB_loc,   //!< MSB bit location of field
                             uint8_t numbits   //!< length of field (i.e. number of bits in field)
                            );
 
 
/* ------------------------------------------------------------------------- */
//! Sets the LTC6946 SPI field value
//! calls function LTC6946_read_field, which
//!  reads specific address/field location then writes to specific field
//!  can handle SPI fields in multiple addresses, if MSB bit is in the lower number address
//! @return void
void set_LTC6946_SPI_FIELD(uint8_t f,           //!< SPI field number
                           long field_data      //!< Value field value to be set to
                          );

                          
/* ------------------------------------------------------------------------- */
//! Writes values to ALL LTC6946 RW address
//! @return void
void set_LTC6946_ALLREGS(uint8_t reg01,         //!< LTC6946 register 1
                         uint8_t reg02,         //!< LTC6946 register 2   
                         uint8_t reg03,         //!< LTC6946 register 3 
                         uint8_t reg04,         //!< LTC6946 register 4 
                         uint8_t reg05,         //!< LTC6946 register 5 
                         uint8_t reg06,         //!< LTC6946 register 6
                         uint8_t reg07,         //!< LTC6946 register 7
                         uint8_t reg08,         //!< LTC6946 register 8
                         uint8_t reg09,         //!< LTC6946 register 9
                         uint8_t reg0A          //!< LTC6946 register 10
                        );

/* ------------------------------------------------------------------------- */
//! Initializes the SPI MAP arrays
//! The values set in initialization are used all the LTC6946 SPI/WRITE and 
//! read functions (set_LTC6946_SPI_FIELD, get_LTC6946_SPI_FIELD,
//! LTC6946_read, LTC6946_write, etc, etc)
//! @return void
void LTC6946_init(void);


/* ------------------------------------------------------------------------- */
//! returns # of addresses in parts register map (array size)
//! @return # of addresses in parts register map
uint8_t get_LTC6946_REGSIZE(void);


/* ------------------------------------------------------------------------- */
//! returns the number of bits for a given field name in the SPI map
//! @return the number of bits for a given field name in the SPI map
uint8_t get_LTC6946_SPI_FIELD_NUMBITS(uint8_t f //!< SPI field number 
                                      );

                                      
/* ------------------------------------------------------------------------- */
//! returns if the given field name is (0)read/write or (1)read_only field
//! @return if the given field is a (0)read/write or (1)read_only field
uint8_t get_LTC6946_SPI_FIELD_RW(uint8_t f   //!< SPI field number 
                                ) ; 



/* ------------------------------------------------------------------------- */
//! write teh #define value above to all write able registers
void LTC6946_initRegs(void);
                                


#endif  // LTC6946_H
