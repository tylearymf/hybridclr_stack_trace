using System.Collections.Generic;
using UnityEngine;

namespace IL2CppBridge
{
    public class IL2CppBridge : MonoBehaviour
    {
        List<IBridgeComponent> bridgeComponents;

        void Awake()
        {
            bridgeComponents = new List<IBridgeComponent>();
            bridgeComponents.Add(new UnityLog());
            bridgeComponents.Add(new DebugInfo());

            foreach (var component in bridgeComponents)
            {
                component.Init();
            }
        }

        void OnDestroy()
        {
            foreach (var component in bridgeComponents)
            {
                component.Dispose();
            }
            bridgeComponents = null;
        }
    }

    public interface IBridgeComponent
    {
        void Init();
        void Dispose();
    }
}