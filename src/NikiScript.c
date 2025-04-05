#include "NikiScript.h"

#include "Utils.h"
#include "Parser.h"
#include "Lexer.h"

#include <mimalloc.h>

void nikiInitSds() {
	mimalloc
}

void nikiHelp_command(NikiContext* pCtx) {
	if (ctx.args.arguments.size() == 0) {
		sdsstream oss{};
		for (auto& command : ctx.commands.commands)
			oss << command.second.name << ' ' << command.second.getArgumentsNames() << '\n';

		Nikiprint(NikiECHO, oss.str());

	} else {
		sds commandName = ctx.args.getString(0);
		trim(commandName);

		Command* pCommand = ctx.commands.get(commandName);
		if (pCommand == nullptr) {
			Nikiprintf(NikiERROR, "Command \"{}\" not found\n", commandName);
			return;
		}

		pCommand->printAsDataTree();
	}
}

void nikiEcho_command(NikiContext* pCtx) {
	Nikiprintf(NikiECHO, "{}\n", ctx.args.getString(0));
}

void nikiVar_command(NikiContext* pCtx) {
	sds name = ctx.args.getString(0);

	if (name.empty()) {
		Nikiprint(PrintLevel::ERROR, "Variable name can not be empty\n");
		return;
	}

	for (size_t i = 0; i < name.size(); ++i) {
		if (isspace(name[i])) {
			Nikiprint(PrintLevel::ERROR, "Variable name can not contain whitespace\n");
			return;
		}

		switch (name[i]) {
		case NIKI_LOOP_VARIABLE:
			if (i == 0 && name.size() > 1)
				break;
			Nikiprintf(PrintLevel::ERROR, "Can not use {} in a variable name alone or after the first character\n", NIKI_LOOP_VARIABLE);
			return;

		case NIKI_TOGGLE_ON:
			if (i == 0 && name.size() > 1)
				break;
			Nikiprintf(PrintLevel::ERROR, "Can not use {} in a variable name alone or after the first character\n", NIKI_TOGGLE_ON);
			return;

		case NIKI_TOGGLE_OFF:
			if (i == 0 && name.size() > 1)
				break;
			Nikiprintf(PrintLevel::ERROR, "Can not use {} in a variable name alone or after the first character\n", NIKI_TOGGLE_OFF);
			return;

		case NIKI_REFERENCE:
			Nikiprintf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKI_REFERENCE);
			return;

		case NIKI_REFERENCE_OPEN:
			Nikiprintf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKI_REFERENCE_OPEN);
			return;

		case NIKI_REFERENCE_CLOSE:
			Nikiprintf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKI_REFERENCE_CLOSE);
			return;

		case NIKI_ARGUMENTS_SEPARATOR:
			Nikiprintf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKI_ARGUMENTS_SEPARATOR);
			return;

		case NIKI_ARGUMENTS_CLOSE:
			Nikiprintf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKI_ARGUMENTS_CLOSE);
			return;

		case NIKI_ARGUMENTS_OPEN:
			Nikiprintf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKI_ARGUMENTS_OPEN);
			return;

		case NIKI_STATEMENT_SEPARATOR:
			Nikiprintf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKI_STATEMENT_SEPARATOR);
			return;

		case NIKI_COMMENT_LINE:
			Nikiprintf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKI_COMMENT_LINE);
			return;

		case NIKI_COMMENT_LINES:
			Nikiprintf(PrintLevel::ERROR, "Can not use {} in a variable name\n", NIKI_COMMENT_LINES);
			return;
		}
	}

	if (ctx.programVariables.count(name) != 0) {
		Nikiprint(PrintLevel::ERROR, "A program variable already uses this name and therefore can not be replaced\n");
		return;
	}

	if (ctx.commands.commands.count(name) != 0) {
		Nikiprint(PrintLevel::ERROR, "A command already uses this name and therefore can not be replaced\n");
		return;
	}

	if (ctx.args.arguments.size() == 1)
		ctx.consoleVariables[name] = "";
	else
		ctx.consoleVariables[name] = ctx.args.getString(1);
}

void nikiDelvar_command(NikiContext* pCtx) {
	const sds varName = ctx.args.getString(0);

	if (ctx.consoleVariables.count(varName) == 0) {
		Nikiprintf(PrintLevel::ERROR, "Expected console variable\n");
		return;
	}

	for (size_t i = 0; i < ctx.loopVariablesRunning.size(); ++i) {
		if (ctx.loopVariablesRunning[i]->first == varName) {
			ctx.loopVariablesRunning.erase(ctx.loopVariablesRunning.begin()+i);
			break;
		}
	}

	if (varName.size() > 1 && varName[0] == '+') {
		sds toggleVarName = varName.substr(1);

		for (size_t i = 0; i < ctx.toggleVariablesRunning.size(); ++i) {
			if (ctx.toggleVariablesRunning[i]->first == toggleVarName) {
				ctx.toggleVariablesRunning.erase(ctx.toggleVariablesRunning.begin()+i);
				break;
			}
		}
	}

	ctx.consoleVariables.erase(varName);
}

void nikiToggle_command(NikiNikiContext* pCtx) {
	const sds varName = ctx.args.getString(0);
	const sds option1 = ctx.args.getString(1);
	const sds option2 = ctx.args.getString(2);

	if (ctx.consoleVariables.count(varName) != 0) {
		sds varValue = ctx.consoleVariables[varName];

		if (varValue == option1)
			varValue = option2;
		else
			varValue = option1;

		return;
	}

	NikiProgramVariable& var = ctx.programVariables[varName];
	sds varValue = var.get(ctx, &var);

	if (varValue == option1)
		var.set(ctx, &var, option2);
	else
		var.set(ctx, &var, option1);
}

void nikiExec_command(NikiContext* pCtx) {
	parseFile(ctx, ctx.args.getString(0).c_str(), 1);
}

void nikiIncrementvar_command(NikiContext* pCtx) {
	const sds variableName = ctx.args.getString(0);

	float min = ctx.args.getFloat(1);
	float max = ctx.args.getFloat(2);
	if (min > max) {
		Nikiprintf(NikiERROR, "max({}) should be higher than min({})\n", max, min);
		return;
	}

	float delta = ctx.args.getFloat(3);

	float value = 0.0;
	if (ctx.consoleVariables.count(variableName) != 0) {
		try {
			value = std::stof(ctx.consoleVariables[variableName]);
		} catch (...) {
			Nikiprintf(NikiERROR, "\"{}\" is not a number\n", ctx.consoleVariables[variableName]);
			return;
		}
	} else {
		try {
			value = std::stof(ctx.programVariables[variableName].get(ctx, &ctx.programVariables[variableName]));
		} catch (...) {
			Nikiprintf(NikiERROR, "\"{}\" is not a number\n", ctx.consoleVariables[variableName]);
			return;
		}
	}

	value += delta;
	if (value > max)
		value = min;

	if (value < min)
		value = min;

	if (ctx.consoleVariables.count(variableName) != 0) {
		ctx.consoleVariables[variableName] = std::to_string(value);
	} else
		ctx.programVariables[variableName].set(ctx, &ctx.programVariables[variableName], std::to_string(value));
}

void nikiRegisterCommands(NikiNikiContext* pCtx) {
	ctx.commands.add(Command("echo", 1, 1, echo_command, "prints the passed message to console", {"s[message]", "content to print to console"}));
	ctx.commands.add(Command("help", 0,1, help_command, "prints a list of commands with their usages or the usage of the specified command", {"s[command?]", "command to see usage"}));
	ctx.commands.add(Command("var", 1,2, var_command, "creates a variable", {"s[name]", "variable name", "s[value?]", "if value is not specified, variable becomes an empty string"}));
	ctx.commands.add(Command("delvar", 1,1, delvar_command, "deletes a variable", {"v[variable]", "variable to delete"}));
	ctx.commands.add(Command("toggle", 3,3, toggle_command, "toggles value between option1 and option2", {"v[variable]", "variable to modify value", "s[option1]", "option to set variable in case variable value is option2", "s[option2]", "option to set variable in case variable value is option1"}));
	ctx.commands.add(Command("exec", 1,1, exec_command, "parses a script file", {"s[filePath]", "path to the file"}));
	ctx.commands.add(Command("incrementvar", 3,4, incrementvar_command, "do value + delta, when value > max: value = min", {"v[variable]", "variable to modify value", "d[min]", "minimum value possible", "d[max]", "maximum possible value", "d[delta?]", "to increase value with -> value + delta"}));
}

void nikiRegisterVariable(NikiNikiContext* pCtx, const sds name, const sds_view& description, void* pVar, const GetProgramVariableValue& get, const SetProgramVariableValue& set) {
	ctx.programVariables[name] = ProgramVariable(pVar, description, get, set);
}