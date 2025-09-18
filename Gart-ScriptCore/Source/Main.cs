using System;

namespace Gart
{
    public class Main
    {
        public Main()
        {
            Console.WriteLine("Main Constructor!");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello World From C#");
        }

        public void PrintInt(int message)
        {
            Console.WriteLine($"C# Says : {message}");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# Says : {message}");
        }
    }
}