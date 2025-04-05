#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <locale>

void ns::ltrim(sds s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !isspace(ch);
	}));
}

void ns::rtrim(sds s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !isspace(ch);
	}).base(), s.end());
}

void ns::trim(sds s) {
	ltrim(s);
	rtrim(s);
}

uint8_t ns::isSpaceNotNewline(char c) {
	return isspace(c) && c != '\n';
}