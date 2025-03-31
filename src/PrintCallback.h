#pragma once

#include <stdint.h>
#include <string>

#include "DLLExport.h"
#include "Utils.h"

namespace ns {
	enum PrintLevel : uint8_t {
		DEFAULT = 0, ///< any user input
		ECHO, ///< any text that came from a command that is not an error
		WARNING, ///< anything that is wrong but can continue
		ERROR, ///< anything that went wrong and can not continue
	};

	NIKIAPI std::string printLevelToString(PrintLevel level);

	typedef void(*PrintCallback)(void* pData, PrintLevel level, const std::string& message);

	extern PrintCallback printCallback;
	extern void* pPrintCallbackData;

	template<typename... Args>
	void printf(const PrintLevel& level, const std::string& format, Args ... args) {
		print(level, formatString(format, args...));
	}

	extern PrintCallback printCallback;
	extern void* pPrintCallbackData;

	NIKIAPI void setPrintCallback(void* pData, PrintCallback callback);
	NIKIAPI void print(const PrintLevel& level, const std::string& str);
	NIKIAPI void printUnknownCommand(const std::string& command);
}