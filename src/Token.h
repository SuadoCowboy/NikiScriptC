#pragma once

#include <stdint.h>

#include <sds.h>

#include "DLLExport.h"

#ifndef NIKI_REFERENCE_INDEX_TYPE
#define NIKI_REFERENCE_INDEX_TYPE uint64_t
#endif

#ifndef NIKI_REFERENCES_SIZE_TYPE
#define NIKI_REFERENCES_SIZE_TYPE uint64_t
#endif

typedef struct NIKIAPI NikiReference {
	NIKI_REFERENCE_INDEX_TYPE index; ///< index where should insert the reference. **does not count with previous inserted references** so it needs to sum with an offset
	sds name;
} NikiReference;

typedef struct NIKIAPI NikiReferences {
	NikiReference* pReferences;
	NIKI_REFERENCES_SIZE_TYPE size;
} NikiReferences;

typedef enum TokenType {
	NIKI_TOKEN_NONE = 0,
	NIKI_TOKEN_IDENTIFIER = 1, ///< If it's the first token after an EOS, END or NONE. Should be either a variable or command
	NIKI_TOKEN_ARGUMENT = 2, ///< If lexer could not convert to integer and an identifier was already caught up
	NIKI_TOKEN_EOS = 4, ///< End Of Statement
	NIKI_TOKEN_END = 8 ///< End of input data
} TokenType;

typedef struct NIKIAPI NikiToken {
	TokenType type;
	sds value;
	NikiReferences references; ///< References identified in nikiToken value.
} NikiToken;

struct NikiContext;

/**
 * @brief inserts all references in the value
 */
NIKIAPI void nikiInsertReferencesInToken(struct NikiContext* pCtx, NikiToken* pToken);