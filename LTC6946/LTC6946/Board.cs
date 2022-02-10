using LTC6946.Registers;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Media;



namespace LTC6946
{
    class Board :INotifyPropertyChanged
    {

        // Fields
        public static decimal defaultFrequency;
        private decimal targetFrequency;
        private decimal actualFrequency;
        private readonly uint boardNumber;
        private bool enable_freq_Box;
        private bool mute_Box;
        private bool locked;
        private UInt32 phase;
        private decimal attenuation;
        //private decimal outpower;
        private readonly uint phaseOffset;
        //private decimal phaseVoltage;
        



        // Properties
        public static decimal DefaultFrequency
        {
            get
            {
                return defaultFrequency;
            }
            set
            {
                defaultFrequency = value;

            }
        }

        public Register1 Reg01 { get; set; }
        public Register2 Reg02 { get; set; }
        public Register7 Reg07 { get; set; }
        public Register8 Reg08 { get; set; }
        public Register9 Reg09 { get; set; }
        public RegisterA Reg0A { get; set; }

        public decimal ActualFrequency
        {
            get { return actualFrequency; }
            set { actualFrequency = value; }
            // add in logic to calulate this frequency
        }
        public UInt32 BoardNumber
        {
            get { return boardNumber; }
        }
        public decimal TargetFrequency
        {
            get
            {
                if (Enable_freq_Box == false)
                {
                    targetFrequency = DefaultFrequency;
                }
                return targetFrequency;
            }
            set
            {
                if (Enable_freq_Box)
                {
                    targetFrequency = value;
                    OnPropertyChanged(nameof(TargetFrequency));
                }
                else
                {
                    targetFrequency = defaultFrequency;
                    OnPropertyChanged(nameof(TargetFrequency));
                }
            }
        }
        public UInt32 Phase
        {
            get { return phase; }
            set { phase = value; }
            // add in logic to loop round phase 
        }
        public UInt32 PhaseOffset
        {
            get { return phaseOffset; }
        }
        
        public Decimal Attenuation
        {
            get { return attenuation; }
            set {
                attenuation = value;
                OnPropertyChanged(nameof(OutPower));
            }
        }
        public decimal RefFreq { get; set; } 
        public decimal StepFreq { get; set; }
        public decimal VCOFreq { get; set; }
        public decimal PFDFreq { get; set; }
        public bool Locked {
            get
            {
                return locked;
            }
            set
            {
                locked = value;
            }
        }
        public string LockColour
        {
            get
            {
                if (locked)
                {
                    return Colors.Green.ToString();
                }
                else
                {
                    return Colors.Red.ToString();
                }
            }
        }
        public bool Mute_Box {
            get
            {
                return mute_Box;
            }

            set
            {
                mute_Box = value;
                Reg02.OMUTE = value;
            }
        }
        public bool Enable_freq_Box
        {
            get
            {
                return enable_freq_Box;
            }
            set
            {
                enable_freq_Box = value;
                if (value == false)
                {
                    UpdateTargetFrequency();
                }
            }
        }
        public bool Enable_program_board { get; set; }
        public UInt32 N_Divider { get; set; } = 4800;
        public UInt32 O_Divider { get; set; } = 2;
        public UInt32 R_Divider { get; set; } = 10;
        public UInt32 B_Divider { get; set; } = 0;
        public UInt32 LTCPower { get; set; }
        public UInt32 DATValue { get; set; }

        public Decimal OutPower
        {
            get {
                decimal power, outpower;
                power = 0;
                if (Reg08.RFO == (byte) Register8.RFOS.zerodBm)
                    power = 0;
                if (Reg08.RFO == (byte) Register8.RFOS.minus3dBm)
                    power = -3;
                if (Reg08.RFO == (byte) Register8.RFOS.minus6dBm)
                    power = -6;
                if (Reg08.RFO == (byte) Register8.RFOS.minus9dBm)
                    power = -9;

                outpower = power - 3 - attenuation;
                return outpower;
            }
            
        }

        public byte ByteReg00 { get; set; }
        public byte ByteReg01 {
            get
            {
                byte reg = 0;
                if (Reg01.TLO)  reg += Constants.TLO_BIT;
                if (Reg01.THI) reg += Constants.THI_BIT;
                if (Reg01.LOCK) reg += Constants.LOCK_BIT;
                if (Reg01.ALCLO) reg += Constants.ALCLO_BIT;
                if (Reg01.ALCHI) reg += Constants.ALCHI_BIT;
                if (Reg01.UNLOCK) reg += Constants.UNLOCK_BIT;
                return reg;
            }
        }
        public byte ByteReg02 {
            get
            {
                byte reg = 0;
                if (Reg02.POR) reg += Constants.POR_BIT; 
                if (Reg02.OMUTE) reg += Constants.OMUTE_BIT;
                if (Reg02.MTCAL) reg += Constants.MTCAL_BIT;
                if (Reg02.PDREFO) reg += Constants.PDREFO_BIT;
                if (Reg02.PDOUT) reg += Constants.PDOUT_BIT;
                if (Reg02.PDVCO) reg += Constants.PDVCO_BIT;
                if (Reg02.PDPLL) reg += Constants.PDPLL_BIT;
                if (Reg02.PDALL) reg += Constants.PDALL_BIT;

                return reg;
            }           
        }

        public byte ByteReg03
        {
            get
            {
                byte reg = 0;
                reg = (byte)((R_Divider & Constants.R_HIGH_MASK) >> Constants.R_HIGH_SHIFT);
                reg += (byte) ((B_Divider & Constants.B_MASK) << Constants.B_SHIFT);
                
                return reg;
            }
         }
        public byte ByteReg04
        { 
                get
            {
                byte reg = 0;
                reg = (byte)(R_Divider & Constants.R_LOW_MASK);
                return reg;
            }
        }
        public byte ByteReg05
        { 
            get
            {
                byte reg = 0;
                reg = (byte) ((N_Divider & Constants.N_HIGH_MASK) >> Constants.N_HIGH_SHIFT);
                return reg;
            }
        }
        public byte ByteReg06
        {
            get
            {
                byte reg = 0;
                reg = (byte)(N_Divider & Constants.N_LOW_MASK);
                return reg;
            }
        }
        public byte ByteReg07
        {
            get
            {
                byte reg = 0;
                if (Reg07.LKEN) reg += Constants.LKEN_BIT;
                if (Reg07.CAL)  reg += Constants.CAL_BIT;
                if (Reg07.ALCULOK) reg += Constants.ALCULOK_BIT;
                if (Reg07.ALCCAL) reg += Constants.ALCCAL_BIT;
                if (Reg07.ALCMON) reg += Constants.ALCMON_BIT;
                if (Reg07.ALCEN) reg += Constants.ALCEN_BIT;
                return reg;
            }
        }
        public byte ByteReg08
        {
            get
            {
                byte reg = 0;
                reg += (byte)(O_Divider & Constants.O_MASK);
                reg += (byte)((Reg08.RFO & Constants.RFO_MASK) << Constants.RFO_SHIFT);
                reg += (byte)((Reg08.FILT & Constants.FILT_MASK) << Constants.FILT_SHIFT);
                if (Reg08.BST) reg +=  Constants.BST_BIT;
                OnPropertyChanged(nameof(OutPower));
                return reg;
            }
        }
        public byte ByteReg09
        {
            get
            {
                byte reg = 0;
                reg += (byte)(Reg09.CP & Constants.CP_MASK);
                reg += (byte)((Reg09.LKCT & Constants.LKCT_MASK) << Constants.LKCT_SHIFT);
                reg += (byte)((Reg09.LKWIN & Constants.LKWIN_MASK) << Constants.LKWIN_SHIFT);
                return reg;
            }
        }
        public byte ByteReg0A
        {
            get
            {
                byte reg = 0;
                if (Reg0A.CPDN) reg += Constants.CPDN_BIT;
                if (Reg0A.CPUP) reg += Constants.CPUP_BIT;
                if (Reg0A.CPRST) reg += Constants.CPRST_BIT;
                if (Reg0A.CPWIDE) reg += Constants.CPWIDE_BIT;
                if (Reg0A.CPINV) reg += Constants.CPINV_BIT;
                if (Reg0A.CPMID) reg += Constants.CPMID_BIT;
                if (Reg0A.CPCLO) reg += Constants.CPCLO_BIT;
                if (Reg0A.CPCHI) reg += Constants.CPCHI_BIT;
                return reg;
            }
        }
        public byte ByteReg0B { get; set; }

        public byte[] ByteRegs
        {
            get
            {
                byte[] Regs = { ByteReg00, ByteReg01, ByteReg02, ByteReg03, ByteReg04, ByteReg05, ByteReg06, ByteReg07, ByteReg08, ByteReg09, ByteReg0A, ByteReg0B};
                return Regs;
            }
        }

        
        
        
        // Constructors
        public Board(UInt32 Number, UInt32 Offset)
        {
            boardNumber = Number;
            phaseOffset = Offset;
            Enable_freq_Box = false;
            TargetFrequency = DefaultFrequency;

            Reg01 = new  Register1();
            Reg02 = new  Register2();
            Reg07 = new  Register7();
            Reg08 = new  Register8();
            Reg09 = new  Register9();
            Reg0A = new  RegisterA();
        }

        // events

        public event PropertyChangedEventHandler PropertyChanged;

        // Methods

            public void PropertyChangedReg01()
        {
            OnPropertyChanged(nameof(ByteReg01));
        }

        public void PropertyChangedReg02()
        {
            OnPropertyChanged(nameof(ByteReg02));
        }
        public void PropertyChangedReg03()
        {
            OnPropertyChanged(nameof(ByteReg03));
        }
        public void PropertyChangedReg04()
        {
            OnPropertyChanged(nameof(ByteReg04));
        }
        public void PropertyChangedReg05()
        {
            OnPropertyChanged(nameof(ByteReg05));
        }
        public void PropertyChangedReg06()
        {
            OnPropertyChanged(nameof(ByteReg06));
        }

        public void PropertyChangedReg07()
        {
            OnPropertyChanged(nameof(ByteReg07));
        }

        public void PropertyChangedReg08()
        {
            OnPropertyChanged(nameof(ByteReg08));
        }

        public void PropertyChangedReg09()
        {
            OnPropertyChanged(nameof(ByteReg09));
        }

        public void PropertyChangedReg0A()
        {
            OnPropertyChanged(nameof(ByteReg0A));
        }
        protected void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
            }
        }
        public void UpdateTargetFrequency()
        {
            if (Enable_freq_Box== false)
            {          
                targetFrequency = defaultFrequency;
                OnPropertyChanged(nameof(TargetFrequency));
            }
            Calc_Frf(10e6M, TargetFrequency, 500e3M);
        }

        public void SendPhase(SerialPort mySerialPort)
        {
            //var buf = new byte[4] { 0x00000000 };
            byte[] buf = { 0, 0, 0, 0 };
            //uint reg = 0;

            uint packet = 0;
            // set packet type to Phase
            packet = Protocol.startVal + Protocol.endVal + Protocol.funPKT +  Protocol.phasePKT;
            packet = packet + ((boardNumber << Protocol.nodeLoc) & Protocol.nodeMask);
            packet = packet + ((phase << Protocol.valueLoc) & Protocol.valueMask);


            for (int i = 0; i < 4; i++)
            {
                buf[i] = (byte)(packet >> (i * 8));

            }
            mySerialPort.Write(buf, 0, 4);

        }

        public void SendMute(SerialPort mySerialPort)
        {
            //var buf = new byte[4] { 0x00000000 };
            byte[] buf = { 0, 0, 0, 0 };
            //uint reg = 0;

            uint packet = 0;
            // set packet type to Phase
            packet = Protocol.startVal + Protocol.endVal + Protocol.funPKT + Protocol.mutePKT;
            packet = packet + ((boardNumber << Protocol.nodeLoc) & Protocol.nodeMask);
            if (mute_Box) {
                packet = packet + ((1 << Protocol.valueLoc) & Protocol.valueMask);
            }
            else
            {
                packet = packet + ((0 << Protocol.valueLoc) & Protocol.valueMask);
            }

            for (int i = 0; i < 4; i++)
            {
                buf[i] = (byte)(packet >> (i * 8));

            }
            mySerialPort.Write(buf, 0, 4);

        }

        public void SendPower(SerialPort mySerialPort)
        {
            //var buf = new byte[4] { 0x00000000 };
            byte[] buf = { 0, 0, 0, 0 };
            // uint reg = 0;
            uint power;
            power = (uint) Math.Floor(attenuation * 2);
            uint packet = 0;
            // set packet type to Phase
            packet = Protocol.startVal + Protocol.endVal + Protocol.funPKT + Protocol.powerPKT;
            packet = packet + ((boardNumber << Protocol.nodeLoc) & Protocol.nodeMask);
            packet = packet + ((power << Protocol.valueLoc) & Protocol.valueMask);


            for (int i = 0; i < 4; i++)
            {
                buf[i] = (byte)(packet >> (i * 8));

            }
            mySerialPort.Write(buf, 0, 4);

        }



        public void SendRegisters(SerialPort mySerialPort, uint firstReg )
        {

            //var buf = new byte[4] { 0x00000000 };
            byte[] buf = { 0,0,0,0 };
            uint reg = 0;
            
            uint packet = 0;
            // set packet type to REG write
            packet = Protocol.startVal + Protocol.endVal + Protocol.RegWritePKT;
            // Set board/node number
            packet = packet + ((boardNumber << Protocol.nodeLoc) & Protocol.nodeMask);
            // set reg address to first register
            packet = packet + ((firstReg << Protocol.funcHighLoc) & Protocol.funcHighMask);
            // add first register in low location
            reg = ByteRegs[firstReg];
            reg = reg << Protocol.RegLowLoc;
            packet = packet + reg;
            //add next register in high location. This must be the next highest reg
            reg = ByteRegs[firstReg + 1];
            reg = reg << Protocol.RegHighLoc;
            packet = packet + reg;


            


            for (int i = 0; i < 4; i++)
            {
                buf[i] = (byte)(packet >> (i * 8));

            }
            mySerialPort.Write(buf, 0,4);
            // if thsi works then credate byte array and loop through it.
            // Does the protocol not have two reg in each packet???? 


        }


        public void SendAllRegisters(SerialPort mySerialPort)
        {
            for (uint i = 1; i < 11; i=i+2)
            {
                SendRegisters(mySerialPort, i);
            }
                
                
        }

        public bool CheckLock(SerialPort mySerialPort)
        {
            if (BoardNumber > 1)
            {
                Locked = false;
                return false;
            }
            else
            {
                Locked = true;
                return true;
            }
        }

        // First step is to calulate the divider value
        // Valid ansewrs are 1 to 6
        // 
        private UInt32 Calc_O_div()
        {
            bool valid_input = false;
            UInt32 odiv=999;
            decimal Temp_vco;
            for (UInt64 i = 1; i <= 6; i++)
            {
                Temp_vco = TargetFrequency * i;

                if (Decimal.Compare(Temp_vco, Constants.VCO_Min)>=0)
                {
                    if (Decimal.Compare(Temp_vco ,Constants.VCO_Max) <=0)
                    {
                        valid_input = true;
                        odiv = (UInt32)i;
                    }
                }
                
            } // end of for loop

            if (valid_input == false)
            {
                odiv = 999;
            }
            return odiv;

        }

        //Fstep = Fref/(R*O)
        //R = Fref/(Fstep*O)
        private UInt32 Calc_R_div()        
        {
            decimal rdiv = 0;
            UInt32 r_div = 0;
            rdiv = RefFreq / (StepFreq * O_Divider);
            r_div = (UInt32)Math.Round(rdiv);
            return r_div;
        }

        private decimal Calc_Fpfd()
        // Fpfd = Fref/R
        {
            decimal Fpdf;

            Fpdf = RefFreq / R_Divider;

            return Fpdf;

        }

        private UInt32 Calc_N_div()
        //Fvco = Fpfd * N
        // N = Fvco/Fpfd
        {
            decimal temp_Fvco, temp_N;
            UInt32 ndiv;

            temp_Fvco = TargetFrequency / O_Divider;
            temp_N = temp_Fvco / PFDFreq;
            ndiv = (UInt32)Math.Round(temp_N);

            return ndiv;

        }

        private decimal Calc_VCO()
            //Fvco = Fpfd * N
        {
            decimal VCO;

            VCO = PFDFreq * N_Divider;

            return VCO;

        }

        private decimal Calc_RF()
        {
            decimal RF;

            RF = VCOFreq * O_Divider;

            return RF;
        }

        private UInt32 Calc_Bdiv()
            // B>= Fpfd/(Fcal-max)
        {
            double tempB;
            UInt32 bdiv = 999;

            tempB = (double)(PFDFreq / Constants.Fcal_max);

            for (UInt32 i =0; i<Constants.B_div.Length; i++)
            {
                if (tempB < Constants.B_div[i])
                {
                    bdiv = i;
                }
            }
            // 999 is a an error return
            return bdiv;

        }

        public int Calc_Frf(decimal Fref, decimal Frf, decimal Fstep)
        {
            UInt32 temp_odiv;
            if (Frf.CompareTo(Constants.VCO_Max)>0 | Frf.CompareTo(Constants.VCO_Min / 6) < 0)
            {
                // error fRef is out of range
                return -1;
            }
            else
            {
                TargetFrequency = Frf;
            }

            if (Fref.CompareTo(Constants.REF_Max)>0 | Fref.CompareTo(Constants.REF_Min) < 0)
            {
                //error Reference frequerncy out of range
                return -2;

            }
            else
            {
                RefFreq = Fref;
            }

            // What are valif Fstep sizes????
            StepFreq = Fstep;


            temp_odiv= Calc_O_div();
            if (temp_odiv > 10)
            {
                // error odiv should be between 0 and 6
                // invalid set frequency
                return -4;
            }
            else
            {
                O_Divider = temp_odiv;
            }
            R_Divider =  Calc_R_div();
            PFDFreq = Calc_Fpfd();
            N_Divider = Calc_N_div();
            VCOFreq = Calc_VCO();
            ActualFrequency = Calc_RF();
            B_Divider = Calc_Bdiv();

            return 1;
        }
      

    }

   


}
