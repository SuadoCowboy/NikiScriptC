#pragma once

#include "DLLExport.h"
#include "Context.h"

#ifndef NIKI_LOOP_VARIABLE
#define NIKI_LOOP_VARIABLE '!'
#endif

#ifndef NIKI_TOGGLE_ON
#define NIKI_TOGGLE_ON '+'
#endif

#ifndef NIKI_TOGGLE_OFF
#define NIKI_TOGGLE_OFF '-'
#endif

namespace ns {
	/**
	 * @brief Clear anything related to the current lexer statement
	 * @note It does not go to the next EOS
	 */
	NIKIAPI void clearStatementData(NikiContext* pCtx);

	/**
	 * @brief if variable is a toggle variable and its allowed to run or if it's a common variable.
	 * It's also mandatory to call this function so that it updates those special variables
	 *
	 * @note loop variables are only appended/erased from Context::LoopVariablesRunning
	 * @param ctx
	 * @see ns::parse
	 * @see ns::handleIdentifierToken
	 */
	NIKIAPI uint8_t canRunVariable(NikiContext* pCtx);

	/**
	 * @brief Called in parse function when EOS or END is reached
	 * @note Sets ctx.pCommand to nullptr
	 * @note ctx.pCommand should point to a valid Command object
	 * @param ctx
	 * @param pProgramVar if not null: sets/gets program variable
	 * @see ns::parse
	 */
	NIKIAPI void handleCommandCall(NikiContext* pCtx, ProgramVariable*& pProgramVar);

	/**
	 * @brief Called in parse function when IDENTIFIER token is passed
	 * 
	 * @param ctx
	 * @param pProgramVar
	 * @return 0 = it called advanceUntil(EOS) 1 = should call pLexer->advance() 2 = if it's a variable and should run it
	 * @see ns::parse
	 * @see ns::canRunVariable
	 */
	NIKIAPI uint8_t handleIdentifierToken(NikiContext* pCtx, ProgramVariable*& pProgramVar, uint8_t printError);

	/**
	 * @brief Handles references in the string and checks if the parameter matches the argument type
	 * @note Possible argument types: s = string, i = integer, d = decimal, v = variable
	 * @see ns::parse
	 */
	NIKIAPI void handleArgumentToken(NikiContext* pCtx, uint8_t printError);

	/**
	 * @brief Called in parse function when an ConsoleVariable is passed as a IDENTIFIER
	 * @note I find this algorithm very interesting and fun(it creates a bunch of lexers for each variable call instead of recursive calling)
	 * @see ns::Context::maxConsoleVariablesRecursiveDepth
	 * @param ctx
	 * @param pProgramVar
	 */
	NIKIAPI void handleConsoleVariableCall(NikiContext* pCtx, ProgramVariable*& pProgramVar, uint8_t printError);

	/**
	 * @brief if a loop variable is active, its script is ran here
	 * @param ctx
	 */
	NIKIAPI void updateLoopVariables(NikiContext* pCtx);

	/**
	 * @brief Parses and interpret scripts: handles commands and variables as well as their arguments
	 * @param ctx
	 */
	NIKIAPI void parse(NikiContext* pCtx, uint8_t printError=true);

	NIKIAPI uint8_t parseFile(NikiContext* pCtx, const char* filePath, uint8_t printError);
}