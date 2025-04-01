#pragma once

// #include <vector>
#include <stdint.h>
#include <sds.h>
// #include <unordered_map>

#include "DLLExport.h"
#include "Command.h"
// #include "CommandHandler.h"
// #include "PrintCallback.h"
// #include "ProgramVariable.h"
// #include "Lexer.h"

/**
 * @warning **DO NOT** rearrange this enum. ns::handleConsoleVariableCall uses bit logic on VARIABLE related to VARIABLE_IN_VARIABLE
 */
typedef enum {
	COMMAND = 1, ///< if a command is calling another command
	VARIABLE = 2, ///< any variable
	VARIABLE_IN_VARIABLE = 4, ///< var x that calls var y
	VARIABLE_LOOP = 8, ///< '!'
	VARIABLE_TOGGLE = 16, ///< '+' or '-'
	FILE = 32, ///< ns::parseFile or exec command
	INTERNAL = 64, ///< raw script generated from C++ code and not from a file or variable or anything else
} OriginType;
typedef struct NIKIAPI {
	sds arg;
	size_t size;
} NikiCommandArgument;

typedef struct NIKIAPI {
	char* name;
	char* value;
} NikiConsoleVariable;

typedef std::vector<ConsoleVariables::pointer> LoopVariablesRunning;
typedef std::vector<ConsoleVariables::pointer> ToggleVariablesRunning; ///< This is unecessary to be a pointer but I like the idea of using only 8 bytes instead of the same bytes as the var name
typedef std::vector<Command*> ToggleCommandsRunning;

typedef struct NIKIAPI {
	NikiLexer* pLexer;

	NikiCommand* pCommand;

	NikiCommandArgument** pArgs;

	NikiConsoleVariable** pConsoleVariables;
	NikiProgramVariables programVariables;

	NikiCommandHandler commands;

	LoopVariablesRunning loopVariablesRunning;
	ToggleVariablesRunning toggleVariablesRunning;
	ToggleCommandsRunning toggleCommandsRunning;

	char* filePath; ///< when running script from a file
	size_t lineCount;

	uint8_t origin; ///< this is used so that the command knows where he's running in. See ns::OriginType

	uint16_t maxConsoleVariablesRecursiveDepth; ///< How many console variables can be called inside each other 
} NikiContext;

/**
 * @brief Do not copy Context without calling this function.
 * LoopVariablesRunning and ToggleVariablesRunning stores pointers
 * pointed to ConsoleVariables as well as toggleCommandsRunning whose
 * pointers are from CommandHandler. That's why this function exists:
 * It updates all those pointers.
 * @param pSource object to copy content from
 */
NIKIAPI Context ns_copyContext(const Context* pSource);