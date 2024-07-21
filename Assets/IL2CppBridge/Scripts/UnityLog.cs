using System.Runtime.InteropServices;
using UnityEngine;

namespace IL2CppBridge
{
    public class UnityLog : IBridgeComponent
    {
        delegate void UnityLogDelegate(int logType, string message);

        [DllImport("__Internal")]
        static extern void SetUnityLogCallback(UnityLogDelegate callback);

        [AOT.MonoPInvokeCallback(typeof(UnityLogDelegate))]
        static void UnityLogCallback(int logType, string message)
        {
            var logEnum = (LogType)logType;
            Debug.LogFormat(logEnum, LogOption.NoStacktrace, null, message);
        }

        public void Init()
        {
            SetUnityLogCallback(UnityLogCallback);
        }

        public void Dispose()
        {
        }
    }
}