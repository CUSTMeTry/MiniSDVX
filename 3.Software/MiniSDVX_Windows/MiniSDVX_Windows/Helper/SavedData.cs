using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiniSDVX_Windows.Helper
{
    public class KeyItem : IComparable<KeyItem>
    {
        public string? Name { get; set; }
        public string? KeyString { get; set; }
        public int KeyCode { get; set; }

        public int CompareTo(KeyItem _obj)
        {
            if(string.Compare(Name, _obj.Name, true) == 1)
                return 1;
            else
                return -1;
        }

        public override string ToString()
        {
            //return '-' + KeyCode;
            return "-" + KeyCode;
        }
    }

    public class DataPack
    {
        public List<KeyItem>? KeyItems { get; set; }
        public double LEncoder { get; set; }
        public double REncoder { get; set; }

        public override string ToString()
        {
            string _ = "";
            foreach (var item in KeyItems)
            {
      
                    _ += item.ToString();
                
            }
            return _;
        }
    }

    public class SavedData
    {
        public DataPack? Data { get; set; }
    }

}
