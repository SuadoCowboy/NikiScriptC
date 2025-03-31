#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <locale>

void ns::ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !isspace(ch);
	}));
}

void ns::rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !isspace(ch);
	}).base(), s.end());
}

void ns::trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

bool ns::isSpaceNotNewline(char c) {
	return isspace(c) && c != '\n';
}