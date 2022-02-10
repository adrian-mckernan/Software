using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LTC6946.Registers
{
    class Register1
    {
        public bool TLO { get; set; } = false;
        public bool THI { get; set; } = false;
        public bool LOCK { get; set; } = true;
        public bool ALCLO { get; set; } = false;
        public bool ALCHI { get; set; } = false;
        public bool UNLOCK { get; set; } = false;


        public Register1 ShallowCopy()
        {
            return (Register1)this.MemberwiseClone();
        }
    }
}
