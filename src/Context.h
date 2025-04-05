#pragma once

// #include <vector>
#include <stdint.h>
// #include <unordered_map>

#include <sds.h>

#include "DLLExport.h"
#include "Command.h"
#include "CommandHandler.h"
#include "PrintCallback.h"
#include "ProgramVariable.h"
#include "Lexer.h"

/**
 * @warning **DO NOT** rearrange this enum. nikiHandleConsoleVariableCall uses bit logic on VARIABLE related to VARIABLE_IN_VARIABLE
 */
typedef enum NikiOriginType {
	NIKI_ORIGIN_COMMAND = 1, ///< if a command is calling another command
	NIKI_ORIGIN_VARIABLE = 2, ///< any variable
	NIKI_ORIGIN_VARIABLE_IN_VARIABLE = 4, ///< var x that calls var y
	NIKI_ORIGIN_VARIABLE_LOOP = 8, ///< '!'
	NIKI_ORIGIN_VARIABLE_TOGGLE = 16, ///< '+' or '-'
	NIKI_ORIGIN_FILE = 32, ///< ns::parseFile or exec command
	NIKI_ORIGIN_INTERNAL = 64, ///< raw script generated from C++ code and not from a file or variable or anything else
} NikiOriginType;

typedef struct NIKIAPI NikiCommandArgument {
	sds arg;
	size_t size;
} NikiCommandArgument;

typedef struct NIKIAPI NikiConsoleVariable {
	sds name;
	sds value;
} NikiConsoleVariable;

typedef NikiConsoleVariable** nikiLoopVariablesRunning;
typedef NikiConsoleVariable** nikiToggleVariablesRunning; ///< This is unecessary to be a pointer but I like the idea of using only 8 bytes instead of the same bytes as the var name
typedef NikiCommand* nikiToggleCommandsRunning;

typedef struct NIKIAPI NikiContext {
	NikiPrintCallback nikiPrintCallback;
	void* pPrintCallbackData;

	NikiLexer* pLexer;

	NikiCommand* pCommand;

	NikiCommandArgument** pArgs;

	NikiConsoleVariable** pConsoleVariables;
	NikiProgramVariables programVariables;

	NikiCommandHandler commands;

	NikiLoopVariablesRunning loopVariablesRunning;
	NikiToggleVariablesRunning toggleVariablesRunning;
	NikiToggleCommandsRunning toggleCommandsRunning;

	sds filePath; ///< when running script from a file
	size_t lineCount;

	uint8_t origin; ///< this is used so that the command knows where he's running in. See NikiOriginType

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
NIKIAPI Context nikiCopyContext(const Context* pSource);