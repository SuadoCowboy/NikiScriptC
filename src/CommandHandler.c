#include "CommandHandler.h"

#include "Context.h"

ns::Command* ns::CommandHandler::get(const sds_view& name) {
	return commands.count(name) == 0? nullptr : &commands[name];
}

uint8_t ns::CommandHandler::add(const Command& command) {
	if (commands.count(command.name) != 0)
		return 0;

	commands[command.name] = command;

	return 1;
}

void ns::CommandHandler::remove(const sds_view& name, NikiContext* pCtx) {
	for (size_t i = 0; i < ctx.toggleCommandsRunning.size(); ++i) {
		if (name == ctx.toggleCommandsRunning[i]->name) {
			ctx.toggleCommandsRunning.erase(ctx.toggleCommandsRunning.begin()+i);
			break;
		}
	}

	commands.erase(name);
}