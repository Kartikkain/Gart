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

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static ulong Find_Entity_By_Name(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static object GetScriptInstance(ulong entityID);

        #endregion

        #region RigidBody2d

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void RigidBody2DComponent_ApplyImpulse(ulong id,ref Vector2 impulse, ref Vector2 worldposition, bool wake = true);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void RigidBody2DComponent_ApplyImpulseToCenter(ulong id, ref Vector2 impulse,bool wake = true);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void RigidBody2DComponent_GetLinearVelocity(ulong id, out Vector2 LinearVelocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static RigidBody2DComponent.BodyType RigidBody2DComponent_GetBodyType(ulong id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void RigidBody2DComponent_SetBodyType(ulong id, RigidBody2DComponent.BodyType bodyType);

        #endregion

        #region TextComponent

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static string TextComponent_GetText(ulong id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void TextComponent_SetText(ulong id, string text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void TextComponent_GetColor(ulong id, out Vector4 color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void TextComponent_SetColor(ulong id, ref Vector4 color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static float TextComponent_GetKerning(ulong id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void TextComponent_SetKerning(ulong id, float kerning);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static float TextComponent_GetLineSpacing(ulong id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public static void TextComponent_SetLineSpacing(ulong id, float lineSpacing);

        #endregion

    }

}
