#include "Context.h"

sds ns::Arguments::getString(size_t index) {
	return arguments[index];
}

unsigned long long ns::Arguments::getUnsignedLongLong(size_t index) {
	return std::stoull(arguments[index]);
}


float ns::Arguments::getFloat(size_t index) {
	return std::stof(arguments[index]);
}

double ns::Arguments::getDouble(size_t index) {
	return std::stod(arguments[index]);
}

long double ns::Arguments::getLongDouble(size_t index) {
	return std::stold(arguments[index]);
}

long ns::Arguments::getLong(size_t index) {
	return std::stol(arguments[index]);
}

long long ns::Arguments::getLongLong(size_t index) {
	return std::stoll(arguments[index]);
}

ns::Context ns::copyContext(const NikiContext* source) {
	Context copy{source};

	for (size_t i = 0; i < copy.loopVariablesRunning.size(); ++i)
		copy.loopVariablesRunning[i] = &*copy.consoleVariables.find(copy.loopVariablesRunning[i]->first);

	for (size_t i = 0; i < copy.toggleVariablesRunning.size(); ++i)
		copy.toggleVariablesRunning[i] = &*copy.consoleVariables.find(copy.toggleVariablesRunning[i]->first);

	for (size_t i = 0; i < copy.toggleCommandsRunning.size(); ++i)
		copy.toggleCommandsRunning[i] = &copy.commands.commands.find(copy.toggleCommandsRunning[i]->name)->second;

	return copy;
}