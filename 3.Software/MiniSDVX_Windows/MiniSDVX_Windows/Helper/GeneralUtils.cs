using System;
using System.Collections.Generic;
using System.Windows.Input;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Diagnostics;
using Newtonsoft.Json;
using System.IO;
using System.Runtime.Intrinsics.Arm;
using System.Windows.Threading;
using System.Windows;


namespace MiniSDVX_Windows.Helper
{

    public class GeneralUtils
    {
        public static bool isPortOpen = false;
        public delegate void MessageSendDelegate(string message);
        public SerialPort sp = new SerialPort();
        public static int GetTypeCode()
        { 
            return 0;
        }

        public int ConnectMiniSDVXPort()
        {
            string ports = SerialClass.GetPorts();
            if(ports == "")
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
                for(int i = dataPack.KeyItems.Count; i < 8; i++)
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
            byte[] data = {0xfe};
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
                    if(mw.dataPack.KeyItems != null)
                    {
                        mw.dataPack.KeyItems.Clear();
                        for (int i = 0; i < 9; i++)
                        {
                            mw.dataPack.KeyItems.Add(new KeyItem
                            {
                                Name = "K" + (i + 1),
                                KeyCode = recivedData[i],
                                KeyString = ((Key)recivedData[i]).ToString()
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
            catch(ArgumentException)
            {
                ms("没有得到正确的路径");
            }
            catch (DirectoryNotFoundException)
            {
                ms("找不到文件路径");
            }catch (IOException)
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
            }catch (OutOfMemoryException)
            {
                ms("内存不足");
            }
            return null;
        }
        }

}
