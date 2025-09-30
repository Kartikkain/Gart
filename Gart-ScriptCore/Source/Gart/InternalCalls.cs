using System;
using System.Runtime.CompilerServices;
using System.Runtime.Versioning;

namespace Gart
{
    public class InternalCalls
    {
        #region Transform

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void Transform_GetTranslation(ulong id, out Vector3 position);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static float Transform_SetTranslation(ulong id, ref Vector3 parameter);

        #endregion

        #region Input

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool Input_GetKeyDown(Keycode keycode);

        #endregion

        #region Entity

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static bool Entity_Has_Component(ulong id, Type componentType);

        #endregion

        #region RigidBody2d

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void RigidBody2DComponent_ApplyImpulse(ulong id,ref Vector2 impulse, ref Vector2 worldposition, bool wake = true);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void RigidBody2DComponent_ApplyImpulseToCenter(ulong id, ref Vector2 impulse,bool wake = true);

        #endregion

    }

}
