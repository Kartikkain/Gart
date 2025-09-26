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
        extern public static void Entity_GetTranslation(ulong id,out Vector3 position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static float Entity_SetTranslation(ulong id,ref Vector3 parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool Input_GetKeyDown(Keycode keycode);

    }

    public class Entity
    {
        protected Entity() { ID = 0; }
        internal Entity(ulong id)
        {
           ID = id;
        }

        public readonly ulong ID;

        public Vector3 Translation
        {
            get
            {
                InternalCalls.Entity_GetTranslation(ID, out Vector3 translation);
                return translation;

            }
            set
            {
                InternalCalls.Entity_SetTranslation(ID, ref value);
            }
        }
    }
}