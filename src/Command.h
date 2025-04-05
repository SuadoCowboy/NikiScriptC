#pragma once

#include <sds.h>

#include "DLLExport.h"

struct NikiContext;

typedef void(*NikiCommandCallback)(struct NikiContext* pCtx);

typedef struct NIKIAPI NikiCommand {
	sds name;
	unsigned char minArgs, maxArgs;
	NikiCommandCallback callback;
	sds description;

	/**
	 * @note odd = name
	 * @note even = description
	 */
	sds* pArgsDescriptions;
} NikiCommand;

/**
 * @param name
 * @param minArgs
 * @param maxArgs
 * @param callback
 * @param description Command description
 * @param pArgsDescriptions Arguments description. Should have 2 strings for each argument, where the first one is argument name and the second is argument description.
 * @see nikiRegisterCommands for code example
 */
NikiCommand_init(NikiCommand* pCommand, const sds name, unsigned char minArgs, unsigned char maxArgs, NikiCommandCallback callback, const sds description, const sds* pArgsDescriptions);

/**
 * @brief prints usage, description and argsDescriptions all like a data tree
 */
void NikiCommand_printAsDataTree(const NikiCommand* pCommand);