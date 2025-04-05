#include "Command.h"

#include <assert.h>

#include "PrintCallback.h"

NikiCommand_init(NikiCommand* pCommand, const sds name, unsigned char minArgs, unsigned char maxArgs, NikiCommandCallback callback, const sds description, const sds* pArgsDescriptions) {
	assert(!name.empty());
	assert(minArgs <= maxArgs);
	assert(argsDescriptions.size() % 2 == 0);
	assert(argsDescriptions.size() / 2 == maxArgs);

	uint8_t isName = true;
	for (uint8_t i = 0; i < argsDescriptions.size(); ++i) {
		const sds_view& arg = argsDescriptions[i];

		if (isName) {
			assert(arg.size() > 3);
			assert(arg[0] == 's' || arg[0] == 'i' || arg[0] == 'd' || arg[0] == 'v');
			assert(arg[1] == '[' && arg[arg.size()-1] == ']');
			if (i >= minArgs*2)
				assert(arg[arg.size()-2] == '?');
		}

		isName = !isName;
	}

	assert(callback != nullptr);
}

sds ns::Command::getArgumentsNames() {
	if (argsDescriptions.size() == 0)
		return "";

	sdsstream oss{};
	uint8_t isName = true;

	for (uint64_t i = 0; i < argsDescriptions.size(); ++i) {
		if (isName)
			oss << argsDescriptions[i] << ' ';

		isName = !isName;
	}

	sds out = oss.str();
	out.erase(out.size()-1);

	return out;
}

void ns::Command::printAsDataTree() {
	sdsstream descriptions{};
	sdsstream usage{};
	uint8_t isName = true;

	usage << "- Usage: " << name << ' ';
	descriptions << "- Description: " << description << "\n- Arguments:\n";
	for (uint64_t i = 0; i < argsDescriptions.size(); ++i) {
		if (isName) {
			descriptions << "- - " << argsDescriptions[i] << ": ";
			usage << argsDescriptions[i] << ' ';

		} else
			descriptions << argsDescriptions[i] << '\n';

		isName = !isName;
	}
	usage << '\n' << descriptions.str();

	nikiPrint(PrintLevel::ECHO, usage.str());
}