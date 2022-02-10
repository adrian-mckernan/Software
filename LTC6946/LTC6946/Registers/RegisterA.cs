using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LTC6946.Registers
{
    class RegisterA
    {
        public bool CPDN { get; set; } = false;
        public bool CPUP { get; set; } = false;
        public bool CPRST { get; set; } = false;
        public bool CPWIDE { get; set; } = false;
        public bool CPINV { get; set; } = false;
        public bool CPMID { get; set; } = false;
        public bool CPCLO { get; set; } = false;
        public bool CPCHI { get; set; } = false;


        public RegisterA ShallowCopy()
        {
            return (RegisterA)this.MemberwiseClone();
        }
    }
}
