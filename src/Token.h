#pragma once
#include <stdint.h>
#include <string>
#include <vector>

#include "DLLExport.h"

namespace ns {
	enum TokenType : uint8_t {
		NONE = 0,
		IDENTIFIER = 1, ///< If it's the first token after an EOS, END or NONE. Should be either a variable or command
		ARGUMENT = 2, ///< If lexer could not convert to integer and an identifier was already caught up
		EOS = 4, ///< End Of Statement
		END = 8 ///< End of input data
	};

	struct NIKIAPI Token {
		TokenType type = TokenType::NONE;
		std::string value;
		std::vector<std::pair<uint64_t, std::string>> references{}; ///< References identified in Token::value. **pair.first** = index where should insert the reference but _does not count with previous inserted references_

		Token();
		Token(TokenType type);
		Token(TokenType type, const std::string& value);
	};

	struct Context;

	/**
	 * @brief inserts all references in the value
	 */
	NIKIAPI void insertReferencesInToken(Context& ctx, Token& token);
}

NIKIAPI uint8_t operator|(ns::TokenType l, ns::TokenType r);
NIKIAPI uint8_t operator|(uint8_t l, ns::TokenType r);
NIKIAPI uint8_t operator|(ns::TokenType l, uint8_t r);
NIKIAPI uint8_t operator&(uint8_t l, ns::TokenType r);
NIKIAPI uint8_t operator&(ns::TokenType l, uint8_t r);