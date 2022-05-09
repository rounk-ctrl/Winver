using System;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Media;
using Microsoft.Win32;
using System.Windows.Media.Imaging;
using System.Windows.Input;
using static Winver.MicaHandler;
using System.Runtime.InteropServices;

namespace Winver
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private string caption;

        [DllImport("winbrand.dll", CharSet = CharSet.Unicode)]
        [DefaultDllImportSearchPaths(DllImportSearchPath.System32)]
        static extern string BrandingFormatString(string format);
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_ContentRendered(object sender, EventArgs e)
        {
            ApplyMica((HwndSource)sender);
            Loadstuff();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            PresentationSource presentationSource = PresentationSource.FromVisual((Visual)sender);
            presentationSource.ContentRendered += Window_ContentRendered;
        }

        public void Loadstuff()
        {
            RegistryKey key = Registry.CurrentUser.OpenSubKey(@"Software\Microsoft\Windows\CurrentVersion\Themes\Personalize");
            object o = key.GetValue("AppsUseLightTheme");
            Uri light = new Uri("pack://application:,,,/WPFUI;component/Styles/Theme/Light.xaml", UriKind.RelativeOrAbsolute);
            Uri dark = new Uri("pack://application:,,,/WPFUI;component/Styles/Theme/Dark.xaml", UriKind.RelativeOrAbsolute);

            int registryValue = (int)o;
            if (registryValue == 0)
            {
                Application.Current.Resources.MergedDictionaries.Add(new ResourceDictionary()
                {
                    Source = dark
                });
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
                Application.Current.Resources.MergedDictionaries.Add(new ResourceDictionary()
                {
                    Source = light
                });
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
            RenderOptions.SetBitmapScalingMode(hero, BitmapScalingMode.LowQuality);
            hero.Source = new BitmapImage(new Uri("pack://application:,,,/final.png"));
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
            Text.Text = BrandingFormatString("The %WINDOWS_LONG% operating system and its user interface are protected by trademark and other pending or existing intellectual property rights in the United States and other countries/regions.");
            label7.Content = (string)CurrentVersionKey.GetValue("RegisteredOwner");
            label8.Content = (string)CurrentVersionKey.GetValue("RegisteredOrganization");
            CurrentVersionKey.Close();
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
                _ = MessageBox.Show("Error");
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                Close();
            }
            else if (e.Key == Key.Escape)
            {
                Close();
            }
            if (Keyboard.Modifiers == ModifierKeys.Control && e.Key == Key.F11)
            {

            }
        }
    }
}
