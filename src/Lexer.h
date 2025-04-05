#pragma once

#include <stdint.h>

#include <sds.h>

#include "DLLExport.h"
#include "Token.h"

#ifndef NIKI_ESCAPE_NEXT_CHAR
#define NIKI_ESCAPE_NEXT_CHAR '\\'
#endif

#ifndef NIKI_STATEMENT_SEPARATOR
#define NIKI_STATEMENT_SEPARATOR ';'
#endif

#ifndef NIKI_REFERENCE
#define NIKI_REFERENCE '$'
#endif

#ifndef NIKI_REFERENCE_OPEN
#define NIKI_REFERENCE_OPEN '{'
#endif

#ifndef NIKI_REFERENCE_CLOSE
#define NIKI_REFERENCE_CLOSE '}'
#endif

#ifndef NIKI_ARGUMENTS_SEPARATOR
#define NIKI_ARGUMENTS_SEPARATOR ','
#endif

#ifndef NIKI_ARGUMENTS_OPEN
#define NIKI_ARGUMENTS_OPEN '('
#endif

#ifndef NIKI_ARGUMENTS_CLOSE
#define NIKI_ARGUMENTS_CLOSE ')'
#endif

#ifndef NIKI_COMMENT_LINE
#define NIKI_COMMENT_LINE '/'
#endif

#ifndef NIKI_COMMENT_LINES // Joined together with NIKI_COMMENT_LINE -> /* This is a comment */
#define NIKI_COMMENT_LINES '*'
#endif

#ifndef NIKI_ARGUMENTS_QUOTE
#define NIKI_ARGUMENTS_QUOTE '"'
#endif


#ifndef NIKI_LEXER_INPUT_SIZE_TYPE
#define NIKI_LEXER_INPUT_SIZE_TYPE uint64_t
#endif

typedef struct NIKIAPI NikiLexer {
	sds input;
	NIKI_LEXER_INPUT_SIZE_TYPE inputSize;
	NIKI_LEXER_INPUT_SIZE_TYPE position;
	NIKI_LEXER_INPUT_SIZE_TYPE openArguments; ///< how many times NIKI_ARGUMENTS_OPEN was found
	NIKI_LEXER_INPUT_SIZE_TYPE lineIndex; ///< how many newlines were found basically

	/**
	 * @brief This variable is where advance stores the token
	 * @see Lexer::advance
	 */
	NikiToken token;

} NikiLexer;

/**
 * @brief Gets the next token in the input
 * @note Position is set to the **next** token position in the end of this function
 * @see NikiLexer::token
 * @see nikiLexerSetTokenValue
 * @see nikiLexerSetTokenType
 */
void nikiLexerAdvance(NikiLexer* pLexer);

/**
 * @brief Advances tokens until it reaches one of the flags
 * @param flags Bitwise TokenType
 * @see nikiLexerAdvance
 */
void nikiLexerAdvanceUntil(NikiLexer* pLexer, uint8_t flags);

/**
 * @brief Gets token value by checking where a whitespace is found
 *
 * @return Next input's initial position. Either whitespace, EOS or END
 * @see nikiLexerSetTokenType
 */
NIKI_LEXER_INPUT_SIZE_TYPE nikiLexerSetTokenValue(NikiLexer* pLexer);

/**
 * @brief Identifies token type by checking the previous token type
 * @see nikiLexerSetTokenValue
 */
void nikiLexerSetTokenType(NikiLexer* pLexer);

/**
 * @brief resets members
 */
void nikiLexerClear(NikiLexer* pLexer);