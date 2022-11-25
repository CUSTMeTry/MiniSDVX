using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Ports;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;
using System.Xml.Linq;

namespace MiniSDVX_Windows.Helper
{
    public class KeyCoV
    {
        public int KeyCode { get; set; }
        public int KeyValue { get; set; }
        public string Name { get; set; }

        public KeyCoV(int c, int v, string name)
        {
            this.KeyCode = c;
            this.KeyValue = v;
            this.Name = name;
        }

        public KeyCoV(Key c, int v, string name)
        {
            this.KeyCode = (int)c;
            this.KeyValue = v;
            this.Name = name;
        }

        public KeyCoV(Key c, GeneralUtils.KeyV v, string name)
        {
            this.KeyCode = (int)c;
            this.KeyValue = (int)v;
            this.Name = name;
        }
    }


    public class GeneralUtils
    {
        public enum KeyV
        {
            // Modifiers
            KEY_LEFT_CTRL = 0xe0,
            KEY_LEFT_SHIFT = 0xe1,
            KEY_LEFT_ALT = 0xe2,
            KEY_LEFT_GUI = 0xe3,
            KEY_RIGHT_CTRL = 0xe4,
            KEY_RIGHT_SHIFT = 0xe5,
            KEY_RIGHT_ALT = 0xe6,
            KEY_RIGHT_GUI = 0xe7,

            // Misc keys
            KEY_UP_ARROW = 0x52,
            KEY_DOWN_ARROW = 0x51,
            KEY_LEFT_ARROW = 0x50,
            KEY_RIGHT_ARROW = 0x4f,
            KEY_BACKSPACE = 0x2a,
            KEY_TAB = 0x2b,
            KEY_RETURN = 0x28,
           // KEY_MENU = 0xED,// "Keyboard Application" in USB standard
            KEY_ESC = 0x29,
            KEY_INSERT = 0x19,
            KEY_DELETE = 0x4c,
            KEY_PAGE_UP = 0x4b,
            KEY_PAGE_DOWN = 0x4e,
            KEY_HOME = 0x4a,
            KEY_END = 0x4d,
            KEY_CAPS_LOCK = 0x39,
            KEY_PRINT_SCREEN = 0x46,// Print Screen / SysRq
            KEY_SCROLL_LOCK = 0x47,
            KEY_PAUSE = 0x48,// Pause / Break

            // Numeric keypad
            KEY_NUM_LOCK = 0x53,
            KEY_KP_SLASH = 0x54,
            KEY_KP_ASTERISK = 0x55,
            KEY_KP_MINUS = 0x56,
            KEY_KP_PLUS = 0x57,
            KEY_KP_ENTER = 0x58,
            KEY_KP_DOT = 0x63,
        }

        //public Dictionary<int, string> KeyDic = new Dictionary<int, string>();
        public List<KeyCoV> KeyCovList = new List<KeyCoV>();
        public static bool isPortOpen = false;
        public delegate void MessageSendDelegate(string message);
        public SerialPort sp = new SerialPort();

        public GeneralUtils()
        {
            int j;

            // A-Z转换

            j = 0;
            for (int i = 44; i < 70; i++)
            {
                KeyCovList.Add(new(i, j + 0x04, ((char)(j + 65)).ToString()));
                j++;

            }


            // 数字转换

            j = 0;
            for (int i = 34; i < 44; i++)
            {
                KeyCovList.Add(new KeyCoV(i, j + 0x1e, j.ToString()));
                j++;

            }


            //小键盘数字转换

            j = 0;
            for (int i = 74; i < 84; i++)
            {
                KeyCovList.Add(new KeyCoV(i, j + 0x59, "Num" + j));
                j++;
            }


            //Function转换

            j = 0;
            for (int i = 90; i < 102; i++)
            {
                KeyCovList.Add(new KeyCoV(i, j + 0x3a, "F" + (j + 1)));
                j++;

            }

            KeyCovList.Add(new KeyCoV(Key.Back, KeyV.KEY_BACKSPACE, "BACKSPACE"));
            KeyCovList.Add(new KeyCoV(Key.Tab, KeyV.KEY_TAB, "TAB"));
            KeyCovList.Add(new KeyCoV(Key.Enter, KeyV.KEY_RETURN, "ENTER"));
            KeyCovList.Add(new KeyCoV(Key.LeftShift, KeyV.KEY_LEFT_SHIFT, "LSHIFT"));
            KeyCovList.Add(new KeyCoV(Key.RightShift, KeyV.KEY_RIGHT_SHIFT, "RSHIFT"));
            KeyCovList.Add(new KeyCoV(Key.LeftCtrl, KeyV.KEY_LEFT_CTRL, "LCTRL"));
            KeyCovList.Add(new KeyCoV(Key.RightCtrl, KeyV.KEY_RIGHT_CTRL, "RCTRL"));
            KeyCovList.Add(new KeyCoV(Key.LeftAlt, KeyV.KEY_LEFT_ALT, "LALT"));
            KeyCovList.Add(new KeyCoV(Key.RightAlt, KeyV.KEY_RIGHT_ALT, "RALT"));
            KeyCovList.Add(new KeyCoV(Key.CapsLock, KeyV.KEY_CAPS_LOCK, "CPAS LOCK"));
            KeyCovList.Add(new KeyCoV(Key.Up, KeyV.KEY_UP_ARROW, "UP"));
            KeyCovList.Add(new KeyCoV(Key.Down, KeyV.KEY_DOWN_ARROW, "DOWN"));
            KeyCovList.Add(new KeyCoV(Key.Left, KeyV.KEY_LEFT_ARROW, "LEFT"));
            KeyCovList.Add(new KeyCoV(Key.Right, KeyV.KEY_RIGHT_ARROW, "RIGHT"));
            KeyCovList.Add(new KeyCoV(Key.Insert, KeyV.KEY_INSERT, "INSERT"));
            KeyCovList.Add(new KeyCoV(Key.Delete, KeyV.KEY_DELETE, "DELETE"));
            KeyCovList.Add(new KeyCoV(Key.NumLock, KeyV.KEY_NUM_LOCK, "NUM LOCK"));
            KeyCovList.Add(new KeyCoV(Key.PrintScreen, KeyV.KEY_PRINT_SCREEN, "PS"));
            KeyCovList.Add(new KeyCoV(Key.Home, KeyV.KEY_HOME, "Home"));
            KeyCovList.Add(new KeyCoV(Key.End, KeyV.KEY_END, "END"));
            KeyCovList.Add(new KeyCoV(Key.PageUp, KeyV.KEY_PAGE_UP, "PAGE UP"));
            KeyCovList.Add(new KeyCoV(Key.PageDown, KeyV.KEY_PAGE_DOWN, "PAGE DOWN"));

            KeyCovList.Add(new KeyCoV(Key.Add, KeyV.KEY_KP_PLUS, "+"));
            KeyCovList.Add(new KeyCoV(Key.Multiply, KeyV.KEY_KP_ASTERISK, "*"));
            KeyCovList.Add(new KeyCoV(Key.Subtract, KeyV.KEY_KP_MINUS, "-"));
            KeyCovList.Add(new KeyCoV(Key.Divide, KeyV.KEY_KP_SLASH, "/"));
            KeyCovList.Add(new KeyCoV(Key.Decimal, KeyV.KEY_KP_DOT, "."));

            KeyCovList.Add(new KeyCoV(Key.Escape, KeyV.KEY_ESC, "ESC"));

                        KeyCovList.Add(new KeyCoV(Key.Oem1, 0x33, ";:"));
            KeyCovList.Add(new KeyCoV(Key.OemPlus, 0x2e, "=+"));

            KeyCovList.Add(new KeyCoV(Key.OemComma, 0x36, ",<"));
            KeyCovList.Add(new KeyCoV(Key.OemPeriod, 0x37, ".>"));

            KeyCovList.Add(new KeyCoV(Key.OemMinus, 0x2d, "-_"));
            KeyCovList.Add(new KeyCoV(Key.Oem3, 0x35, "`~"));
            KeyCovList.Add(new KeyCoV(Key.OemQuestion, 0x38, "/?"));

            KeyCovList.Add(new KeyCoV(Key.OemOpenBrackets, 0x2f, "[{"));
            KeyCovList.Add(new KeyCoV(Key.Oem6, 0x30, "]}"));

            KeyCovList.Add(new KeyCoV(Key.Oem5, 0x31, "\\|"));
            KeyCovList.Add(new KeyCoV(Key.OemQuotes, 0x34, "\'\""));

            KeyCovList.Add(new KeyCoV(Key.Space, 0x2c, "SPACE"));
        }

        public int ConnectMiniSDVXPort()
        {
            string ports = SerialClass.GetPorts();
            if (ports == "")
            {
                return 1;
            }
            sp = new SerialPort
            {
                PortName = ports,
                BaudRate = 512000,
                Parity = Parity.None,
                StopBits = StopBits.One
            };
            sp.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);

            try
            {
                sp.Open();
            }
            catch
            {
                return 2;
            }


            return 0;
        }

        public int SendSDVXMessage(DataPack dataPack)
        {
            if (dataPack == null || dataPack.KeyItems == null)
            {
                return 1;
            }
            dataPack.KeyItems.Sort();
            byte[] data = new byte[10];

            //默认写入指令
            data[0] = 0xfc;

            if (dataPack.KeyItems != null)
            {
                //第一次发送
                data[1] = 0;
                for (int i = 0; i < dataPack.KeyItems.Count && i < 8; i++)
                {
                    data[i + 2] = (byte)dataPack.KeyItems[i].KeyCode;
                }
                for (int i = dataPack.KeyItems.Count; i < 8; i++)
                {
                    data[i + 2] = 0;
                }
                if (sp.IsOpen)
                {
                    foreach (var item in data)
                    {
                        Debug.Write(item + " ");
                    }
                    Debug.WriteLine(" ");

                    sp.Write(data, 0, 10);
                }

                //延迟10毫秒
                Task.Delay(10);

                //第二次发送
                data[1] = 8;
                if (dataPack.KeyItems.Count >= 8)
                {
                    data[2] = (byte)dataPack.KeyItems[8].KeyCode;
                }
                else
                {
                    data[2] = 0;
                }
                data[3] = (byte)(int)(dataPack.LEncoder);
                data[4] = (byte)(int)(dataPack.REncoder);
                if (sp.IsOpen)
                {
                    foreach (var item in data)
                    {
                        Debug.Write(item + " ");
                    }
                    Debug.WriteLine(" ");

                    sp.Write(data, 0, 10);
                }

                //延迟10毫秒
                Task.Delay(10);

                //第三次发送
                data[1] = 1;
                if (sp.IsOpen)
                {
                    foreach (var item in data)
                    {
                        Debug.Write(item + " ");
                    }
                    Debug.WriteLine(" ");

                    sp.Write(data, 0, 10);
                    return 0;
                }

            }
            return 2;
        }

        public int ReadSDVXMessage()
        {
            //Debug.WriteLine(sp.IsOpen);
            byte[] data = { 0xfe };
            if (sp.IsOpen)
            {
                Debug.WriteLine("Send Read Key");
                sp.Write(data, 0, 1);
            }
            return 0;
        }

        private static void DataReceivedHandler(
                       object sender,
                       SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            int key = sp.ReadByte();
            Debug.WriteLine("GET KEY: " + key);
            if (key == 0xfe)
            {
                byte[] recivedData = new byte[11];
                sp.Read(recivedData, 0, 11);

                foreach (var item in recivedData)
                {
                    Debug.Write(item + " ");
                }

                Debug.WriteLine("");

                _ = Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Normal, new Action(() =>
                {
                    MainWindow mw = (MainWindow)Application.Current.MainWindow;
                    if (mw.dataPack.KeyItems != null)
                    {
                        mw.dataPack.KeyItems.Clear();
                        for (int i = 0; i < 9; i++)
                        {
                            string keyName = "";
                            foreach (var item in mw.gu.KeyCovList)
                            {
                                if(item.KeyValue == recivedData[i])
                                {
                                    keyName = item.Name;
                                }
                            }
                            mw.dataPack.KeyItems.Add(new KeyItem
                            {
                                Name = "K" + (i + 1),
                                KeyCode = recivedData[i],
                                KeyString = keyName
                            });

                        }
                    }
                    mw.dataPack.LEncoder = recivedData[9];
                    mw.dataPack.REncoder = recivedData[10];
                    mw.DataRefreshAfter();
                }));
            }
            //string indata = sp.ReadExisting();
            //Debug.WriteLine("Data Received:");
            //Debug.Write(indata);
        }

        public static bool WriteJson(DataPack dataPack, string path, MessageSendDelegate ms)
        {
            string jsondata = JsonConvert.SerializeObject(dataPack);  //class类转string
            try
            {
                using (StreamWriter sw = new(path))  //将string 写入json文件
                {
                    sw.WriteLine(jsondata);
                }
                return true;
            }
            catch (UnauthorizedAccessException)
            {
                ms("文件写入权限不够");
            }
            catch (PathTooLongException)
            {
                ms("路径名实在是太长了");
            }
            catch (ArgumentException)
            {
                ms("没有得到正确的路径");
            }
            catch (DirectoryNotFoundException)
            {
                ms("找不到文件路径");
            }
            catch (IOException)
            {
                ms("文件写入失败，检查文件是否被占用");
            }

            return false;
        }

        public static DataPack? ReadJson(string path, MessageSendDelegate ms)
        {
            string datacache = "";
            try
            {
                using (StreamReader r1 = new(path))//读取json文件
                {
                    string? line;
                    while ((line = r1.ReadLine()) != null)
                    {
                        datacache += line;
                    }

                    DataPack? rt = JsonConvert.DeserializeObject<DataPack>(datacache);
                    if (rt != null)
                    {
                        return rt;
                    }
                    else
                    {
                        ms("文件格式出现了问题");
                    }
                }
            }
            catch (UnauthorizedAccessException)
            {
                ms("文件写入权限不够");
            }
            catch (PathTooLongException)
            {
                ms("路径名实在是太长了");
            }
            catch (ArgumentException)
            {
                ms("没有得到正确的路径");
            }
            catch (DirectoryNotFoundException)
            {
                ms("找不到文件路径");
            }
            catch (IOException)
            {
                ms("文件写入失败，检查文件是否被占用");
            }
            catch (OutOfMemoryException)
            {
                ms("内存不足");
            }
            return null;
        }

    }

}



