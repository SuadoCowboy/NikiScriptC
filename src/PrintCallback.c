#include "PrintCallback.h"

#include "Context.h"

sds nikiPrintLevelToString(PrintLevel level) {
	switch (level) {
	case PrintLevel::DEFAULT:
		return sds("DEFAULT");
	case PrintLevel::ECHO:
		return sds("ECHO");
	case PrintLevel::WARNING:
		return sds("WARNING");
	case PrintLevel::ERROR:
		return sds("ERROR");
	default:
		return sds("UNKNOWN");
	}

}

void nikiSetPrintCallback(void* pData, PrintCallback callback) {
	printCallback = callback;
	pPrintCallbackData = pData;
}

void nikiPrint(NikiPrintLevel level, NikiContext* pCtx, const sds str) {
	printCallback(pPrintCallbackData, pCtx, level, str);
}

void nikiPrintf(NikiPrintLevel level, const sds format, ...) {
	

	print(level, formatString(format, args...));
}

void nikiPrintUnknownCommand(const sds command) {
	printf(PrintLevel::ERROR, "Unknown command \"{}\"\n", command);
}