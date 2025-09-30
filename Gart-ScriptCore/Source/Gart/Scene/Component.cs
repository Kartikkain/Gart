using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Gart
{
    public abstract class Component
    {
        public Entity Entity { get; internal set; }
    }

    public class TransformComponent : Component
    {
        public static int ID = 1;
        public Vector3 Translation
        {
            get
            {
                InternalCalls.Transform_GetTranslation(Entity.ID, out Vector3 translation);
                return translation;

            }
            set
            {
                InternalCalls.Transform_SetTranslation(Entity.ID, ref value);
            }
        }
    }

    public class RigidBody2DComponent:Component
    {
        public void ApplyImpulse(Vector2 impulse, Vector2 worldposition,bool wake)
        {
            InternalCalls.RigidBody2DComponent_ApplyImpulse(Entity.ID, ref impulse, ref worldposition,wake);
        }

        public void ApplyImpulse(Vector2 impulse, bool wake)
        {
            InternalCalls.RigidBody2DComponent_ApplyImpulseToCenter(Entity.ID, ref impulse, wake);
        }
    }
}
