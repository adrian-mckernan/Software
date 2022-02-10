using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LTC6946.Registers
{
    class Register2
    {
        public bool POR { get; set; }  = false;
        public bool OMUTE { get; set; } = false;
        public bool MTCAL { get; set; } = true;
        public bool PDREFO { get; set; } = true;
        public bool PDOUT { get; set; } = false;
        public bool PDVCO { get; set; } = false;
        public bool PDPLL { get; set; } = false;
        public bool PDALL { get; set; } = false;

        public Register2 ShallowCopy()
        {
            return (Register2) this.MemberwiseClone();
        }
    }
}
