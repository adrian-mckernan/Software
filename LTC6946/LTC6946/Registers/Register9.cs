using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LTC6946.Registers
{
    class Register9
    {
        public byte CP { get; set; } = (byte) CPs._11200uA;
        public byte LKCT { get; set; } = (byte) LKCTs._32;
        public byte LKWIN { get; set; } = (byte) LKWINs._30ns;

        public enum CPs : byte
        {
            _250uA = 0,
            _350uA = 1,
            _500uA = 2,
            _700uA = 3,
            _1000uA = 4,
            _1400uA = 5,
            _2000uA = 6,
            _2800uA = 7,
            _4000uA = 8,
            _5600uA = 9,
            _8000uA = 10,
            _11200uA = 11
        }


        public enum LKWINs : byte
        {
            _3ns = 0x0,
            _10ns = 0x1,
            _30ns = 0x2,
            _90ns = 0x3
        }

        public enum LKCTs  : byte
        {
            _32 = 0x0,
            _128 = 0x1,
            _512 = 0x2,
            _2048 = 0x3
        }


        public Register9 ShallowCopy()
        {
            return (Register9) this.MemberwiseClone();
        }
    }
}
