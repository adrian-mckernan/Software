using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace LTC6946.Registers
{
    class Register8
    {

        public byte RFO { get; set; } = (byte) RFOS.zerodBm;
        public byte FILT { get; set; } = (byte)FILTS.LessThan20MHz;
        public bool BST { get; set; } = false;

        //public byte OD { get; set; } = (byte)ODS._2;

        public enum RFOS : byte
        {
            minus9dBm = 0,
            minus6dBm = 0x01,
            minus3dBm = 0x2,
            zerodBm = 0x3
        }

        public enum FILTS : byte
         {
            LessThan20MHz = 0x3,
            Between20and50MHz = 0x1,
            GreaterThan50MHz = 0x0
        }

        public enum BSTS 
        {
            LessThan2Vpp = 0x01,
            GreaterThanEqualTo2Vpp = 0x0
        }

        //public enum ODS : byte
        //{
        //    _1 = 0x01,
        //    _2 = 0x02,
        //    _3 = 0x03,
        //    _4 = 0x04,
        //    _5 = 0x05,
        //    _6 = 0x06
        //}


        public Register8 ShallowCopy()
        {
            return (Register8) this.MemberwiseClone();
        }
    }
}
