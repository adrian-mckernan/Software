using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LTC6946
{
    class Protocol
    {

        public const Byte startLoc = 0;
        public const Byte nodeLoc = 2;
        public const Byte funcLowLoc = 8;
        public const Byte funcHighLoc = 10;
        public const Byte valueLoc = 14;
        public const Byte RegHighLoc = 22;
        public const Byte RegLowLoc = 14;
        public const Byte endLoc = 30;


        public const uint startMask = 0x00000003;
        public const uint nodeMask = 0x000000FC;
        public const uint funcLowMask = 0x00000300;
        public const uint funcHighMask = 0x00003C00;
        public const uint valueMask = 0x3FFFC000;
        public const uint endMask = 0xC0000000;
        public const uint startVal = 3u << startLoc;
        public const uint endVal = 2u << endLoc;
        public const uint RegReadPKT = 0u << funcLowLoc;
        public const uint RegWritePKT = 2u << funcLowLoc;
        public const uint funPKT = 1u << funcLowLoc;
        public const uint initalisePKT = 1u << funcHighLoc;
        public const uint phasePKT = 2u  << funcHighLoc;
        public const uint powerPKT = 4u  << funcHighLoc;
        public const uint mutePKT = 8u << funcHighLoc;
    }
}
