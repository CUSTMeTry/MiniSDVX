using System.Collections.Generic;
using System.Diagnostics;
using System.Management;
using System.Printing;

namespace MiniSDVX_Windows.Helper
{


public class SerialClass
{
    /// <summary>
    /// 获取串口们
    /// </summary>
    /// <returns> 天书串口类型</returns>
    /// 

    public static string GetPorts()
    {

        using (ManagementObjectSearcher searcher = new ManagementObjectSearcher("SELECT * FROM Win32_PnPEntity")) //调用 WMI，获取 Win32_PnPEntity，即所有设备
        {
            var hardInfos = searcher.Get();
            foreach (var hardInfo in hardInfos)
            {
                if (hardInfo.Properties["Name"].Value != null){
                    if (hardInfo.Properties["Name"].Value.ToString()!.Contains("COM"))
                    {
                        string name = hardInfo.Properties["Name"].Value + "";
                        if(hardInfo.Properties["DeviceID"].Value != null && (hardInfo.Properties["DeviceID"].Value+"").Contains("VID_0483&PID_52A4")){
                        Debug.WriteLine("-----" + name + "------");
                        int p = name.IndexOf('(');
                            if (name.Contains("COM"))
                            {
                                return name.Substring(p + 1, name.Length - p - 2);
                            }
                        }
                    }
                }
            }
            searcher.Dispose();
        }

        return "";
    }
}
}