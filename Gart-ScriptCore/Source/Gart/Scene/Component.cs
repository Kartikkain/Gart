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
        public enum BodyType { Static = 0, Dynamic, Kinematic };
        public Vector2 LinearVelocity
        {
            get
            {
                InternalCalls.RigidBody2DComponent_GetLinearVelocity(Entity.ID, out Vector2 linearVelocity);
                return linearVelocity;
            }
        }

        public BodyType Type
        {
            get => InternalCalls.RigidBody2DComponent_GetBodyType(Entity.ID);
            set => InternalCalls.RigidBody2DComponent_SetBodyType(Entity.ID, value);
        }
        public void ApplyImpulse(Vector2 impulse, Vector2 worldposition,bool wake)
        {
            InternalCalls.RigidBody2DComponent_ApplyImpulse(Entity.ID, ref impulse, ref worldposition,wake);
        }

        public void ApplyImpulse(Vector2 impulse, bool wake)
        {
            InternalCalls.RigidBody2DComponent_ApplyImpulseToCenter(Entity.ID, ref impulse, wake);
        }
    }

    public class TextComponent:Component
    {
        public string Text
        {
            get => InternalCalls.TextComponent_GetText(Entity.ID);
            set => InternalCalls.TextComponent_SetText(Entity.ID, value);
        }

        public Vector4 Color
        {
            get
            {
                InternalCalls.TextComponent_GetColor(Entity.ID, out Vector4 color);
                return color;
            }

            set=> InternalCalls.TextComponent_SetColor(Entity.ID, ref value);
        }

        public float Kerning
        {
            get=> InternalCalls.TextComponent_GetKerning(Entity.ID);
            set=> InternalCalls.TextComponent_SetKerning(Entity.ID, value);
        }

        public float LineSpacing
        {
            get=>InternalCalls.TextComponent_GetLineSpacing(Entity.ID);
            set=>InternalCalls.TextComponent_SetLineSpacing(Entity.ID, value);
        }
    }

}
