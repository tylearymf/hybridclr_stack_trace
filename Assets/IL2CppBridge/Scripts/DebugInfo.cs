using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection.Metadata;
using System.Reflection.Metadata.Ecma335;
using System.Runtime.InteropServices;

namespace IL2CppBridge
{
    public class DebugInfo : IBridgeComponent
    {
        delegate void DebugInfoDelegate(int methodToken, int ilOffset, ref string sourceFilePath, ref int lineNumber);

        [DllImport("__Internal")]
        static extern void SetDebugInfoCallback(DebugInfoDelegate callback);

        [AOT.MonoPInvokeCallback(typeof(DebugInfoDelegate))]
        static void DebugInfoCallback(int methodToken, int ilOffset, ref string sourceFilePath, ref int lineNumber)
        {
            var methodHandle = (MethodDefinitionHandle)MetadataTokens.Handle(methodToken);
            if (methodHandle.IsNil)
                return;

            var debugInfoHandle = methodHandle.ToDebugInformationHandle();
            if (debugInfoHandle.IsNil)
                return;

            foreach (var provider in s_PdbProviders.Values)
            {
                var reader = provider.GetMetadataReader();
                var debugInfo = reader.GetMethodDebugInformation(debugInfoHandle);
                if (debugInfo.SequencePointsBlob.IsNil)
                    continue;

                SequencePoint? lastPoint = null;
                foreach (var point in debugInfo.GetSequencePoints())
                {
                    if (point.IsHidden)
                        continue;

                    if (ilOffset < point.Offset)
                        break;

                    lastPoint = point;
                }

                if (lastPoint.HasValue)
                {
                    sourceFilePath = reader.GetString(reader.GetDocument(lastPoint.Value.Document).Name);
                    lineNumber = lastPoint.Value.StartLine;
                    break;
                }
            }
        }

        static Dictionary<string, MetadataReaderProvider> s_PdbProviders = new();
        public static void RegisterPDB(string assemblyName, byte[] pdbData)
        {
            if (string.IsNullOrEmpty(assemblyName) || pdbData == null || pdbData.Length == 0)
            {
                throw new Exception($"Invalid pdb data. assemblyName:{assemblyName}");
            }

            s_PdbProviders.Add(assemblyName, MetadataReaderProvider.FromPortablePdbStream(new MemoryStream(pdbData)));
        }


        public void Init()
        {
            SetDebugInfoCallback(DebugInfoCallback);
        }

        public void Dispose()
        {
            if (s_PdbProviders != null)
            {
                foreach (var item in s_PdbProviders.Values)
                    item.Dispose();

                s_PdbProviders = null;
            }
        }
    }
}