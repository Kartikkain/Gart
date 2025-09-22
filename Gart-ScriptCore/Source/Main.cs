using System;
using System.Runtime.CompilerServices;

namespace Gart
{
    public struct Vector3
    {
        public float x, y, z;
        
        public Vector3(float x,float y,float z)
        {
            this.x = x; 
            this.y = y; 
            this.z = z;
        }
    }

    public class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void NativeVector(ref Vector3 position, out Vector3 Result);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static float NativeVectorFloat(ref Vector3 parameter);

    }

    public class Entity
    {
        public Entity()
        {
            Console.WriteLine("Main Constructor!");

            Vector3 pos = new Vector3(2,3,6);

            InternalCalls.NativeVector(ref pos, out Vector3 result);

            Console.WriteLine($"normalize vector : {result.x}, {result.y}, {result.z}");

            Console.WriteLine($"Dot product : {InternalCalls.NativeVectorFloat(ref pos)}");

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