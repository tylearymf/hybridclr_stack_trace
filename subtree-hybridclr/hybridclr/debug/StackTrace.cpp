#include "StackTrace.h"

#include <sstream>
#include <iomanip>

#include "../transform/TransformContext.h"
#include "UnityBridge.h"

namespace hybridclr
{
	std::stack<StackFrameInfo*> StackTrace::s_callStack;
	std::mutex StackTrace::s_mutex;

	void StackTrace::EnterFunction(StackFrameInfo* info) {
		std::lock_guard<std::mutex> lock(s_mutex);

		s_callStack.push(info);
	}

	void StackTrace::LeaveFunction() {
		std::lock_guard<std::mutex> lock(s_mutex);

		if (!s_callStack.empty()) {
			delete s_callStack.top();
			s_callStack.pop();
		}
	}

	void StackTrace::Cleanup() {
		std::lock_guard<std::mutex> lock(s_mutex);

		while (!s_callStack.empty()) {
			delete s_callStack.top();
			s_callStack.pop();
		}
	}

	std::string StackTrace::GetFramesLog()
	{
		if (s_callStack.empty()) {
			return "";
		}

		std::vector<StackFrameInfo*> frames;

		std::stack<StackFrameInfo*> tempStack = s_callStack;
		while (!tempStack.empty()) {
			frames.push_back(tempStack.top());
			tempStack.pop();
		}

		std::string logTrace;
		uint32_t last_il_offset = hybridclr::transform::g_exec_il_offset;

		for (size_t i = 0; i < frames.size(); i++)
		{
			const StackFrameInfo* frame = frames[i];
			const MethodInfo* method = frame->method;

			const char* sourceFile = nullptr;
			int lineNumber = 0;
			GetDebugInfo(method->token, last_il_offset, &sourceFile, &lineNumber);

			std::stringstream ss;
			if (sourceFile != nullptr)
			{
				ss << sourceFile << ":" << lineNumber;
			}
			else
			{
				ss << "IL_" << std::setfill('0') << std::setw(4) << std::uppercase << std::hex << last_il_offset;
			}

			std::string namespaceName;
			if (method->klass->namespaze != nullptr && method->klass->namespaze[0] != '\0') {
				namespaceName = std::string(method->klass->namespaze) + ".";
			}

			logTrace += namespaceName + std::string(method->klass->name)
				+ "." + std::string(method->name)
				+ "() (at " + ss.str() + ")\n";

			last_il_offset = frame->callerILOffset;
		}

		return logTrace + "\n";
	}
}