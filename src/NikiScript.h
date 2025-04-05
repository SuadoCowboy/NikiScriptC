#pragma once

#include <sds.h>

#include "DLLExport.h"
#include "Context.h"

NIKIAPI void nikiInitSds();

/**
 * @brief shows command usage
 * @param ctx
 * @note s[command?]
 */
NIKIAPI void nikiHelp_command(NikiContext* pCtx);

/**
 * @brief prints all the arguments passed
 * @param ctx
 * @note s[message]
 */
NIKIAPI void nikiEcho_command(NikiContext* pCtx);

/**
 * @brief creates a variable
 * @param ctx
 * @note s[name] s[value]
 */
NIKIAPI void nikiVar_command(NikiContext* pCtx);
/**
 * @brief deletes a variable
 * @param ctx
 * @note v[consoleVariable]
 */
NIKIAPI void nikiDelvar_command(NikiContext* pCtx);

/**
 * @brief toggles a variable value between option1 and option2
 * @param ctx
 * @note v[variable] s[option1] s[option2]
 */
NIKIAPI void nikiToggle_command(NikiContext* pCtx);

/**
 * @brief parses a file as nikiscript
 * @param ctx
 * @note s[filePath]
 */
NIKIAPI void nikiExec_command(NikiContext* pCtx);

/**
 * @brief increments a variable value
 * @param ctx
 * @note v[variable] n[min] n[max] n[delta?]
 */
NIKIAPI void nikiIncrementvar_command(NikiContext* pCtx);

/**
 * @brief adds default commands such as echo
 * @param ctx
 * @see echo_command
 */
NIKIAPI void nikiRegisterCommands(NikiContext* pCtx);

/**
 * @brief creates a variable and stores it in ns::Context::programVariables
 * @param ctx
 * @param name
 * @param description
 * @param pVar
 * @param get
 * @param set
 */
NIKIAPI void nikiRegisterVariable(NikiContext* pCtx, const sds name, const sds_view& description, void* pVar, const GetProgramVariableValue& get, const SetProgramVariableValue& set);