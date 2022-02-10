using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LTC6946.Registers
{
    class Register7
    {
        public bool LKEN { get; set; } = true;
        public bool CAL { get; set; } = true;
        public bool ALCULOK { get; set; } =false;
        public bool ALCCAL { get; set; } = true;
        public bool ALCMON { get; set; } = true;
        public bool ALCEN { get; set; } = false;

        public Register7 ShallowCopy()
        {
            return (Register7) this.MemberwiseClone();
        }
    }
}
