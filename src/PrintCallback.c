#include "PrintCallback.h"

ns::PrintCallback ns::printCallback = nullptr;
void* ns::pPrintCallbackData = nullptr;

std::string ns::printLevelToString(PrintLevel level) {
	switch (level) {
	case PrintLevel::DEFAULT:
		return std::string("DEFAULT");
	case PrintLevel::ECHO:
		return std::string("ECHO");
	case PrintLevel::WARNING:
		return std::string("WARNING");
	case PrintLevel::ERROR:
		return std::string("ERROR");
	default:
		return std::string("UNKNOWN");
	}

}

void ns::setPrintCallback(void* pData, PrintCallback callback) {
	printCallback = callback;
	pPrintCallbackData = pData;
}

void ns::print(const ns::PrintLevel &level, const std::string& str) {
	printCallback(pPrintCallbackData, level, str);
}

void ns::printUnknownCommand(const std::string& command) {
	printf(PrintLevel::ERROR, "Unknown command \"{}\"\n", command);
}