#pragma once

#include <stdint.h>
#include <string>

#include "DLLExport.h"
#include "Token.h"

#ifndef NIKISCRIPT_ESCAPE_NEXT_CHAR
#define NIKISCRIPT_ESCAPE_NEXT_CHAR '\\'
#endif

#ifndef NIKISCRIPT_STATEMENT_SEPARATOR
#define NIKISCRIPT_STATEMENT_SEPARATOR ';'
#endif

#ifndef NIKISCRIPT_REFERENCE
#define NIKISCRIPT_REFERENCE '$'
#endif

#ifndef NIKISCRIPT_REFERENCE_OPEN
#define NIKISCRIPT_REFERENCE_OPEN '{'
#endif

#ifndef NIKISCRIPT_REFERENCE_CLOSE
#define NIKISCRIPT_REFERENCE_CLOSE '}'
#endif

#ifndef NIKISCRIPT_ARGUMENTS_SEPARATOR
#define NIKISCRIPT_ARGUMENTS_SEPARATOR ','
#endif

#ifndef NIKISCRIPT_ARGUMENTS_OPEN
#define NIKISCRIPT_ARGUMENTS_OPEN '('
#endif

#ifndef NIKISCRIPT_ARGUMENTS_CLOSE
#define NIKISCRIPT_ARGUMENTS_CLOSE ')'
#endif

#ifndef NIKISCRIPT_COMMENT_LINE
#define NIKISCRIPT_COMMENT_LINE '/'
#endif

#ifndef NIKISCRIPT_COMMENT_LINES // Joined together with NIKISCRIPT_COMMENT_LINE -> /* This is a comment */
#define NIKISCRIPT_COMMENT_LINES '*'
#endif

#ifndef NIKISCRIPT_ARGUMENTS_QUOTE
#define NIKISCRIPT_ARGUMENTS_QUOTE '"'
#endif

namespace ns {
	struct NIKIAPI Lexer {
		std::string input;
		uint64_t position = 0;
		size_t openArguments = 0; ///< how many times NIKISCRIPT_ARGUMENTS_OPEN was found
		size_t lineIndex = 0; ///< how many newlines were found basically

		/**
		 * @brief This variable is where advance stores the token
		 * @see Lexer::advance
		 */
		Token token = {TokenType::NONE};

		Lexer();
		Lexer(const std::string& input);

		/**
		 * @brief Gets the next token in the input
		 * @note Position is set to the **next** token position in the end of this function
		 * @see Lexer::token
		 * @see Lexer::setTokenValue
		 * @see Lexer::setTokenType
		 */
		void advance();

		/**
		 * @brief Advances tokens until it reaches one of the flags
		 * @param flags Bitwise TokenType
		 * @see Lexer::advance
		 */
		void advanceUntil(uint8_t flags);

		/**
		 * @brief Gets token value by checking where a whitespace is found
		 *
		 * @return Next input's initial position. Either whitespace, EOS or END
		 * @see Lexer::setTokenType
		 */
		uint64_t setTokenValue();

		/**
		 * @brief Identifies token type by checking the previous token type
		 * @see Lexer::setTokenValue
		 */
		void setTokenType();

		/**
		 * @brief resets members
		 */
		void clear();
	};
}