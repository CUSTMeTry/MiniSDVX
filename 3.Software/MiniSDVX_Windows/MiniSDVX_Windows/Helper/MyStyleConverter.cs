using System;
using System.Globalization;
using System.Windows.Data;

namespace MiniSDVX_Windows.Helper
{
    public class MyStyleConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if ((bool)value)
            {
                return "{StaticResource ButtonDashedSuccess}";
            }
            else
            {
                return "{StaticResource ButtonDashedWarning}";
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            throw new NotImplementedException();
        }
    }
}
