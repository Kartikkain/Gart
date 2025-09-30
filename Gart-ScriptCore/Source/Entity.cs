using System;
using System.Runtime.CompilerServices;

namespace Gart
{
   
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
                InternalCalls.Transform_GetTranslation(ID, out Vector3 translation);
                return translation;

            }
            set
            {
                InternalCalls.Transform_SetTranslation(ID, ref value);
            }
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_Has_Component(ID, componentType);
        }
        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>()) return null;

            T component =  new T(){ Entity = this};
            return component;
        }
    }
}