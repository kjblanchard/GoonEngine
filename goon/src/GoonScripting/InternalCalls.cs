using System.Runtime.CompilerServices;

namespace Goon
{
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static ulong Entity_GetGameObjectById(ulong number);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static string Entity_GetGameObjectName(ulong number);
    }
}