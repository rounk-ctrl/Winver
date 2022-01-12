using Microsoft.Win32;
using System;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Documents;
using System.Windows.Interop;
using System.Windows.Media;

namespace Winver
{
    /// <summary>
    /// Interaction logic for Window2.xaml
    /// </summary>
    public partial class Window2 : Window
    {

        [DllImport("dwmapi.dll")]
        public static extern int DwmSetWindowAttribute(IntPtr hwnd, DwmWindowAttribute dwAttribute, ref int pvAttribute, int cbAttribute);
        
        [Flags]
        public enum DwmWindowAttribute : uint
        {
            DWMWA_USE_IMMERSIVE_DARK_MODE = 20,
            DWMWA_MICA_EFFECT = 1029
        }

        public static void EnableMica(HwndSource source, bool darkThemeEnabled)
        {
            int trueValue = 0x01;
            int falseValue = 0x00;
            // Set dark mode before applying the material, otherwise you'll get an ugly flash when displaying the window.
            if (darkThemeEnabled)
                DwmSetWindowAttribute(source.Handle, DwmWindowAttribute.DWMWA_USE_IMMERSIVE_DARK_MODE, ref trueValue, Marshal.SizeOf(typeof(int)));
            else
                DwmSetWindowAttribute(source.Handle, DwmWindowAttribute.DWMWA_USE_IMMERSIVE_DARK_MODE, ref falseValue, Marshal.SizeOf(typeof(int)));

            DwmSetWindowAttribute(source.Handle, DwmWindowAttribute.DWMWA_MICA_EFFECT, ref trueValue, Marshal.SizeOf(typeof(int)));
        }

        public static void UpdateStyleAttributes(HwndSource hwnd)
        {
            var darkThemeEnabled = false;
            RegistryKey key = Registry.CurrentUser.OpenSubKey(@"Software\Microsoft\Windows\CurrentVersion\Themes\Personalize");
            Object o = key.GetValue("AppsUseLightTheme");
            int registryValue = (int)o;
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

        public Window2()
        {
            InitializeComponent();
        }

        private void Window_ContentRendered(object sender, System.EventArgs e)
        {
            // Apply Mica brush and ImmersiveDarkMode if needed
            UpdateStyleAttributes((HwndSource)sender);
            loadstuff();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            PresentationSource presentationSource = PresentationSource.FromVisual((Visual)sender);
            presentationSource.ContentRendered += Window_ContentRendered;
        }

        public void loadstuff()
        {
            LoadTextDocument(@"C:\Windows\System32\license.rtf");
            RegistryKey key = Registry.CurrentUser.OpenSubKey(@"Software\Microsoft\Windows\CurrentVersion\Themes\Personalize");
            Object o = key.GetValue("AppsUseLightTheme");
            int registryValue = (int)o;
            if (registryValue == 0)
            {
                RichTextBox1.Foreground = new SolidColorBrush(Colors.White);
                label2.Foreground = new SolidColorBrush(Colors.White);
            }
            else
            {
                RichTextBox1.Foreground = new SolidColorBrush(Colors.Black);
                label2.Foreground = new SolidColorBrush(Colors.Black);
            }
            key.Close();
        }

        private void LoadTextDocument(string fileName)
        {
            TextRange textRange;
            System.IO.FileStream fileStream;
            if (System.IO.File.Exists(fileName))
            {
                textRange = new TextRange(RichTextBox1.Document.ContentStart, RichTextBox1.Document.ContentEnd);
                using (fileStream = new System.IO.FileStream(fileName, System.IO.FileMode.Open, System.IO.FileAccess.Read))
                {
                    textRange.Load(fileStream, System.Windows.DataFormats.Rtf);
                }
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void Button_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {

        }
    }
}
