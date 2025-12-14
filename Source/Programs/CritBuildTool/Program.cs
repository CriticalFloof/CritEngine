
using System.Diagnostics;
using System.Reflection;
class Program
{
    static int Main(string[] args)
    {
        string action = args.Length == 1 ? args[0] : "vs2026";
        string toolchain = args.Length == 2 ? args[1] : "msc-v145";

        //TODO: Put these paths in some config file.
        string RootPath = Path.GetFullPath(Path.Combine(Assembly.GetExecutingAssembly().Location, "../../../.."));
        string PremakePath = Path.GetFullPath(Path.Combine(RootPath, "thirdparty/premake/premake5.exe"));
        string TargetPremakeScriptPath = Path.GetFullPath(Path.Combine(RootPath, "../../../Source/Runtime/premake5.lua"));

        Process premakeProcess = new Process();
        premakeProcess.StartInfo.FileName = PremakePath;
        premakeProcess.StartInfo.ArgumentList.Add(action);
        premakeProcess.StartInfo.ArgumentList.Add($"--cc={toolchain}");
        premakeProcess.StartInfo.ArgumentList.Add($"--FILE={TargetPremakeScriptPath}");
        premakeProcess.StartInfo.RedirectStandardOutput = true;
        premakeProcess.StartInfo.RedirectStandardError = true;
        //premakeProcess.StartInfo.CreateNoWindow = true;

        premakeProcess.OutputDataReceived += new DataReceivedEventHandler(PremakeOutputHandler);
        premakeProcess.ErrorDataReceived += new DataReceivedEventHandler(PremakeOutputHandler);
        premakeProcess.Start();
        premakeProcess.BeginOutputReadLine();
        premakeProcess.BeginErrorReadLine();

        premakeProcess.WaitForExit();
        

        return 0;
    }

    static void PremakeOutputHandler(object sendingProcess, DataReceivedEventArgs args)
    {
        Console.WriteLine(args.Data);
    }
}