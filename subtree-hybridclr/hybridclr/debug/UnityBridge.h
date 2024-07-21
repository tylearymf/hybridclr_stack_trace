#pragma once

#include <string>

enum class UnityLogType {
	Error = 0,
	Assert = 1,
	Warning = 2,
	Log = 3,
	Exception = 4,
};

typedef void (*UnityLogCallback)(int logType, const char* message);
typedef void (*DebugInfoCallback)(int methodToken, int ilOffset, const char** sourceFilePath, int* lineNumber);

extern "C" void SetUnityLogCallback(UnityLogCallback callback);
extern "C" void Internal_UnityLog(UnityLogType logType, const char* message);
extern "C" void UnityLog(const char* message);
extern "C" void UnityLogString(std::string message);
extern "C" void UnityLogWarning(std::string message);
extern "C" void UnityLogError(std::string message);
extern "C" void UnityLogException(std::string message);

extern "C" void SetDebugInfoCallback(DebugInfoCallback callback);
extern "C" void GetDebugInfo(int methodToken, int ilOffset, const char** sourceFilePath, int* lineNumber);