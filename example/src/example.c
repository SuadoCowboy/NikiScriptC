#include <iostream>
#include <string>
#include <stdint.h>
#include <sstream>

#include <NikiScript.h>
#include <PrintCallback.h>
#include <Context.h>
#include <Token.h>
#include <Lexer.h>
#include <Parser.h>

void nikiScriptPrintCallback(void*, ns::PrintLevel level, const std::string& msg) {
	std::cout << ns::printLevelToString(level) << ": " << msg;
}

std::string tokenTypeToString(const ns::TokenType& type) {
	switch (type) {
	case ns::TokenType::NONE:
		return "NONE";
	case ns::TokenType::IDENTIFIER:
		return "IDENTIFIER";
	case ns::TokenType::ARGUMENT:
		return "ARGUMENT";
	case ns::TokenType::EOS:
		return "EOS";
	case ns::TokenType::END:
		return "END";
	default:
		return "UNKNOWN";
	}
}

std::string tokenToString(const ns::Token& token) {
	std::stringstream out{};
	out << '(' << tokenTypeToString(token.type) << ", \"" << token.value << "\", REFS: {";

	std::string formatted = token.value;
	if (token.references.empty())
		return out.str()+"})";

	else for (auto& ref : token.references) {
		out << '(' << ref.first << ", " << ref.second << "), ";
		formatted.insert(ref.first, ref.second);
	}

	std::string outString = out.str();
	outString.replace(outString.begin()+outString.size()-2, outString.begin()+outString.size()-1, "}) -> ");

	return outString + formatted;
}


static void test_command(ns::Context& ctx) {
	ns::Context copy = ns::copyContext(ctx);
	ns::Lexer lexer{ctx.args.getString(0)};
	copy.pLexer = &lexer;
	copy.args.arguments.clear();

	ns::parse(copy);
}

bool running = false;
static void quit_command(ns::Context&) {
	running = false;
}

int main(int, char**) {
	ns::setPrintCallback(nullptr, nikiScriptPrintCallback);

	ns::Context ctx;
	ns::registerCommands(ctx);

	ns::Lexer lexer;
	ctx.pLexer = &lexer;

	ctx.commands.add(ns::Command("quit", 0,1, quit_command, "stops the main loop from running", {"s[?]", ""}));
	ctx.commands.add(ns::Command("test", 1,1, test_command, "runs script", {"s[script]", "parses to nikiscript"}));

	// decimal numbers
	float floatNumber = 0;
	ns::registerVariable(ctx, "float", "", &floatNumber, ns::getNumber<float>, ns::setFloat);

	double doubleNumber = 0;
	ns::registerVariable(ctx, "double", "", &doubleNumber, ns::getNumber<double>, ns::setDouble);

	long double lDoubleNumber = 0;
	ns::registerVariable(ctx, "ldouble", "", &lDoubleNumber, ns::getNumber<long double>, ns::setLongDouble);

	// signed numbers
	int8_t int8 = 0;
	ns::registerVariable(ctx, "int8", "", &int8, ns::getNumber<int8_t>, ns::setChar);

	int16_t int16 = 0;
	ns::registerVariable(ctx, "int16", "", &int16, ns::getNumber<int16_t>, ns::setShort);

	int32_t int32 = 0;
	ns::registerVariable(ctx, "int32", "", &int32, ns::getNumber<int32_t>, ns::setInteger);

	int64_t int64 = 0;
	ns::registerVariable(ctx, "int64", "", &int64, ns::getNumber<int64_t>, ns::setLong);

	long long int128 = 0;
	ns::registerVariable(ctx, "int128", "", &int128, ns::getNumber<long long>, ns::setLongLong);

	// unsigned numbers
	uint8_t uint8 = 0;
	ns::registerVariable(ctx, "uint8", "", &uint8, ns::getNumber<uint8_t>, ns::setUnsigned<uint8_t>);

	uint16_t uint16 = 0;
	ns::registerVariable(ctx, "uint16", "", &uint16, ns::getNumber<uint16_t>, ns::setUnsigned<uint16_t>);

	uint32_t uint32 = 0;
	ns::registerVariable(ctx, "uint32", "", &uint32, ns::getNumber<uint32_t>, ns::setUnsigned<uint32_t>);

	uint64_t uint64 = 0;
	ns::registerVariable(ctx, "uint64", "", &uint64, ns::getNumber<uint64_t>, ns::setUnsigned<uint64_t>);

	long long uint128 = 0;
	ns::registerVariable(ctx, "uint128", "", &uint128, ns::getNumber<long long>, ns::setUnsignedLongLong);

	running = true;
	while (running) {
		std::string input;

		std::cout << "> ";
		std::getline(std::cin, input);

		lexer.input = input;
		ns::parse(ctx);
		lexer.clear();

		ns::updateLoopVariables(ctx);
	}
}