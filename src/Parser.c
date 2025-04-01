#include "Parser.h"

#include <algorithm>
#include <fstream>
#include <sstream>

#include "PrintCallback.h"

void ns::clearStatementData(Context& ctx) {
	ctx.pCommand = nullptr;
	ctx.args.arguments.clear();
}

uint8_t ns::canRunVariable(Context& ctx) {
	switch (ctx.pLexer->token.value[0]) {
	case NIKISCRIPT_TOGGLE_ON: {
		ConsoleVariables::pointer pVarPair = &*ctx.consoleVariables.find(ctx.pLexer->token.value);
		auto it = std::find(ctx.toggleVariablesRunning.begin(), ctx.toggleVariablesRunning.end(), pVarPair);

		if (it == ctx.toggleVariablesRunning.end()) {
			ctx.toggleVariablesRunning.push_back(pVarPair);

			return true;
		}

		return false;
	}

	case NIKISCRIPT_TOGGLE_OFF: {
		ConsoleVariables::pointer pPlusVariable = nullptr;
		{
			auto it = ctx.consoleVariables.find('+'+ctx.pLexer->token.value.substr(1));
			if (it == ctx.consoleVariables.end())
				return false;

			pPlusVariable = &*it;
		}

		auto it = std::find(ctx.toggleVariablesRunning.begin(), ctx.toggleVariablesRunning.end(), pPlusVariable);
		if (it != ctx.toggleVariablesRunning.end()) {
			ctx.toggleVariablesRunning.erase(it);

			return true;
		}
		return false;
	}

	case NIKISCRIPT_LOOP_VARIABLE: {
		ConsoleVariables::pointer pVar = &*ctx.consoleVariables.find(ctx.pLexer->token.value);

		auto it = std::find(ctx.loopVariablesRunning.begin(), ctx.loopVariablesRunning.end(), pVar);
		if (it == ctx.loopVariablesRunning.end())
			ctx.loopVariablesRunning.push_back(pVar);
		else
			ctx.loopVariablesRunning.erase(it);
		return false;
	}

	default:
		return true;
	}
}

void ns::handleCommandCall(Context& ctx, ProgramVariable*& pProgramVar) {
	if (pProgramVar != nullptr) {
		if (ctx.args.arguments.size() == 0)
			ns::printf(ns::ECHO, "Value: {}\n{}\n", pProgramVar->get(ctx, pProgramVar), pProgramVar->description);
		else
			pProgramVar->set(ctx, pProgramVar, ctx.args.arguments[0]);

		clearStatementData(ctx);
		pProgramVar = nullptr;
		return;
	}

	if (ctx.pCommand == nullptr)
		return;

	if (ctx.pCommand->minArgs > ctx.args.arguments.size()) {
		if (ctx.pCommand->minArgs == ctx.pCommand->maxArgs)
			ns::printf(ns::ERROR, "Expected {} argument(s) but received {} arguments\n", static_cast<uint16_t>(ctx.pCommand->minArgs), ctx.args.arguments.size());
		else
			ns::printf(ns::ERROR, "Expected arguments between [{}, {}] but received {} arguments\n", static_cast<uint16_t>(ctx.pCommand->minArgs), static_cast<uint16_t>(ctx.pCommand->maxArgs), ctx.args.arguments.size());

		ns::printf(ns::ECHO, "{} {}\n", ctx.pCommand->name, ctx.pCommand->getArgumentsNames());
		clearStatementData(ctx);
		return;
	}

	switch (ctx.pCommand->name[0]) {
	case NIKISCRIPT_TOGGLE_ON: {
		auto it = std::find(ctx.toggleCommandsRunning.begin(), ctx.toggleCommandsRunning.end(), ctx.pCommand);

		if (it == ctx.toggleCommandsRunning.end())
			ctx.toggleCommandsRunning.push_back(ctx.pCommand);

		else {
			clearStatementData(ctx);
			return;
		}

		break;
	}
	
	case NIKISCRIPT_TOGGLE_OFF: {
		Command* pPlusCommand = ctx.commands.get('+'+std::string(ctx.pCommand->name.substr(1)));
		if (pPlusCommand == nullptr)
			break;

		auto it = std::find(ctx.toggleCommandsRunning.begin(), ctx.toggleCommandsRunning.end(), pPlusCommand);

		if (it == ctx.toggleCommandsRunning.end()) {
			clearStatementData(ctx);
			return;

		} else
			ctx.toggleCommandsRunning.erase(it);

		break;
	}
	}

	ctx.pCommand->callback(ctx);
	clearStatementData(ctx);
}

uint8_t ns::handleIdentifierToken(Context& ctx, ProgramVariable*& pProgramVar, uint8_t printError) {
	if (ctx.pLexer->token.value.empty()) {
		ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
		return 1;
	}

	if (ctx.consoleVariables.count(ctx.pLexer->token.value) != 0) {
		if (canRunVariable(ctx))
			return 2;

		ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
		return 0;

	} else if (ctx.programVariables.count(ctx.pLexer->token.value) != 0) {
		pProgramVar = &ctx.programVariables[ctx.pLexer->token.value];
		return 1;

	} else {
		ctx.pCommand = ctx.commands.get(ctx.pLexer->token.value);

		if (ctx.pCommand == nullptr) {
			if (printError)
				ns::printf(PrintLevel::ERROR, "Unknown identifier \"{}\"\n", ctx.pLexer->token.value);
			ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
			return 0;
		} else
			return 1;
	}

	return false;
}

void ns::handleArgumentToken(Context& ctx, uint8_t printError) {
	insertReferencesInToken(ctx, ctx.pLexer->token);

	if (ctx.pLexer->token.value.empty())
		return;

	if (ctx.pCommand == nullptr) { // if command is nullptr then just append arguments to a single one. This is useful for ProgramVariable
		if (ctx.args.arguments.size() == 0)
			ctx.args.arguments.push_back(ctx.pLexer->token.value);
		else
			ctx.args.arguments.back() += ' '+ctx.pLexer->token.value;
		return;
	}

	if (ctx.pCommand->maxArgs == 0) {
		if (printError)
			ns::printf(ns::ERROR, "Expected 0 arguments for {} command\n", ctx.pCommand->name);
		clearStatementData(ctx);
		ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
		return;
	}

	if (ctx.args.arguments.size() == ctx.pCommand->maxArgs) {
		ctx.pLexer->token.value = ctx.args.arguments.back()+' '+ctx.pLexer->token.value;
		ctx.args.arguments.pop_back();
	}

	const std::string_view& arg = ctx.pCommand->argsDescriptions[ctx.args.arguments.size()*2];
	switch (arg[0]) {
	case 'i':
		try {
			std::stoll(ctx.pLexer->token.value);
		} catch (...) {
			if (printError)
				ns::printf(PrintLevel::ERROR, "{} -> Type not matched: expected (i)nteger number\n", arg);
			clearStatementData(ctx);
			ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
			return;
		}
		break;

	case 'd':
		try {
			std::stold(ctx.pLexer->token.value);
		} catch (...) {
			if (printError)
				ns::printf(PrintLevel::ERROR, "{} -> Type not matched: expected (d)ecimal number\n", arg);
			clearStatementData(ctx);
			ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
			return;
		}
		break;

	case 's':
		break;

	case 'v':
		if (ctx.programVariables.count(ctx.pLexer->token.value) == 0 && ctx.consoleVariables.count(ctx.pLexer->token.value) == 0) {
			if (printError)
				ns::printf(PrintLevel::ERROR, "{} -> Type not matched: expected (v)ariable\n", arg);
			clearStatementData(ctx);
			ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
			return;
		}
		break;

	default: // should never happen
		break;
	}

	ctx.args.arguments.push_back(ctx.pLexer->token.value);
}

void ns::handleConsoleVariableCall(Context& ctx, ProgramVariable*& pProgramVar, uint8_t printError) {
	Lexer* pOriginalLexer = ctx.pLexer;

	std::vector<Lexer> tempLexers;
	tempLexers.emplace_back(ctx.consoleVariables[ctx.pLexer->token.value]);

	ctx.pLexer = &tempLexers.back();
	ctx.pLexer->advance();

	ctx.origin |= (ctx.origin & OriginType::VARIABLE)<<1; // if (ctx.origin & VARIABLE(2)) ctx.origin |= VARIABLE_IN_VARIABLE(4)
	ctx.origin |= OriginType::VARIABLE;

	while (tempLexers.size() != 0) {
		switch (ctx.pLexer->token.type) {
		case TokenType::IDENTIFIER:
			if (handleIdentifierToken(ctx, pProgramVar, printError) == 2) {
				if (ctx.maxConsoleVariablesRecursiveDepth != 0 && tempLexers.size() >= ctx.maxConsoleVariablesRecursiveDepth) {
					ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
					break;
				}

				tempLexers.emplace_back(ctx.consoleVariables[ctx.pLexer->token.value]);
				ctx.pLexer = &tempLexers.back();
				ctx.origin |= OriginType::VARIABLE_IN_VARIABLE;
			}
			break;

		case TokenType::EOS:
			handleCommandCall(ctx, pProgramVar);
			break;

		case TokenType::ARGUMENT:
			handleArgumentToken(ctx, printError);
			break;

		default:
			break;
		}

		ctx.pLexer->advance();
		while (ctx.pLexer->token.type == TokenType::END) {
			handleCommandCall(ctx, pProgramVar);

			tempLexers.pop_back();
			if (tempLexers.size() == 0)
				break;
			else if (tempLexers.size() == 1 && !(ctx.origin & OriginType::VARIABLE_LOOP))
				ctx.origin &= ~OriginType::VARIABLE_IN_VARIABLE;

			ctx.pLexer = &tempLexers.back();
			ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
		}
	}

	if (!(ctx.origin & OriginType::VARIABLE_LOOP))
		ctx.origin &= ~OriginType::VARIABLE;

	ctx.pLexer = pOriginalLexer;
}

void ns::updateLoopVariables(Context& ctx) {
	ctx.origin |= OriginType::VARIABLE|OriginType::VARIABLE_LOOP;

	ctx.pLexer->clear();
	for (auto& pVar : ctx.loopVariablesRunning) {
		ctx.pLexer->input = pVar->second;
		parse(ctx);
		ctx.pLexer->clear();
	}
	
	ctx.origin &= ~(OriginType::VARIABLE|OriginType::VARIABLE_LOOP);
}

void ns::parse(Context& ctx, uint8_t printError) {
	if (ctx.pLexer == nullptr)
		return;

	ProgramVariable* pProgramVar = nullptr;

	ctx.pLexer->advance();
	while (ctx.pLexer->token.type != TokenType::END) {
		switch (ctx.pLexer->token.type) {
		case TokenType::IDENTIFIER: { // can be either variable or command
			uint8_t result = handleIdentifierToken(ctx, pProgramVar, printError);
			if (result == 2) {
				handleConsoleVariableCall(ctx, pProgramVar, printError);
				ctx.pLexer->advanceUntil(static_cast<uint8_t>(TokenType::EOS));
			} else if (result == 1)
				ctx.pLexer->advance();
			break;
		}

		case TokenType::ARGUMENT:
			handleArgumentToken(ctx, printError);
			ctx.pLexer->advance();
			break;

		case TokenType::EOS:
			handleCommandCall(ctx, pProgramVar);
			ctx.pLexer->advance();
			break;

		default:
			ctx.pLexer->advance();
			break;
		}
	}

	handleCommandCall(ctx, pProgramVar);
}

uint8_t ns::parseFile(Context& ctx, const char* filePath, uint8_t printError) {
	std::ifstream file{filePath};

	if (!file) {
		if (printError)
			printf(PrintLevel::ERROR, "Could not load file \"{}\"\n", filePath);
		return false;
	}

	uint8_t runningFromAnotherFile = (ctx.origin & OriginType::FILE);
	ctx.origin |= OriginType::FILE;

	std::string originalFilePath = ctx.filePath;
	ctx.filePath = filePath;

	std::stringstream script{};
	while (file.good()) {
		std::string line;
		std::getline(file, line);

		if (!line.empty())
			script << line << '\n';
	}

	Arguments originalArguments = ctx.args;
	ctx.args.arguments.clear();

	Lexer* pOriginalLexer = ctx.pLexer;

	Command* pOriginalCommand = ctx.pCommand;
	ctx.pCommand = nullptr;

	size_t originalLineCount = ctx.lineCount;

	Lexer lexer{script.str()};
	ctx.pLexer = &lexer;
	parse(ctx);
	ctx.pLexer = pOriginalLexer;

	ctx.lineCount = originalLineCount;
	ctx.pCommand = pOriginalCommand;
	ctx.args = originalArguments;
	ctx.filePath = originalFilePath;

	if (!runningFromAnotherFile)
		ctx.origin &= ~OriginType::FILE;

	return true;
}