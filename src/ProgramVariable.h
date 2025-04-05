#pragma once

#include <stdint.h>

#include <sds.h>
// #include <unordered_map>

#include "DLLExport.h"

#ifndef NIKI_PROGRAM_VARIABLES_SIZE_TYPE
#define NIKI_PROGRAM_VARIABLES_SIZE_TYPE uint32_t
#endif1

struct ProgramVariable;
struct Context;

typedef sds(*GetProgramVariableValue)(NikiContext* pCtx, ProgramVariable* pVar);
typedef void(*SetProgramVariableValue)(NikiContext* pCtx, ProgramVariable* pVar, const sds str);

struct Context;

typedef struct NIKIAPI ProgramVariable {
	void* pValue;
	sds description;

	GetProgramVariableValue get;
	SetProgramVariableValue set;
} NikiProgramVariable;

typedef struct NIKIAPI NikiProgramVariables {
	NikiProgramVariable* pVars;
	NIKI_PROGRAM_VARIABLES_SIZE_TYPE size;
} NikiProgramVariables;

NIKIAPI sds getString(NikiContext*, NikiProgramVariable* pVar);
NIKIAPI void setString(NikiContext*, NikiProgramVariable* pVar, const sds& str);

template<typename T>
sds getNumber(NikiContext*, ProgramVariable* pVar) {
	return std::to_string(*static_cast<T*>(pVar->pValue));
}

/**
 * @brief uses std::stoul so any number below that can be used
 * @tparam T number type
 * @param ctx
 * @param pVar 
 * @param str
 */

NIKIAPI void setUnsignedLongLong(NikiContext*, ProgramVariable* pVar, const sds& str);

NIKIAPI void setFloat(NikiContext*, ProgramVariable* pVar, const sds& str);
NIKIAPI void setDouble(NikiContext*, ProgramVariable* pVar, const sds& str);
NIKIAPI void setLongDouble(NikiContext*, ProgramVariable* pVar, const sds& str);

NIKIAPI void setChar(NikiContext*, ProgramVariable* pVar, const sds& str);
NIKIAPI void setShort(NikiContext*, ProgramVariable* pVar, const sds& str);
NIKIAPI void setInteger(NikiContext*, ProgramVariable* pVar, const sds& str);
NIKIAPI void setLong(NikiContext*, ProgramVariable* pVar, const sds& str);
NIKIAPI void setLongLong(NikiContext*, ProgramVariable* pVar, const sds& str);