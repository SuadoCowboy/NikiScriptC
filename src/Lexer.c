#include "Lexer.h"

#include <sstream>

#include "Utils.h"

ns::Lexer::Lexer() {}
ns::Lexer::Lexer(const std::string& input) : input(input) {}

void ns::Lexer::advance() {
	token.references.clear();
	while (position < input.size() && isSpaceNotNewline(input[position]))
		++position;

	if (position >= input.size()) {
		token.type = TokenType::END;
		token.value = "";
		return;
	}

	if (input[position] == '\n')
		lineIndex++;

	uint64_t nextTokenPosition = setTokenValue();
	setTokenType();

	position = nextTokenPosition;
}

void ns::Lexer::advanceUntil(uint8_t flags) {
	flags |= static_cast<uint8_t>(TokenType::END);

	advance();
	while (!(flags & token.type))
		advance();
}

uint64_t ns::Lexer::setTokenValue() {
	if (input[position] == NIKISCRIPT_STATEMENT_SEPARATOR || input[position] == '\n') {
		token.value = NIKISCRIPT_STATEMENT_SEPARATOR;
		return position+1;
	}

	uint64_t nextTokenPosition = position;
	std::stringstream result{};

	/*
	1 = allow white space and NIKISCRIPT_STATEMENT_SEPARATOR
	2 = escape next char
	4 = skipping all until NIKISCRIPT_COMMENT_LINES+NIKISCRIPT_COMMENT_LINE is found
	*/
	unsigned char flags = openArguments == 0? 0 : 1;

	while (nextTokenPosition < input.size() && (position == nextTokenPosition || ((!isSpaceNotNewline(input[nextTokenPosition]) && (input[nextTokenPosition] != NIKISCRIPT_STATEMENT_SEPARATOR || (flags & 2))) || (flags & 1)))) {
		if (flags & 2) {
			flags &= ~2;
			result << input[nextTokenPosition++];
			continue;
		}

		if (flags & 4) {
			if (input[nextTokenPosition] == '\n')
				++lineIndex;

			if (input[nextTokenPosition] == NIKISCRIPT_COMMENT_LINE && input[nextTokenPosition-1] == NIKISCRIPT_COMMENT_LINES)
				flags &= ~5;

			++nextTokenPosition;
			continue;
		}

		if (input[nextTokenPosition] == '\n')
			break;

		if (nextTokenPosition+1 < input.size() && input[nextTokenPosition] == NIKISCRIPT_COMMENT_LINE) {
			if (input[nextTokenPosition+1] == NIKISCRIPT_COMMENT_LINE) {
				size_t i = nextTokenPosition;
				for (; i < input.size(); ++i) {
					if (input[i] == '\n')
						break;
				}

				nextTokenPosition = i;
				break;
			}

			if (input[nextTokenPosition+1] == NIKISCRIPT_COMMENT_LINES) {
				flags |= 5;
				nextTokenPosition += 3;
				continue;
			}
		}

		if (input[nextTokenPosition] == NIKISCRIPT_ARGUMENTS_OPEN) {
			if (token.type == TokenType::NONE || ((TokenType::EOS|TokenType::END) & token.type))
				break;

			++openArguments;
			flags |= 1;

			if (openArguments == 1) {
				++nextTokenPosition;
				continue;
			}

		} else if (input[nextTokenPosition] == NIKISCRIPT_ARGUMENTS_SEPARATOR && openArguments == 1) {
			++nextTokenPosition;
			break;

		} else if (input[nextTokenPosition] == NIKISCRIPT_ARGUMENTS_CLOSE && openArguments != 0) {
			--openArguments;
			if (openArguments == 0) {
				++nextTokenPosition;
				break;
			}
		}

		if (input[nextTokenPosition] == NIKISCRIPT_ESCAPE_NEXT_CHAR) {
			flags |= 2;
			++nextTokenPosition;
			continue;

		} else if (input[nextTokenPosition] == NIKISCRIPT_REFERENCE && nextTokenPosition+1 < input.size() && input[nextTokenPosition+1] == NIKISCRIPT_REFERENCE_OPEN) {
			std::stringstream referenceStream;

			uint64_t tempIndex = nextTokenPosition+2;
			
			bool foundCloseReference = false;
			for (; tempIndex < input.size() && !isSpaceNotNewline(input[tempIndex]); ++tempIndex) {
				if (input[tempIndex] == NIKISCRIPT_REFERENCE_CLOSE) {
					++tempIndex;
					foundCloseReference = true;
					break;
				}

				referenceStream << input[tempIndex];
			}

			if (foundCloseReference) {
				token.references.emplace_back(result.str().size(), referenceStream.str());
				nextTokenPosition = tempIndex;
				continue;
			}
		
		} else if (input[nextTokenPosition] == NIKISCRIPT_ARGUMENTS_QUOTE && openArguments == 0) {
			++nextTokenPosition;
			
			if (flags & 1) {
				flags &= ~1;
				break;
			} else {
				flags |= 1;
				continue;
			}
		}

		result << input[nextTokenPosition++];
	}

	token.value = result.str();
	return nextTokenPosition;
}

void ns::Lexer::setTokenType() {
	if (!token.value.empty() && token.value[0] == NIKISCRIPT_STATEMENT_SEPARATOR) {
		token.type = TokenType::EOS;

	} else if (token.type == TokenType::NONE || ((TokenType::EOS|TokenType::END) & token.type)) { // if the lexer just started and is not EOS
		token.type = TokenType::IDENTIFIER;

	} else if ((TokenType::IDENTIFIER|TokenType::ARGUMENT) & token.type)
		token.type = TokenType::ARGUMENT;
}

void ns::Lexer::clear() {
	input.clear();
	position = 0;
	token = {TokenType::NONE};
	openArguments = 0;
	lineIndex = 0;
}