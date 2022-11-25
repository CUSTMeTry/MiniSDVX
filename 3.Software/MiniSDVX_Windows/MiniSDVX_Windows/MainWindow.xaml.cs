using HandyControl.Controls;
using HandyControl.Data;
using HandyControl.Tools.Extension;
using Microsoft.Win32;
using MiniSDVX_Windows.Helper;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Threading;

namespace MiniSDVX_Windows
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow
    {
        public bool isPortOpen = false;
        string keyboardData = "";
        private Button? currentButton;
        private Button? connectButton;

        private Button? writeButton;
        private Button? readButton;
        int ret;
        private static bool dataOK;
        private DispatcherTimer mDataTimer = new();

        public DataPack dataPack = new()
        {
            KeyItems = new List<KeyItem>(),
            LEncoder = 5,
            REncoder = 5
        };
        public GeneralUtils gu = new();

        public MainWindow()
        {
            InitializeComponent();
            mDataTimer.Tick += new EventHandler(RefreshPortsAsync);
            mDataTimer.Interval = TimeSpan.FromMilliseconds(1000);
            mDataTimer.Start();
            connectButton = (Button?)FindName("ConnectButton");
            writeButton = (Button?)FindName("WriteButton");
            readButton = (Button?)FindName("ReadButton");
        }

        private void CircleProgressBar_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            Slider? slider = (Slider?)sender;
            if (slider != null)
            {
                if (slider.Name == "LEncSlider")
                {
                    dataPack.LEncoder = e.NewValue;
                }
                else if (slider.Name == "REncSlider")
                {
                    dataPack.REncoder = e.NewValue;
                }
            }
        }
        private void Grid_KeyUp(object sender, KeyEventArgs e)
        {
            //Debug.WriteLine("-------------Keyup-----------------");
            Key a = e.Key;
            if (currentButton != null)
            {
                if(e.Key == Key.System)
                {
                    a = e.SystemKey;
                }

                int b = 0;
                keyboardData = "";
                for (int i = 0; i < gu.KeyCovList.Count; i++)
                {
                    if((int)a == gu.KeyCovList[i].KeyCode)
                    {
                        keyboardData = gu.KeyCovList[i].Name;
                        b = gu.KeyCovList[i].KeyValue;
                    }
                }

                int fontsize = 50;
                if(keyboardData.Length > 8)
                {
                    fontsize = 10;
                }
                else if (keyboardData.Length > 5)
                {
                    fontsize = 15;
                }
                else if(keyboardData.Length > 2)
                {
                    fontsize = 20;
                }
                


                currentButton.FontSize = fontsize;
                currentButton.Content = keyboardData;
                foreach (KeyItem k in dataPack.KeyItems!)
                {
                    if (k.Name == currentButton.Name)
                    {
                        k.KeyCode = b;
                        k.KeyString = keyboardData;
                        return;
                    }
                }
                KeyItem keyData = new()
                {
                    Name = currentButton.Name,
                    KeyCode = b,
                    KeyString = keyboardData
                };
                dataPack.KeyItems!.Add(keyData);
            }
        }


        private void OpenDraw(object sender, RoutedEventArgs e)
        {
            DrawerLeft.IsOpen = true;
        }

        private void KClick(object sender, RoutedEventArgs e)
        {
            if (currentButton != null)
            {
                currentButton!.BorderBrush = (Brush)FindResource("BorderBrush");

            }
            currentButton = (Button?)sender;
            currentButton!.BorderBrush = (Brush)FindResource("PrimaryBrush");
        }

        private void ConnectSDVX(object sender, RoutedEventArgs e)
        {
            if (ret == 1)
            {
                SendWarningMessage("未找到适配的MiniSDVX手台");
                return;
            }
            else if (ret == 2)
            {
                SendWarningMessage("MiniSDVX已和另一个进程建立通讯");
                return;
            }

            SendSuccessMessage("已连接到MiniSDVX手台");
        }

        private void ClearCurrentButton(object sender, RoutedEventArgs e)
        {
            if (currentButton != null)
            {
                currentButton.BorderBrush = (Brush)FindResource("BorderBrush");

            }
            currentButton = null;
        }

        private void WriteSDVX(object sender, RoutedEventArgs e)
        {
            if (!gu.sp.IsOpen)
            {
                SendSuccessMessage("还未连接到MiniSDVX的说");
            }

            if (gu.SendSDVXMessage(dataPack) == 0)
            {
                SendSuccessMessage("写入成功");
            }
            else
            {
                SendWarningMessage("写入失败");
            }

        }

        public void DataRefreshAfter()
        {
            if (dataPack == null || dataPack.KeyItems == null)
            {
                return;
            }
            for (int i = 0; i < 9; i++)
            {

                if (dataPack.KeyItems[i] != null && dataPack.KeyItems[i].Name != null)
                {
                    Button thisButton = (Button)FindName(dataPack.KeyItems[i].Name);
                    //Debug.WriteLine(dataPack.KeyItems[i].KeyCode);

                    if (dataPack.KeyItems[i].KeyCode != 0)
                    {

                        if (dataPack.KeyItems[i].KeyString != null)
                        {
                            int fontsize = 50;
                            if (keyboardData.Length > 8)
                            {
                                fontsize = 10;
                            }
                            else if (keyboardData.Length > 5)
                            {
                                fontsize = 15;
                            }
                            else if (keyboardData.Length > 2)
                            {
                                fontsize = 20;
                            }
                            thisButton.FontSize = fontsize;
                            thisButton.Content = dataPack.KeyItems[i].KeyString;
                        }
                    }
                    else
                    {
                        thisButton.Content = dataPack.KeyItems[i].Name;

                    }

                }
            }
            LEncSlider.Value = dataPack.LEncoder;
            REncSlider.Value = dataPack.REncoder;
            SendSuccessMessage("数据读取成功");
            return;

        }

        private void ReadSDVX(object sender, RoutedEventArgs e)
        {
            gu.ReadSDVXMessage();

        }

        private static void SendSuccessMessage(string message)
        {
            GrowlInfo growlInfo = new GrowlInfo() { WaitTime = 1, Message = message };
            Growl.Success(growlInfo);
        }

        private static void SendWarningMessage(string message)
        {
            GrowlInfo growlInfo = new GrowlInfo() { WaitTime = 1, Message = message };
            Growl.Warning(growlInfo);
        }

        private void SetJson(object sender, RoutedEventArgs e)
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.Filter = "MiniSDVX JSON files(*.json)|*.json";
            saveFileDialog.FileName = "MiniSDVX";
            saveFileDialog.FilterIndex = 2;
            saveFileDialog.RestoreDirectory = true;
            if (saveFileDialog.ShowDialog() == true)
            {
                if (GeneralUtils.WriteJson(dataPack, saveFileDialog.FileName, SendWarningMessage))
                {
                    SendSuccessMessage("文件写入成功");
                }
            }

        }
        private void GetJson(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "MiniSDVX JSON files(*.json)|*.json";
            openFileDialog.FilterIndex = 2;
            openFileDialog.RestoreDirectory = true;
            if (openFileDialog.ShowDialog() == true)
            {
                DataPack? dp = GeneralUtils.ReadJson(openFileDialog.FileName, SendWarningMessage);
                if (dp != null && dp.KeyItems != null)
                {
                    dataPack = dp;
                    foreach (KeyItem keyItem in dp.KeyItems)
                    {
                        Button? btn = (Button?)FindName(keyItem.Name);
                        if (btn != null && keyItem.KeyString != null)
                        {
                            int fontsize = 50;
                            while (fontsize * keyItem.KeyString.Length > 110 && fontsize > 10)
                            {
                                fontsize -= 2;
                            }
                            btn.FontSize = fontsize;
                            btn.Content = keyItem.KeyString;


                        }
                    }
                    LEncSlider.Value = dataPack.LEncoder;
                    REncSlider.Value = dataPack.REncoder;
                    SendSuccessMessage("读取成功");
                    return;
                }
                SendWarningMessage("读取失败");
            }
        }

        private async void RefreshPortsAsync(object sender, EventArgs e)
        {
            await Task.Run(() =>
            {
                if (connectButton != null)
                {


                    connectButton.Dispatcher.Invoke(new Action(delegate
                    {
                        if (writeButton != null)
                        {
                            writeButton.IsEnabled = gu.sp.IsOpen;

                        }
                        if (readButton != null)
                        {
                            readButton.IsEnabled = gu.sp.IsOpen;

                        }
                        if (gu.sp.IsOpen)
                        {
                            ConnectButton.Style = (Style)FindResource("ButtonDashedSuccess");
                            connectButton.Content = "已连接到MiniSDVX设备";
                        }
                        else
                        {
                            connectButton.Content = "未连接到MiniSDVX设备";
                            ret = gu.ConnectMiniSDVXPort();
                            if (ret == 2)
                            {
                                ConnectButton.Style = (Style)FindResource("ButtonDashedDanger");
                            }
                            else if (ret != 0)
                            {
                                ConnectButton.Style = (Style)FindResource("ButtonDashedWarning");
                            }
                        }
                    }), null);
                }
            });
        }

        protected override void OnMouseDoubleClick(MouseButtonEventArgs e)
        {
            base.OnMouseDoubleClick(e);
            if (e.ChangedButton == MouseButton.Right)
            {
                DrawerLeft.IsOpen = true;
            }


        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);


            // 获取鼠标相对标题栏位置  
            //Point position = e.GetPosition(TittleBar);


            // 如果鼠标位置在标题栏内，允许拖动  
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                this.DragMove();

            }
        }


    }
}

