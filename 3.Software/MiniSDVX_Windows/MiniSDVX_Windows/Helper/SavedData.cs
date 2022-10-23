using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiniSDVX_Windows.Helper
{
    public class KeyItem
    {
        public string? Name { get; set; }
        public string? KeyString { get; set; }
        public int? KeyCode { get; set; }
    }

    public class EncoderItem
    {
        public string? Name { get; set; }
        public int? Value { get; set; }

    }

    public class DataPack
    {
        public List<KeyItem>? KeyItems { get; set; }
        public double LEncoder { get; set; }
        public double REncoder { get; set; }
    }

    public class SavedData
    {
        public DataPack? Data { get; set; }
    }

}
