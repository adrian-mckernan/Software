using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Collections;
using System.IO.Ports;
using LTC6946.Registers;
using System.Threading;

namespace LTC6946
{
    /// <summary>
    /// Interaction logic for Home.xaml
    /// </summary>
    public partial class Home : Page
    {
        private List<Board> Boards = new List<Board>();
        private SerialPort mySerialPort = new SerialPort();
        private Parity DEFAULT_PORT_PARITY ;
        private Int32 DEFAULT_PORT_DATABITS ;
        private Int32 DEFAULT_PORT_BAUDRATE;
        private StopBits DEFAULT_PORT_STOPBITS ;
        //private decimal DefaultFrequency = 2400;


        public Home()
        {
            InitializeComponent();
            InitHome();


        }

        public void InitHome()
        {
            Int32 NumberOfBoards = Properties.Settings.Default.NumberOfBoards;
            string[] Offsets = Properties.Settings.Default.Phase_offset.Split(',');
            UInt32[] PhaseOffsets;
            PhaseOffsets = new UInt32[Offsets.Length];
            // make static value equal to GUI value
            // add code here to get default from config file
            GlobalFrequency.DefaultValue = 2400;
            //GlobalFrequency.Value = GlobalFrequency.DefaultValue;
            Board.DefaultFrequency = GlobalFrequency.Value.GetValueOrDefault();

            // find comm ports
            string[] ports = SerialPort.GetPortNames();
            CommPorts.ItemsSource = ports;

            if (NumberOfBoards == Offsets.Length)
            {

                for (uint i = 0; i < Offsets.Length; i++)
                {
                    if(UInt32.TryParse(Offsets[i], out PhaseOffsets[i]))
                    {
                                               
                        Boards.Add(new Board((i+1), PhaseOffsets[i]));
                        //Boards.Last().TargetFrequency = boa;
                        Boards.Last().Locked = false;
                        Boards.Last().Phase = 0;
                        Boards.Last().Attenuation = 0;
                    }
                   
                }
                Board_interface.ItemsSource = Boards;
                Reg_CB_Board.ItemsSource = Boards;
                


            }
            else
            {
                System.Windows.MessageBox.Show("Invalid Config file");
                System.Windows.Application.Current.Shutdown();
            }
        }

        private void GlobalFrequency_ValueChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            Board.DefaultFrequency = GlobalFrequency.Value.GetValueOrDefault();

            foreach (Board PLL in Boards)
            {
                PLL.UpdateTargetFrequency();
            }

        }

        private void SerialBtnCheck(object sender, RoutedEventArgs e)
        {
                   

            if (mySerialPort.IsOpen)
            {
                try
                {
                    mySerialPort.Close();
  
                }
                catch
                {

                }
            }

            try
            {
                if (LoadCommPortSettings() < 1)
                {
                    MessageBox.Show("Invalid comm port settings");
                    return;
                }
                mySerialPort.DataBits = DEFAULT_PORT_DATABITS;
                mySerialPort.StopBits = DEFAULT_PORT_STOPBITS;
                mySerialPort.Parity = DEFAULT_PORT_PARITY;
                mySerialPort.BaudRate = DEFAULT_PORT_BAUDRATE;
                mySerialPort.PortName = CommPorts.SelectedValue.ToString();
                mySerialPort.Open();
                SerialEllipse.Fill = Brushes.Green;                
                SerialBtn.Content = "Disconnect";
                CommPorts.IsEnabled = false;
                
            }
            catch
            {
                SerialEllipse.Fill = Brushes.Red;
                MessageBox.Show("Serial Port error");
            }
            

            //SerialPort(String, Int32, Parity, Int32)

        }

        private void SerialBtnUnchecked(object sender, RoutedEventArgs e)
        {
            CommPorts.IsEnabled = true;
            SerialBtn.Content = "Connect";
            SerialEllipse.Fill = Brushes.Red;
        }

        private int LoadCommPortSettings()
        {
                    
            if(Enum.TryParse(Properties.Settings.Default.Parity,true,out DEFAULT_PORT_PARITY)== false)
            {
                return -1;
            }
            if (Int32.TryParse(Properties.Settings.Default.DataBits,out DEFAULT_PORT_DATABITS) == false)
            {
                return -1;
            }
            if (Enum.TryParse(Properties.Settings.Default.StopBits, true, out DEFAULT_PORT_STOPBITS) == false)
            {
                return -1;
            }
            if (Int32.TryParse(Properties.Settings.Default.Baudrate, out DEFAULT_PORT_BAUDRATE) == false)
            {
                return -1;
            }
            

            return 1;

        }

        private void BtnProgAll_Click(object sender, RoutedEventArgs e)
        {
            if(mySerialPort.IsOpen == false)
            {
                MessageBox.Show("Connect Comm Port");
                return;
            }

            foreach (Board PLL in Boards)
            {
                PLL.UpdateTargetFrequency();
            }
        }

        private void BtnProgSelected_Click(object sender, RoutedEventArgs e)
        {
            if (mySerialPort.IsOpen == false)
            {
                MessageBox.Show("Connect Comm Port");
                return;
            }
            foreach (Board PLL in Boards)
            {
                if (PLL.Enable_freq_Box)
                {
                    PLL.UpdateTargetFrequency();
                }

                    if (PLL.Enable_program_board)
                {                                    
                    PLL.SendAllRegisters(mySerialPort);
                }

            }
        }

        private void BtnCheckLockAll_Click(object sender, RoutedEventArgs e)
        {
            if (mySerialPort.IsOpen == false)
            {
                MessageBox.Show("Connect Comm Port");
                return;
            }

            foreach (Board PLL in Boards)
            {
                PLL.UpdateTargetFrequency();
            }
        }

        private void BtnCheckLockSelected_Click(object sender, RoutedEventArgs e)
        {
            if (mySerialPort.IsOpen == false)
            {
                MessageBox.Show("Connect Comm Port");
                return;
            }
            foreach (Board PLL in Boards)
            {
                if (PLL.Enable_program_board)
                {
                    PLL.UpdateTargetFrequency();
                }
            }
        }

        private void BtnPause_Click(object sender, RoutedEventArgs e)
        {
            // dummy button to allow code ot be pause
            int idx = Reg_CB_Board.SelectedIndex;
            int idx2 = Reg_CB_Board.SelectedIndex;



        }

        private void Reg_CB_Board_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

            Set_reg_dropdown();

        }

        private void Set_reg_dropdown()
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;

            CB_RFO.ItemsSource = Enum.GetNames(typeof(Register8.RFOS));
            CB_FILT.ItemsSource = Enum.GetNames(typeof(Register8.FILTS));
            CB_CP.ItemsSource = Enum.GetNames(typeof(Register9.CPs));
            CB_LKCT.ItemsSource = Enum.GetNames(typeof(Register9.LKCTs));
            CB_LKWIN.ItemsSource = Enum.GetNames(typeof(Register9.LKWINs));

            //var test = PLL.Reg09.LKWIN;
            //var test2 = Enum.GetName(typeof(Register9.LKWINs), PLL.Reg09.LKWIN);
            CB_RFO.SelectedIndex = CB_RFO.Items.IndexOf(Enum.GetName(typeof(Register8.RFOS), PLL.Reg08.RFO));
            CB_FILT.SelectedIndex = CB_FILT.Items.IndexOf(Enum.GetName(typeof(Register8.FILTS), PLL.Reg08.FILT));
            CB_CP.SelectedIndex = CB_CP.Items.IndexOf(Enum.GetName(typeof(Register9.CPs), PLL.Reg09.CP));
            CB_LKCT.SelectedIndex = CB_LKCT.Items.IndexOf(Enum.GetName(typeof(Register9.LKCTs), PLL.Reg09.LKCT));
            CB_LKWIN.SelectedIndex = CB_LKWIN.Items.IndexOf(Enum.GetName(typeof(Register9.LKWINs), PLL.Reg09.LKWIN));

        }

        private void CB_RFO_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            Register8.RFOS rfo;
            Enum.TryParse(CB_RFO.SelectedValue.ToString(), true, out rfo);
            PLL.Reg08.RFO = (byte)rfo;
            PLL.PropertyChangedReg08();
            //byte rfobyte = (byte)rfo;
        }

        private void CB_FILT_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            Register8.FILTS filt;
            Enum.TryParse(CB_FILT.SelectedValue.ToString(), true, out filt);
            PLL.Reg08.FILT = (byte)filt;
            PLL.PropertyChangedReg08();
        }

        //private void CB_OD_SelectionChanged(object sender, SelectionChangedEventArgs e)
        //{
        //    Board PLL = (Board)Reg_CB_Board.SelectedItem;
        //    Register8.ODS od;
        //    Enum.TryParse(CB_OD.SelectedValue.ToString(), true, out od);
        //    PLL.Reg08.OD = (byte)od;
        //}

        

        private void CB_CP_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            Register9.CPs cp;
            Enum.TryParse(CB_CP.SelectedValue.ToString(), true, out cp);
            PLL.Reg09.CP = (byte)cp;
            PLL.PropertyChangedReg09();
        }

        private void CB_LKCT_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            Register9.LKCTs  lkct;
            Enum.TryParse(CB_LKCT.SelectedValue.ToString(), true, out lkct);
            PLL.Reg09.LKCT = (byte)lkct;
            PLL.PropertyChangedReg09();
        }

        private void CB_LKWIN_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            Register9.LKWINs lkwin;
            Enum.TryParse(CB_LKWIN.SelectedValue.ToString(), true, out lkwin);
            PLL.Reg09.LKWIN = (byte)lkwin;
            PLL.PropertyChangedReg09();
        }

        private void REgALLBoards_Click(object sender, RoutedEventArgs e)
        {
            Board SelectedPLL = (Board)Reg_CB_Board.SelectedItem;
            foreach (Board PLL in Boards)
            {
                if (SelectedPLL.BoardNumber != PLL.BoardNumber)
                {
                    PLL.Reg01 = SelectedPLL.Reg01.ShallowCopy();
                    PLL.Reg02 = SelectedPLL.Reg02.ShallowCopy();
                    PLL.Reg07 = SelectedPLL.Reg07.ShallowCopy();
                    PLL.Reg08 = SelectedPLL.Reg08.ShallowCopy();
                    PLL.Reg09 = SelectedPLL.Reg09.ShallowCopy();
                    PLL.Reg0A = SelectedPLL.Reg0A.ShallowCopy();

                }
            }
        }

        //private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        //{
        //    if (Reg_tab.IsSelected)
        //    {
        //        if (Reg_CB_Board.SelectedIndex > 0 & Reg_CB_Board.SelectedIndex < Properties.Settings.Default.NumberOfBoards)
        //        {
        //            //Set_reg_dropdown();
                    
        //        }
        //    }
        //}

        private void BtnPhasePowerSelected_Click(object sender, RoutedEventArgs e)
        {
            if (mySerialPort.IsOpen == false)
            {
                MessageBox.Show("Connect Comm Port");
                return;
            }
            foreach (Board PLL in Boards)
            {
                if (PLL.Enable_program_board)
                {
                
                    PLL.SendPhase(mySerialPort);
                    Thread.Sleep(5);
                    PLL.SendPower(mySerialPort);
                    Thread.Sleep(5);
                }
            }
        }

        private void BtnPhasePowerAll_Click(object sender, RoutedEventArgs e)
        {
            if (mySerialPort.IsOpen == false)
            {
                MessageBox.Show("Connect Comm Port");
                return;
            }
            foreach (Board PLL in Boards)
            {
                PLL.SendPhase(mySerialPort);
                Thread.Sleep(5);
                PLL.SendPower(mySerialPort);
                Thread.Sleep(5);
            }
        }

        private void BtnMute_Click(object sender, RoutedEventArgs e)
        {
            if (mySerialPort.IsOpen == false)
            {
                MessageBox.Show("Connect Comm Port");
                return;
            }
            CheckBox cb = (CheckBox)sender;
            Board pll = (Board)cb.DataContext;
            pll.SendRegisters(mySerialPort, 1);
            Thread.Sleep(5);
            pll.SendMute(mySerialPort);
            //// BoardNumber = .DA
            //foreach (Board PLL in Boards)
            //{

            //    PLL.SendRegisters(mySerialPort,1);
            //    Thread.Sleep(5);
            //}

        }

 

        private void Attenuation_TargetUpdated(object sender, DataTransferEventArgs e)
        {
          
        }

        private void Reg01_Click(object sender, RoutedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            PLL.PropertyChangedReg01();
        }

        private void Reg02_Click(object sender, RoutedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            PLL.PropertyChangedReg02();
        }
        private void Reg03_Click(object sender, RoutedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            PLL.PropertyChangedReg03();
        }
        private void Reg04_Click(object sender, RoutedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            PLL.PropertyChangedReg04();
        }
        private void Reg05_Click(object sender, RoutedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            PLL.PropertyChangedReg05();
        }
        private void Reg06_Click(object sender, RoutedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            PLL.PropertyChangedReg06();
        }

        private void Reg07_Click(object sender, RoutedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            PLL.PropertyChangedReg07();
        }

        private void Reg08_Click(object sender, RoutedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            PLL.PropertyChangedReg08();
        }

        private void Reg09_Click(object sender, RoutedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            PLL.PropertyChangedReg09();
        }

        private void Reg0A_Click(object sender, RoutedEventArgs e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            PLL.PropertyChangedReg0A();
        }

     

        private void N_ValueChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            PLL.PropertyChangedReg05();
            PLL.PropertyChangedReg06();
        }

        private void R_ValueChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            PLL.PropertyChangedReg03();
            PLL.PropertyChangedReg04();
        }

        private void O_ValueChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            PLL.PropertyChangedReg08();
        }

        private void B_ValueChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            Board PLL = (Board)Reg_CB_Board.SelectedItem;
            PLL.PropertyChangedReg03();
        }





        //private void enable_freq(object sender, RoutedEventArgs e)
        //{
        //    Board vm = (sender as FrameworkElement).DataContext;


        //    if (vm.Enable_freq_Box == true)
        //    {
        //        vm.Enable_freq_Box = false;
        //    } else
        //    {
        //        vm.Enable_freq_Box = true;
        //    }
        //}
    }
}
