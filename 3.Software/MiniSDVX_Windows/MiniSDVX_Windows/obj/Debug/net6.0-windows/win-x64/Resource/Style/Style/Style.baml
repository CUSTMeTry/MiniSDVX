   M S B A M L   `   `   `  ����S  OHandyControl, Version=3.3.0.0, Culture=neutral, PublicKeyToken=45be8712787a1e5bS9clr-namespace:HandyControl.Controls;assembly=HandyControlHandyControl.Controls  5      W SPresentationCore, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35e aSystem.Windows.Controls.Ribbon, Version=6.0.2.0, Culture=neutral, PublicKeyToken=b77a5c561934e089R NWindowsBase, Version=6.0.2.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NSystem.Xaml, Version=6.0.2.0, Culture=neutral, PublicKeyToken=b77a5c561934e089   MyToggleButton  ToggleButtonBaseStyle  �� BasedOn �� 
TargetType �� Property  RegionBrush �� Value  PrimaryTextBrush �� Triggers P� Property P� Value  BasePrimaryBrush  PrimaryBrush  CirclePanelButton  ButtonCustom �� 
TargetType  CirclePanelDemoGeometry 	 DefaultBrush 
 CirclePanelRightGeometry �� Stretch	 �� Triggers  MessageBoxCustom&     HandyControl.Controls.MessageBox"   HandyControl.Controls.Window  DemoInfoSortButtonStyle  ToggleButtonCustom
 �� CornerRadius  BorderBrush/   )HandyControl.Controls.StatusSwitchElement  Custom1Transition W� From	 W� To g� Duration �� TargetProperty W� EasingFunction0  *System.Windows.Media.Animation.ElasticEase   Oscillations  Custom2Transition  Custom3Transition  ListBoxItemTransparent  ListBoxItemBaseStyle  MyButton  SecondaryTextBrush  ListBoxTransparent  ListBoxBaseStyle�� 5      F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     :x,http://schemas.microsoft.com/winfx/2006/xaml     5      Fcontrols9clr-namespace:HandyControl.Controls;assembly=HandyControl  5      0hc'https://handyorg.github.io/handycontrol   5   	   %�  &	        7  &	 [    7  7  7 
 &	 )    7  &	 %   7  &	 �   &	 �   &	 �	   &	 �
    7  &	 �
    &	 �    7  7  �� 5      8    " d�6T   .�5   
   ��  � ��# �  6'   �� 5	   
    � ��# �  6'   �� 5
   
    � ��$ 0q�6,    5   
   P� 5       �@J�IsMouseOver$
 True��6-   . �5      ��  � ��# �  6/   5      P� 5       �@d�	IsChecked$
 TrueY�6+   . �5      ��  � ��# �  6/   5      5      �� 5      8    " ��6N   .�5   
   ��  �@�UseLayoutRounding$ False��6.   �� 5   
    � ��$ 77��6#   �� 5   
    � ��$ 36.06��6"   �� 5   
    � �� 5      �� 5      " ��6"   .R�5      � .+�5      F� 8�� #��� 	 6O   $��77��6u   $��36.06��6�   F� 5      8�� $ Uniform=�6P   $��12q�6b   
�������6n   5       	 5!      P� 5"       �@J�IsMouseOver$
 True��69   . �5#      ��  �@J�Opacity$ .9Q�68   5$      P� 5%       � ��$
 True��67   . �5&      ��  �@J�Opacity$ .6Q�68   5'      P� 5(       � {�$ False��67   . �5)      ��  �@J�Opacity$	 0.4Q�68   5*      5,      5.      5/      �� 51      "   6%   8    .�52   
   ��  �@ NonClientAreaBackground �#262e2f64   �� 53   
   " �@ OtherButtonHoverBackground �#88858067   �� 54   
    �@ OtherButtonForeground
 �����62   �� 55   
   " �@ OtherButtonHoverForeground
 �����67   �� 56   
    �@ NonClientAreaForeground
 �����64   �� 57   
    �@ CloseButtonForeground
 �����62   �� 58   
    �@�Icon$4 .pack://application:,,,/Resource/Image/icon.ico��6!   59      �� 5;      8    " d�6Z   .�5<   
   ��  � �� 5=      �� 5>      $
 4��6   $��1q�6*   #���  6>   #���  6j   $��30��6�   $��30��6�   ._�5?      �� $
��None=�6   $V��Ppack://application:,,,/Resource/Image/LeftMainContent/SortAscending_gray_16x.png��6+   5@      5B      �� 5C   
    �@ CheckedElement 5D      �� 5E      $
 4��6   $��30��6*   $��30��66   $��1q�6A   #���  6U   5G      5H      �� 5J      .��5K   
   W�  506    06$   $ 0:0:0.4D�6+   $U O(UIElement.RenderTransform).(TransformGroup.Children)[3].(TranslateTransform.X)�6>    5L        5M      $ 1��6   5O      5P      �� 5R      .��5S   
   W�  106    06$   $ 0:0:0.4D�6+   $V P(UIElement.RenderTransform).(TransformGroup.Children)[2].(RotateTransform.Angle)�6>    5T        5U      $ 1��6   5W      5X      �� 5Z      .��5[   
   W�  .86    16$   $ 0:0:0.4D�6+   $V P(UIElement.RenderTransform).(TransformGroup.Children)[0].(ScaleTransform.ScaleX)�6>    5\        5]      $ 1��6   5_      5`      �� 5b      8    " ��6[   .�5c   
   ��  � ��
 ���� 6'   5d      �� 5f      " ��6   .�5g   
   ��  �@�OverridesDefaultStyle$
 True��62   �� 5h   
    �@�Cursor$
 Hand��6#   �� 5i   
    � �� 5j      �� 5k      " ��6"   .R�5l      �� $��border��6   $��1q�6+   
���   �#��z��6S   ._�5m      �� $��Center=�6+   $��Center=�6H   5n      	 5p      P� 5q       �@J�IsMouseOver$
 True��69   . �5r      �� # �  6;    � ��5s      5u      5w      5x      �� 5y      8    " ��6S   .�5z   
   ��  � ��
 ���� 6'   �� 5{   
    � ��8  6/   5|      5      