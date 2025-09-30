
namespace Gart
{
    public class Input
    {
        public static bool GetKeyDown(Keycode keycode)
        {
            return InternalCalls.Input_GetKeyDown(keycode);
        }
    }
}
