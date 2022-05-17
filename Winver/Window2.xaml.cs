using Microsoft.Win32;
using System.Windows;
using System.Windows.Documents;
using System.Windows.Interop;
using System.Windows.Media;
using System.IO;
using System.Windows.Input;
using static Winver.MicaHandler;

namespace Winver
{
    /// <summary>
    /// Interaction logic for Window2.xaml
    /// </summary>
    public partial class Window2 : Window
    {
        public Window2()
        {
            InitializeComponent();
        }
        private void Window_ContentRendered(object sender, System.EventArgs e)
        {
            // Apply Mica brush and ImmersiveDarkMode if needed
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
            LoadTextDocument(@"C:\Windows\System32\license.rtf");
            RegistryKey key = Registry.CurrentUser.OpenSubKey(@"Software\Microsoft\Windows\CurrentVersion\Themes\Personalize");
            object o = key.GetValue("AppsUseLightTheme");
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
            FileStream fileStream;
            if (File.Exists(fileName))
            {
                textRange = new TextRange(RichTextBox1.Document.ContentStart, RichTextBox1.Document.ContentEnd);
                using (fileStream = new FileStream(fileName, FileMode.Open, FileAccess.Read))
                {
                    textRange.Load(fileStream, DataFormats.Rtf);
                }
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void Button_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                Close();
            }
            else if (e.Key == Key.Escape)
            {
                Close();
            }
        }
    }
}
