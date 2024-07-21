#pragma once

#include <stack>
#include <mutex>

#include "../il2cpp-class-internals.h"
#include "../libil2cpp/vm/StackTrace.h"

namespace hybridclr
{
	typedef struct StackFrameInfo {
		const MethodInfo* method;
		const uint16_t callerILOffset;
	} StackFrameInfo;

	class StackTrace {
	public:
		static void EnterFunction(StackFrameInfo* info);
		static void LeaveFunction();
		static void Cleanup();
		static std::string GetFramesLog();

	private:
		static std::stack<StackFrameInfo*> s_callStack;
		static std::mutex s_mutex;
	};


#define PUSH_NEW_STACK_FRAME(method, ilOffset)\
	StackFrameInfo* stackFrameInfo = new StackFrameInfo{method, ilOffset};\
	StackTrace::EnterFunction(stackFrameInfo);

#define POP_NEW_STACK_FRAME()\
	StackTrace::LeaveFunction();

#define CLEANUP_NEW_STACK_FRAME()\
	StackTrace::Cleanup();
}