using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LTC6946.Registers
{
    public static class Constants
    {
        public const decimal Fcal_max = 1.8e6M;
        public const decimal VCO_Max = 6.39e9M;
        public const decimal VCO_Min = 4.2e9M;
        public const decimal REF_Max = 250e6M;
        public const decimal REF_Min = 10e6M;
        public static Int32[] B_div = { 8, 12, 16, 24, 32, 48, 64, 96, 128, 192, 256, 384 };
        //public static  Parity DEFAULT_PORT_PARITY= Parity.None;
        //public static  Int32 DEFAULT_PORT_DATABITS = 8;
        //public static  Int32 DEFAULT_PORT_STOPBITS = 1;

        // BITs
        public const byte BIT0 = 1;
        public const byte BIT1 = 1 << 1;
        public const byte BIT2 = 1 << 2;
        public const byte BIT3 = 1 << 3;
        public const byte BIT4 = 1 << 4;
        public const byte BIT5 = 1 << 5;
        public const byte BIT6 = 1 << 6;
        public const byte BIT7 = 1 << 7;
        public const UInt16 BIT8 = 1 << 8;
        public const UInt16 BIT9 = 1 << 9;
        public const UInt16 BIT10 = 1 << 10;
        public const UInt16 BIT11 = 1 << 11;
        public const UInt16 BIT12 = 1 << 12;
        public const UInt16 BIT13 = 1 << 13;
        public const UInt16 BIT14 = 1 << 14;
        public const UInt16 BIT15 = 1 << 15;

        // Reg 0 and Reg 1 BITS
        public const byte TLO_BIT = BIT0;
        public const byte THI_BIT = BIT1;
        public const byte LOCK_BIT = BIT2;
        public const byte ALCLO_BIT = BIT3;
        public const byte ALCHI_BIT = BIT4;
        public const byte UNLOCK_BIT = BIT5;

        // Reg2
        public const byte POR_BIT = BIT0;
        public const byte OMUTE_BIT = BIT1;
        public const byte MTCAL_BIT = BIT2;
        public const byte PDREFO_BIT = BIT3;
        public const byte PDOUT_BIT = BIT4;
        public const byte PDVCO_BIT = BIT5;
        public const byte PDPLL_BIT = BIT6;
        public const byte PDALL_BIT = BIT7;

        // REG3
        public const byte B_MASK = BIT0 | BIT1 | BIT2 | BIT3;
        public const byte B_SHIFT = 4;
        public const UInt32 R_HIGH_MASK = BIT8 | BIT9;
        public const byte R_HIGH_SHIFT = 8;

        // REG4
        // bits 0 to 8
        public const byte R_LOW_MASK = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;

        // REG 5 
        public const UInt32 N_HIGH_MASK = BIT8 | BIT9 | BIT10 | BIT11 | BIT12 | BIT13 | BIT14 | BIT15;
        public const byte N_HIGH_SHIFT = 8;

        // REG 6 
        public const byte N_LOW_MASK = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;

        // REG7
        public const byte LKEN_BIT = BIT0;
        public const byte CAL_BIT = BIT1;
        public const byte ALCULOK_BIT = BIT4;
        public const byte ALCCAL_BIT = BIT5;
        public const byte ALCMON_BIT = BIT6;
        public const byte ALCEN_BIT = BIT7;

        // REG 8
        public const byte O_MASK = BIT0 | BIT1 | BIT2;
        public const byte RFO_MASK = BIT0 | BIT1;
        public const byte RFO_SHIFT = 3;
        public const byte FILT_MASK = BIT0 | BIT1;
        public const byte FILT_SHIFT = 5;
        public const byte BST_BIT = BIT7;
        public const byte BST_SHIFT = 7;

        // REG 9
        public const byte CP_MASK = BIT0 | BIT1 | BIT2 | BIT3;
        public const byte LKCT_MASK = BIT0 | BIT1;
        public const byte LKCT_SHIFT = 4;
        public const byte LKWIN_MASK = BIT0 | BIT1;
        public const byte LKWIN_SHIFT = 6;

        // REG 10
        public const byte CPDN_BIT = BIT1;
        public const byte CPUP_BIT = BIT2;
        public const byte CPRST_BIT = BIT2;
        public const byte CPWIDE_BIT = BIT3;
        public const byte CPINV_BIT = BIT4;
        public const byte CPMID_BIT = BIT5;
        public const byte CPCLO_BIT = BIT6;
        public const byte CPCHI_BIT = BIT7;


    }
}
