using Microsoft.Win32;
using System;
using System.Runtime.InteropServices;
using System.Windows.Interop;

namespace Winver
{
    internal class MicaHandler
    {
        [DllImport("dwmapi.dll")]
        public static extern int DwmSetWindowAttribute(IntPtr hwnd, DwmWindowAttribute dwAttribute, ref int pvAttribute, int cbAttribute);

        [Flags]
        public enum DwmWindowAttribute : uint
        {
            DWMWA_USE_IMMERSIVE_DARK_MODE_20h1 = 20,
            DWMWA_USE_IMMERSIVE_DARK_MODE_19h1 = 19,
            DWMWA_MICA_EFFECT = 1029,
            DWMWA_SYSTEMBACKDROP_TYPE = 38
        }

        public static void EnableMica(HwndSource source, bool darkThemeEnabled)
        {
            int trueValue = 0x01;
            int falseValue = 0x00;
            int mica = 2;
            RegistryKey CurrentVersionKey = Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\Windows NT\CurrentVersion");
            // Set dark mode before applying the material, otherwise you'll get an ugly flash when displaying the window.
            _ = darkThemeEnabled
                ? Convert.ToInt16(CurrentVersionKey.GetValue("CurrentBuild").ToString()) < 19041
                    ? DwmSetWindowAttribute(source.Handle, DwmWindowAttribute.DWMWA_USE_IMMERSIVE_DARK_MODE_19h1, ref trueValue, Marshal.SizeOf(typeof(int)))
                    : DwmSetWindowAttribute(source.Handle, DwmWindowAttribute.DWMWA_USE_IMMERSIVE_DARK_MODE_20h1, ref trueValue, Marshal.SizeOf(typeof(int)))
                : Convert.ToInt16(CurrentVersionKey.GetValue("CurrentBuild").ToString()) < 19041
                    ? DwmSetWindowAttribute(source.Handle, DwmWindowAttribute.DWMWA_USE_IMMERSIVE_DARK_MODE_19h1, ref falseValue, Marshal.SizeOf(typeof(int)))
                    : DwmSetWindowAttribute(source.Handle, DwmWindowAttribute.DWMWA_USE_IMMERSIVE_DARK_MODE_20h1, ref falseValue, Marshal.SizeOf(typeof(int)));
            _ = Convert.ToInt16(CurrentVersionKey.GetValue("CurrentBuild").ToString()) >= 22523
                ? DwmSetWindowAttribute(source.Handle, DwmWindowAttribute.DWMWA_SYSTEMBACKDROP_TYPE, ref mica, Marshal.SizeOf(typeof(int)))
                : DwmSetWindowAttribute(source.Handle, DwmWindowAttribute.DWMWA_MICA_EFFECT, ref trueValue, Marshal.SizeOf(typeof(int)));
            CurrentVersionKey.Close();
        }

        public static void ApplyMica(HwndSource hwnd)
        {
            RegistryKey key = Registry.CurrentUser.OpenSubKey(@"Software\Microsoft\Windows\CurrentVersion\Themes\Personalize");
            object o = key.GetValue("AppsUseLightTheme");
            int registryValue = (int)o;
            bool darkThemeEnabled = registryValue == 0;
            EnableMica(hwnd, darkThemeEnabled);
            key.Close();
        }
    }
}
