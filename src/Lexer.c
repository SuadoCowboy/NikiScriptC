#include "Lexer.h"

#include "Utils.h"

void nikiLexerAdvance(NikiLexer* pLexer) {
	nikiReferencesClear(pLexer->token.references);

	while (pLexer->position < pLexer->inputSize && isSpaceNotNewline(pLexer->input[pLexer->position]))
		++pLexer->position;

	if (pLexer->position >= pLexer->inputSize) {
		pLexer->token.type = NIKI_TOKEN_END;
		pLexer->token.value = "";
		return;
	}

	if (pLexer->input[pLexer->position] == '\n')
		pLexer->lineIndex++;

	NIKI_LEXER_INPUT_SIZE_TYPE nextTokenPosition = nikiLexerSetTokenValue(pLexer);
	nikiLexerSetTokenType(pLexer);

	pLexer->position = nextTokenPosition;
}

void nikiLexerAdvanceUntil(NikiLexer* pLexer, uint8_t flags) {
	flags |= (uint8_t)NIKI_TOKEN_END;

	advance();
	while (!(flags & pLexer->token.type))
		advance();
}

NIKI_LEXER_INPUT_SIZE_TYPE nikiLexerSetTokenValue(NikiLexer* pLexer) {
	if (pLexer->input[pLexer->position] == NIKI_STATEMENT_SEPARATOR || pLexer->input[pLexer->position] == '\n') {
		pLexer->token.value = NIKI_STATEMENT_SEPARATOR;
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
			result = sdscatlen(result, pLexer->input[nextTokenPosition++], 1);
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
			if (pLexer->token.type == NIKI_TOKEN_NONE || ((NIKI_TOKEN_EOS|NIKI_TOKEN_END) & pLexer->token.type))
				break;

			++pLexer->openArguments;
			flags |= 1;

			if (pLexer->openArguments == 1) {
				++nextTokenPosition;
				continue;
			}

		} else if (pLexer->input[nextTokenPosition] == NIKI_ARGUMENTS_SEPARATOR && pLexer->openArguments == 1) {
			++nextTokenPosition;
			break;

		} else if (pLexer->input[nextTokenPosition] == NIKI_ARGUMENTS_CLOSE && pLexer->openArguments != 0) {
			--pLexer->openArguments;
			if (pLexer->openArguments == 0) {
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

			NIKI_LEXER_INPUT_SIZE_TYPE tempIndex = nextTokenPosition+2;

			uint8_t foundCloseReference = 0;
			for (; tempIndex < pLexer->inputSize && !isSpaceNotNewline(pLexer->input[tempIndex]); ++tempIndex) {
				if (pLexer->input[tempIndex] == NIKI_REFERENCE_CLOSE) {
					++tempIndex;
					foundCloseReference = 1;
					break;
				}

				reference = sdscatlen(reference, pLexer->input[tempIndex], 1);
			}

			if (foundCloseReference) {
				nikiReferencesPush(sdslen(result), reference);
				nextTokenPosition = tempIndex;
				continue;
			} else
				sdsfree(reference);
				reference = NULL;

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

		result = sdscatlen(result, pLexer->input[nextTokenPosition++], 1);
	}

	pLexer->token.value = result;
	return nextTokenPosition;
}

void nikiLexerSetTokenType(NikiLexer* pLexer) {
	if (sdslen(pLexer->token.value) != 0 && pLexer->token.value[0] == NIKI_STATEMENT_SEPARATOR) {
		pLexer->token.type = NIKI_TOKEN_EOS;

	} else if (pLexer->token.type == NIKI_TOKEN_NONE || ((NIKI_TOKEN_EOS|NIKI_TOKEN_END) & pLexer->token.type)) { // if the lexer just started and is not EOS
		pLexer->token.type = NIKI_TOKEN_IDENTIFIER;

	} else if ((NIKI_TOKEN_IDENTIFIER|NIKI_TOKEN_ARGUMENT) & pLexer->token.type)
		pLexer->token.type = NIKI_TOKEN_ARGUMENT;
}

void nikiLexerClear(NikiLexer* pLexer) {
	sdsfree(pLexer->input);
	pLexer->input = NULL;

	pLexer->position = 0;

	sdsfree(pLexer->token.value);
	nikiReferencesClear(pLexer->token.references);
	pLexer->token.type = NIKI_TOKEN_NONE;

	pLexer->openArguments = 0;
	pLexer->lineIndex = 0;
}