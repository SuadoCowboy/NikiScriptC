#pragma once

#include <stdint.h>

#include <sds.h>
// #include <unordered_map>

#include "DLLExport.h"

#ifndef NIKI_PROGRAM_VARIABLES_SIZE_TYPE
#define NIKI_PROGRAM_VARIABLES_SIZE_TYPE uint32_t
#endif

typedef sds(*GetProgramVariableValue)(struct NikiContext* pCtx, struct NikiProgramVariable* pVar);
typedef void(*SetProgramVariableValue)(struct NikiContext* pCtx, struct NikiProgramVariable* pVar, const sds str);

typedef struct NIKIAPI NikiProgramVariable {
	void* pValue;
	sds description;

	GetProgramVariableValue get;
	SetProgramVariableValue set;
} NikiProgramVariable;

typedef struct NIKIAPI NikiProgramVariables {
	NikiProgramVariable* pVars;
	NIKI_PROGRAM_VARIABLES_SIZE_TYPE size;
} NikiProgramVariables;

NIKIAPI sds nikiProgramVariableGetString(struct NikiContext*, NikiProgramVariable* pVar);
NIKIAPI void nikiProgramVariableSetString(struct NikiContext*, NikiProgramVariable* pVar, const sds str);

/**
 * @brief uses std::stoul so any number below that can be used
 * @tparam T number type
 * @param ctx
 * @param pVar 
 * @param str
 */

NIKIAPI void nikiProgramVariableSetUnsignedLongLong(struct NikiContext*, NikiProgramVariable* pVar, const sds str);

NIKIAPI void nikiProgramVariableSetFloat(struct NikiContext*, NikiProgramVariable* pVar, const sds str);
NIKIAPI void nikiProgramVariableSetDouble(struct NikiContext*, NikiProgramVariable* pVar, const sds str);
NIKIAPI void nikiProgramVariableSetLongDouble(struct NikiContext*, NikiProgramVariable* pVar, const sds str);

NIKIAPI void nikiProgramVariableSetChar(struct NikiContext*, NikiProgramVariable* pVar, const sds str);
NIKIAPI void nikiProgramVariableSetShort(struct NikiContext*, NikiProgramVariable* pVar, const sds str);
NIKIAPI void nikiProgramVariableSetInteger(struct NikiContext*, NikiProgramVariable* pVar, const sds str);
NIKIAPI void nikiProgramVariableSetLong(struct NikiContext*, NikiProgramVariable* pVar, const sds str);
NIKIAPI void nikiProgramVariableSetLongLong(struct NikiContext*, NikiProgramVariable* pVar, const sds str);