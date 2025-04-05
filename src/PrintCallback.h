#pragma once

#include <stdint.h>

#include <sds.h>

#include "DLLExport.h"
#include "Utils.h"

typedef enum NikiPrintLevel {
	DEFAULT = 0, ///< any user input
	ECHO, ///< any text that came from a command that is not an error
	WARNING, ///< anything that is wrong but can continue
	ERROR, ///< anything that went wrong and can not continue
} NikiPrintLevel;

NIKIAPI sds NikiPrintLevelToString(NikiPrintLevel level);

typedef void(*NikiPrintCallback)(void* pData, struct NikiContext* pCtx, NikiPrintLevel level, const sds message);

NIKIAPI void nikiPrintf(NikiPrintLevel level, const sds format, ...);

NIKIAPI void nikiSetPrintCallback(void* pData, PrintCallback callback);
NIKIAPI void nikiPrint(const PrintLevel& level, const sds str);
NIKIAPI void nikiPrintUnknownCommand(const sds command);