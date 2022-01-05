using System;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;
using System.Management;
using Microsoft.Win32;
using System.Windows.Media.Imaging;
using System.Security;
using System.Drawing;
using System.Reflection;
using System.Drawing.Imaging;

namespace Winver
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [SecurityCritical]
        [DllImport("ntdll.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        internal static extern int RtlGetVersion(ref OSVERSIONINFOEX versionInfo);

        [DllImport("dwmapi.dll")]
        public static extern int DwmSetWindowAttribute(IntPtr hwnd, DwmWindowAttribute dwAttribute, ref int pvAttribute, int cbAttribute);

        [DllImport("gdi32.dll", EntryPoint = "DeleteObject")]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool DeleteObject([In] IntPtr hObject);

        [Flags]
        public enum DwmWindowAttribute : uint
        {
            DWMWA_USE_IMMERSIVE_DARK_MODE_20h1 = 20,
            DWMWA_USE_IMMERSIVE_DARK_MODE_19h1 = 19,
            DWMWA_MICA_EFFECT = 1029
        }

        [StructLayout(LayoutKind.Sequential)]
        internal struct OSVERSIONINFOEX
        {
            // The OSVersionInfoSize field must be set to Marshal.SizeOf(typeof(OSVERSIONINFOEX))
            internal int OSVersionInfoSize;
            internal int MajorVersion;
            internal int MinorVersion;
            internal int BuildNumber;
            internal int PlatformId;
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 128)]
            internal string CSDVersion;
            internal ushort ServicePackMajor;
            internal ushort ServicePackMinor;
            internal short SuiteMask;
            internal byte ProductType;
            internal byte Reserved;
        }

        string caption;

        public static void EnableMica(HwndSource source, bool darkThemeEnabled)
        {
            int trueValue = 0x01;
            int falseValue = 0x00;
            RegistryKey CurrentVersionKey = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\Windows NT\CurrentVersion");
            // Set dark mode before applying the material, otherwise you'll get an ugly flash when displaying the window.
            if (darkThemeEnabled)
            {
                if (Convert.ToInt16(CurrentVersionKey.GetValue("CurrentBuild").ToString()) < 19041)
                {
                    DwmSetWindowAttribute(source.Handle, DwmWindowAttribute.DWMWA_USE_IMMERSIVE_DARK_MODE_19h1, ref trueValue, Marshal.SizeOf(typeof(int)));
                }
                else
                {
                    DwmSetWindowAttribute(source.Handle, DwmWindowAttribute.DWMWA_USE_IMMERSIVE_DARK_MODE_20h1, ref trueValue, Marshal.SizeOf(typeof(int)));
                }
            }

            else
            {
                if (Convert.ToInt16(CurrentVersionKey.GetValue("CurrentBuild").ToString()) < 19041)
                {
                    DwmSetWindowAttribute(source.Handle, DwmWindowAttribute.DWMWA_USE_IMMERSIVE_DARK_MODE_19h1, ref falseValue, Marshal.SizeOf(typeof(int)));
                }
                else
                {
                    DwmSetWindowAttribute(source.Handle, DwmWindowAttribute.DWMWA_USE_IMMERSIVE_DARK_MODE_20h1, ref falseValue, Marshal.SizeOf(typeof(int)));
                }
            }

            DwmSetWindowAttribute(source.Handle, DwmWindowAttribute.DWMWA_MICA_EFFECT, ref trueValue, Marshal.SizeOf(typeof(int)));
            CurrentVersionKey.Close();
        }

        public static void ApplyMica(HwndSource hwnd)
        {
            RegistryKey key = Registry.CurrentUser.OpenSubKey(@"Software\Microsoft\Windows\CurrentVersion\Themes\Personalize");
            object o = key.GetValue("AppsUseLightTheme");
            int registryValue = (int)o;
            bool darkThemeEnabled;
            if (registryValue == 0)
            {
                darkThemeEnabled = true;
            }
            else
            {
                darkThemeEnabled = false;
            }
            EnableMica(hwnd, darkThemeEnabled);
            key.Close();
        }

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_ContentRendered(object sender, EventArgs e)
        {
            ApplyMica((HwndSource)sender);
            loadstuff();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            PresentationSource presentationSource = PresentationSource.FromVisual((Visual)sender);
            presentationSource.ContentRendered += Window_ContentRendered;
        }

        public void loadstuff()
        {
            RegistryKey key = Registry.CurrentUser.OpenSubKey(@"Software\Microsoft\Windows\CurrentVersion\Themes\Personalize");
            object o = key.GetValue("AppsUseLightTheme");
            int registryValue = (int)o;
            if (registryValue == 0)
            {
                label1.Foreground = new SolidColorBrush(Colors.White);
                label2.Foreground = new SolidColorBrush(Colors.White);
                label3.Foreground = new SolidColorBrush(Colors.White);
                label4.Foreground = new SolidColorBrush(Colors.White);
                label5.Foreground = new SolidColorBrush(Colors.White);
                label6.Foreground = new SolidColorBrush(Colors.White);
                label7.Foreground = new SolidColorBrush(Colors.White);
                label8.Foreground = new SolidColorBrush(Colors.White);
            }
            else
            {
                label1.Foreground = new SolidColorBrush(Colors.Black);
                label2.Foreground = new SolidColorBrush(Colors.Black);
                label3.Foreground = new SolidColorBrush(Colors.Black);
                label4.Foreground = new SolidColorBrush(Colors.Black);
                label5.Foreground = new SolidColorBrush(Colors.Black);
                label6.Foreground = new SolidColorBrush(Colors.Black);
                label7.Foreground = new SolidColorBrush(Colors.Black);
                label8.Foreground = new SolidColorBrush(Colors.Black);
            }
            hyper.Foreground = SystemParameters.WindowGlassBrush;
            RegistryKey CurrentVersionKey = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\Windows NT\CurrentVersion");
            string version = (string)CurrentVersionKey.GetValue("DisplayVersion");
            if (version == "")
            {
                version = (string)CurrentVersionKey.GetValue("ReleaseId");
                if (version == "2009")
                {
                    version = "20H2";
                }
            }
            string UBR = ((int)CurrentVersionKey.GetValue("UBR")).ToString();
            string Build = (string)CurrentVersionKey.GetValue("CurrentBuild");
            label3.Content = "Version " + version + " (OS Build " + Build + "." + UBR + ")";
            ManagementObject wmi = new ManagementObjectSearcher("select * from Win32_OperatingSystem").Get().Cast<ManagementObject>().First();
            caption = ((string)wmi["Caption"]).Trim().Replace("Microsoft ", "");
            hero.Source = new BitmapImage(new Uri("pack://application:,,,/final.png"));
            Text.Text = "The " + caption + " operating system and its user interface are protected by trademark and other pending or existing intellectual property rights in the United States and other countries/regions.";
            label7.Content = (string)CurrentVersionKey.GetValue("RegisteredOwner");
            label8.Content = (string)CurrentVersionKey.GetValue("RegisteredOrganization");
            CurrentVersionKey.Close();
            wmi.Dispose();
            key.Close();
        }

        private void Hyperlink_RequestNavigate(object sender, System.Windows.Navigation.RequestNavigateEventArgs e)
        {
            try
            {
                Window2 win2 = new Window2();
                win2.Show();
            }
            catch
            {
                MessageBox.Show("Error");
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void Window_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == System.Windows.Input.Key.Enter)
            {
                Close();
            }
            else if (e.Key == System.Windows.Input.Key.Escape)
            {
                Close();
            }
        }
    }
}
