#include "UnityBridge.h"

static UnityLogCallback unityLogCallback = nullptr;
static DebugInfoCallback debugInfoCallback = nullptr;

extern "C" void SetUnityLogCallback(UnityLogCallback callback)
{
	unityLogCallback = callback;
}

extern "C" void Internal_UnityLog(UnityLogType logType, const char* message)
{
	if (unityLogCallback)
	{
		unityLogCallback(static_cast<int>(logType), message);
	}
}

extern "C" void UnityLog(const char* message)
{
	Internal_UnityLog(UnityLogType::Log, message);
}

extern "C" void UnityLogString(std::string message)
{
	UnityLog(message.c_str());
}

extern "C" void UnityLogWarning(std::string message)
{
	Internal_UnityLog(UnityLogType::Warning, message.c_str());
}

extern "C" void UnityLogError(std::string message)
{
	Internal_UnityLog(UnityLogType::Error, message.c_str());
}

extern "C" void UnityLogException(std::string message)
{
	Internal_UnityLog(UnityLogType::Exception, message.c_str());
}

extern "C" void SetDebugInfoCallback(DebugInfoCallback callback)
{
	debugInfoCallback = callback;
}
extern "C" void GetDebugInfo(int methodToken, int ilOffset, const char** sourceFilePath, int* lineNumber)
{
	if (debugInfoCallback)
	{
		debugInfoCallback(methodToken, ilOffset, sourceFilePath, lineNumber);
	}
}