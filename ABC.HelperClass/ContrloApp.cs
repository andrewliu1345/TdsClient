using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace ABC.HelperClass
{
    public class ContrloApp
    {
        public static void close_app(string fileName)
        {
            Process[] processes = Process.GetProcesses();
            Process[] array = processes;
            for (int i = 0; i < array.Length; i++)
            {
                Process process = array[i];
                if (process.ProcessName.Equals(fileName))
                {
                    process.Kill();
                }
            }
        }

        public static bool search_app(string ProcessName)
        {
            Process[] processes = Process.GetProcesses();
            Process[] array = processes;
            bool result;
            for (int i = 0; i < array.Length; i++)
            {
                Process process = array[i];
                if (process.ProcessName.Equals(ProcessName))
                {
                    result = true;
                    return result;
                }
            }
            result = false;
            return result;
        }

        public static void RunTarget(string fileName, bool createWindows)
        {
            ContrloApp.close_app(Path.GetFileName(fileName).Replace(".exe", ""));
            Process process = new Process();
            process.StartInfo.FileName = Path.GetFileName(fileName);
            process.StartInfo.WorkingDirectory = fileName.Replace(Path.GetFileName(fileName), "");
            process.StartInfo.Arguments = "";
            process.StartInfo.CreateNoWindow = createWindows;
            try
            {
                if (!ContrloApp.search_app(Path.GetFileName(fileName).Replace(".exe", "")))
                {
                    process.Start();
                }
            }
            catch
            {
            }
        }
    }
}   
