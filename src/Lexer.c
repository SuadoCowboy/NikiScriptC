#include "Lexer.h"

#include "Utils.h"

void nikiLexerAdvance(NikiLexer* pLexer) {
	nikiReferencesClear(pLexer->token.references)

	while (pLexer->position < pLexer->inputSize && isSpaceNotNewline(pLexer->input[pLexer->position]))
		++pLexer->position;

	if (pLexer->position >= pLexer->inputSize) {
		token.type = NIKI_TOKEN_END;
		token.value = "";
		return;
	}

	if (pLexer->input[pLexer->position] == '\n')
		lineIndex++;

	uint64_t nextTokenPosition = nikiLexerSetTokenValue();
	nikiLexerSetTokenType();

	pLexer->position = nextTokenPosition;
}

void nikiLexerAdvanceUntil(NikiLexer* pLexer, uint8_t flags) {
	flags |= static_cast<uint8_t>(NIKI_TOKEN_END);

	advance();
	while (!(flags & token.type))
		advance();
}

NIKI_LEXER_INPUT_SIZE_TYPE nikiLexerSetTokenValue(NikiLexer* pLexer) {
	if (pLexer->input[pLexer->position] == NIKI_STATEMENT_SEPARATOR || pLexer->input[pLexer->position] == '\n') {
		token.value = NIKI_STATEMENT_SEPARATOR;
		return pLexer->position+1;
	}

	NIKI_LEXER_INPUT_SIZE_TYPE nextTokenPosition = pLexer->position;
	sds result = sdsempty();

	/*
	1 = allow white space and NIKI_STATEMENT_SEPARATOR
	2 = escape next char
	4 = skipping all until NIKI_COMMENT_LINES+NIKI_COMMENT_LINE is found
	*/
	unsigned char flags = pLexer->openArguments == 0? 0 : 1;

	while (nextTokenPosition < pLexer->inputSize && (pLexer->position == nextTokenPosition || ((!isSpaceNotNewline(pLexer->input[nextTokenPosition]) && (pLexer->input[nextTokenPosition] != NIKI_STATEMENT_SEPARATOR || (flags & 2))) || (flags & 1)))) {
		if (flags & 2) {
			flags &= ~2;
			result = sdscat(result, pLexer->input[nextTokenPosition++]);
			continue;
		}

		if (flags & 4) {
			if (pLexer->input[nextTokenPosition] == '\n')
				++pLexer->lineIndex;

			if (pLexer->input[nextTokenPosition] == NIKI_COMMENT_LINE && pLexer->input[nextTokenPosition-1] == NIKI_COMMENT_LINES)
				flags &= ~5;

			++nextTokenPosition;
			continue;
		}

		if (pLexer->input[nextTokenPosition] == '\n')
			break;

		if (nextTokenPosition+1 < pLexer->inputSize && pLexer->input[nextTokenPosition] == NIKI_COMMENT_LINE) {
			if (pLexer->input[nextTokenPosition+1] == NIKI_COMMENT_LINE) {
				size_t i = nextTokenPosition;
				for (; i < pLexer->inputSize; ++i) {
					if (pLexer->input[i] == '\n')
						break;
				}

				nextTokenPosition = i;
				break;
			}

			if (pLexer->input[nextTokenPosition+1] == NIKI_COMMENT_LINES) {
				flags |= 5;
				nextTokenPosition += 3;
				continue;
			}
		}

		if (pLexer->input[nextTokenPosition] == NIKI_ARGUMENTS_OPEN) {
			if (token.type == NIKI_TOKEN_NONE || ((NIKI_TOKEN_EOS|NIKI_TOKEN_END) & token.type))
				break;

			++openArguments;
			flags |= 1;

			if (openArguments == 1) {
				++nextTokenPosition;
				continue;
			}

		} else if (pLexer->input[nextTokenPosition] == NIKI_ARGUMENTS_SEPARATOR && openArguments == 1) {
			++nextTokenPosition;
			break;

		} else if (pLexer->input[nextTokenPosition] == NIKI_ARGUMENTS_CLOSE && openArguments != 0) {
			--openArguments;
			if (openArguments == 0) {
				++nextTokenPosition;
				break;
			}
		}

		if (pLexer->input[nextTokenPosition] == NIKI_ESCAPE_NEXT_CHAR) {
			flags |= 2;
			++nextTokenPosition;
			continue;

		} else if (pLexer->input[nextTokenPosition] == NIKI_REFERENCE && nextTokenPosition+1 < pLexer->inputSize && pLexer->input[nextTokenPosition+1] == NIKI_REFERENCE_OPEN) {
			sds reference;

			uint64_t tempIndex = nextTokenPosition+2;
			
			uint8_t foundCloseReference = false;
			for (; tempIndex < pLexer->inputSize && !isSpaceNotNewline(pLexer->input[tempIndex]); ++tempIndex) {
				if (pLexer->input[tempIndex] == NIKI_REFERENCE_CLOSE) {
					++tempIndex;
					foundCloseReference = true;
					break;
				}

				reference << pLexer->input[tempIndex];
			}

			if (foundCloseReference) {
				nikiReferencesPush(sdslen(result), reference.str())
				nextTokenPosition = tempIndex;
				continue;
			}
		
		} else if (pLexer->input[nextTokenPosition] == NIKI_ARGUMENTS_QUOTE && pLexer->openArguments == 0) {
			++nextTokenPosition;
			
			if (flags & 1) {
				flags &= ~1;
				break;
			} else {
				flags |= 1;
				continue;
			}
		}

		result = sdscat(result, pLexer->input[nextTokenPosition++]);
	}

	token.value = result.str();
	return nextTokenPosition;
}

void nikiLexerSetTokenType(NikiLexer* pLexer) {
	if (!token.value.empty() && token.value[0] == NIKI_STATEMENT_SEPARATOR) {
		token.type = NIKI_TOKEN_EOS;

	} else if (token.type == NIKI_TOKEN_NONE || ((NIKI_TOKEN_EOS|NIKI_TOKEN_END) & token.type)) { // if the lexer just started and is not EOS
		token.type = NIKI_TOKEN_IDENTIFIER;

	} else if ((NIKI_TOKEN_IDENTIFIER|NIKI_TOKEN_ARGUMENT) & token.type)
		token.type = NIKI_TOKEN_ARGUMENT;
}

void nikiLexerClear(NikiLexer* pLexer) {
	pLexer->input.clear();
	pLexer->position = 0;
	token = {NIKI_TOKEN_NONE};
	openArguments = 0;
	lineIndex = 0;
}