#include "Command.h"

#include <sstream>
#include <cassert>

#include "PrintCallback.h"

ns::Command::Command() {}

ns::Command::Command(const std::string_view& name, unsigned char minArgs, unsigned char maxArgs,
	CommandCallback callback, const std::string_view& description, const std::vector<std::string_view>& argsDescriptions)
	: name(name), minArgs(minArgs), maxArgs(maxArgs), callback(callback), description(description), argsDescriptions(argsDescriptions) {
	assert(!name.empty());
	assert(minArgs <= maxArgs);
	assert(argsDescriptions.size() % 2 == 0);
	assert(argsDescriptions.size() / 2 == maxArgs);

	bool isName = true;
	for (uint8_t i = 0; i < argsDescriptions.size(); ++i) {
		const std::string_view& arg = argsDescriptions[i];

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

std::string ns::Command::getArgumentsNames() {
	if (argsDescriptions.size() == 0)
		return "";

	std::stringstream oss{};
	bool isName = true;

	for (uint64_t i = 0; i < argsDescriptions.size(); ++i) {
		if (isName)
			oss << argsDescriptions[i] << ' ';

		isName = !isName;
	}

	std::string out = oss.str();
	out.erase(out.size()-1);

	return out;
}

void ns::Command::printAsDataTree() {
	std::stringstream descriptions{};
	std::stringstream usage{};
	bool isName = true;

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

	ns::print(PrintLevel::ECHO, usage.str());
}