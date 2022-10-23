using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Diagnostics;
using Newtonsoft.Json;
using System.Reflection.Metadata;
using System.IO;

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

        public int connectMiniSDVXPort()
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


        private static void DataReceivedHandler(
                       object sender,
                       SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            string indata = sp.ReadExisting();
            Debug.WriteLine("Data Received:");
            Debug.Write(indata);
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
