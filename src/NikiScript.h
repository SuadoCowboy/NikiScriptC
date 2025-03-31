#pragma once

#include <string_view>

#include "DLLExport.h"
#include "Context.h"

namespace ns {
	/**
	 * @brief shows command usage
	 * @param ctx
	 * @note s[command?]
	 */
	NIKIAPI void help_command(Context& ctx);

	/**
	 * @brief prints all the arguments passed
	 * @param ctx
	 * @note s[message]
	 */
	NIKIAPI void echo_command(Context& ctx);

	/**
	 * @brief creates a variable
	 * @param ctx
	 * @note s[name] s[value]
	 */
	NIKIAPI void var_command(Context& ctx);
	/**
	 * @brief deletes a variable
	 * @param ctx
	 * @note v[consoleVariable]
	 */
	NIKIAPI void delvar_command(Context& ctx);

	/**
	 * @brief toggles a variable value between option1 and option2
	 * @param ctx
	 * @note v[variable] s[option1] s[option2]
	 */
	NIKIAPI void toggle_command(Context& ctx);

	/**
	 * @brief parses a file as nikiscript
	 * @param ctx
	 * @note s[filePath]
	 */
	NIKIAPI void exec_command(Context& ctx);

	/**
	 * @brief increments a variable value
	 * @param ctx
	 * @note v[variable] n[min] n[max] n[delta?]
	 */
	NIKIAPI void incrementvar_command(Context& ctx);

	/**
	 * @brief adds default commands such as echo
	 * @param ctx
	 * @see echo_command
	 */
	NIKIAPI void registerCommands(Context& ctx);

	/**
	 * @brief creates a variable and stores it in ns::Context::programVariables
	 * @param ctx
	 * @param name
	 * @param description
	 * @param pVar
	 * @param get
	 * @param set
	 */
	NIKIAPI void registerVariable(Context& ctx, const std::string& name, const std::string_view& description, void* pVar, const GetProgramVariableValue& get, const SetProgramVariableValue& set);
}