#include "CommandHandler.h"

#include "Context.h"

ns::Command* ns::CommandHandler::get(const std::string_view& name) {
	return commands.count(name) == 0? nullptr : &commands[name];
}

uint8_t ns::CommandHandler::add(const Command& command) {
	if (commands.count(command.name) != 0)
		return false;

	commands[command.name] = command;

	return true;
}

void ns::CommandHandler::remove(const std::string_view& name, Context& ctx) {
	for (size_t i = 0; i < ctx.toggleCommandsRunning.size(); ++i) {
		if (name == ctx.toggleCommandsRunning[i]->name) {
			ctx.toggleCommandsRunning.erase(ctx.toggleCommandsRunning.begin()+i);
			break;
		}
	}

	commands.erase(name);
}