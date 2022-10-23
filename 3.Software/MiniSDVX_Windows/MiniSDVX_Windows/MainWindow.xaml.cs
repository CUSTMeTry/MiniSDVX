using System;
using System.Collections.Generic;

using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;

using System.Windows.Input;
using System.Windows.Media;

using MiniSDVX_Windows.Helper;

using HandyControl.Controls;
using HandyControl.Data;

using System.Windows.Threading;
using Microsoft.Win32;

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
        int ret;
        private DispatcherTimer mDataTimer = new();

        private DataPack dataPack = new()
        {
            KeyItems = new List<KeyItem>(),
            LEncoder = 0,
            REncoder = 0
        };
        public GeneralUtils gu = new();

        public MainWindow()
        {
            InitializeComponent();
            mDataTimer.Tick += new EventHandler(RefreshPortsAsync);
            mDataTimer.Interval = TimeSpan.FromMilliseconds(1000);
            mDataTimer.Start();
            connectButton = (Button?)FindName("ConnectButton");
        }

        private void CircleProgressBar_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            Slider? slider = (Slider?)sender;
            if(slider != null)
            {
                if(slider.Name == "LEncSlider")
                {
                    dataPack.LEncoder = e.NewValue;
                }else if(slider.Name == "REncSlider")
                {
                    dataPack.REncoder = e.NewValue;
                }
            }
        }
        private void Grid_KeyUp(object sender, KeyEventArgs e)
        {
            if (currentButton != null)
            {
                int b = (int)e.Key;
                keyboardData = e.Key.ToString();
                  
                int fontsize = 50;
                while (fontsize * keyboardData.Length > 110 && fontsize > 10)
                {
                    fontsize -= 2;
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
            if(currentButton != null)
            {
                currentButton.BorderBrush = (Brush)FindResource("BorderBrush");

            }
            currentButton = null;
        }

        private void WriteSDVX(object sender, RoutedEventArgs e)
        {

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
            if (saveFileDialog.ShowDialog() == true) {
                if(GeneralUtils.WriteJson(dataPack, saveFileDialog.FileName, SendWarningMessage))
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
                    foreach(KeyItem keyItem in dp.KeyItems)
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
                        if (gu.sp.IsOpen)
                        {
                            ConnectButton.Style = (Style)FindResource("ButtonDashedSuccess");
                            connectButton.Content = "已连接到MiniSDVX设备";
                        }
                        else
                        {
                            connectButton.Content = "未连接到MiniSDVX设备";
                            ret = gu.connectMiniSDVXPort();
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
            if(e.ChangedButton == MouseButton.Right)
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

