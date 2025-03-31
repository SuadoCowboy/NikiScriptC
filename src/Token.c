#include "Token.h"

#include "Context.h"

ns::Token::Token() {}
ns::Token::Token(TokenType type) : type(type), value("") {}
ns::Token::Token(TokenType type, const std::string& value) : type(type), value(value) {}

uint8_t operator|(ns::TokenType l, ns::TokenType r) {
	return static_cast<uint8_t>(l)|static_cast<uint8_t>(r);
}

uint8_t operator|(uint8_t l, ns::TokenType r) {
	return l|static_cast<uint8_t>(r);
}

uint8_t operator|(ns::TokenType l, uint8_t r) {
	return static_cast<uint8_t>(l)|r;
}

uint8_t operator&(uint8_t l, ns::TokenType r) {
	return l&static_cast<uint8_t>(r);
}

uint8_t operator&(ns::TokenType l, uint8_t r) {
	return static_cast<uint8_t>(l)&r;
}

void ns::insertReferencesInToken(Context& ctx, Token& token) {
	size_t offset = 0;
	for (auto& reference : token.references) {
		if (ctx.consoleVariables.count(reference.second) != 0) { // console variable
			token.value = token.value.insert(offset+reference.first, ctx.consoleVariables[reference.second]);
			offset += ctx.consoleVariables[reference.second].size();

		} else if (ctx.programVariables.count(reference.second) != 0) { // program variable
			ProgramVariable& var = ctx.programVariables[reference.second];
			std::string value = var.get(ctx, &var);

			token.value = token.value.insert(offset+reference.first, value);
			offset += value.size();
		
		} else {
			token.value = token.value.insert(offset+reference.first, formatString("{}{}{}{}", NIKISCRIPT_REFERENCE, NIKISCRIPT_REFERENCE_OPEN, reference.second, NIKISCRIPT_REFERENCE_CLOSE));
			offset += reference.second.size()+3;
		}
	}
}