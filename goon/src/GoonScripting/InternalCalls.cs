using System.Runtime.CompilerServices;

namespace Goon
{
    /// <summary>
    /// This class is fully static and is bindings to the internal C++ calls underneath.
    /// </summary>
    public static class InternalCalls
    {

        /// <summary>
        ///  This is how you get a gameobject
        /// </summary>
        /// <param name="number"> The entity ID to get</param>
        /// <returns>The number</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static ulong Entity_GetGameObjectById(ulong number);
        /// <summary>
        /// This is how you can get a entitys name
        /// </summary>
        /// <param name="number"> The entity id to get the name for</param>
        /// <returns>Gameobjects name</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static string Entity_GetGameObjectName(ulong number);
    }
}