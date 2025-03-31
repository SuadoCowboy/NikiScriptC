#include "ProgramVariable.h"

#include "Context.h"
#include "Utils.h"

ns::ProgramVariable::ProgramVariable() {}

ns::ProgramVariable::ProgramVariable(void* pVariable, const std::string_view& description, const GetProgramVariableValue& get, const SetProgramVariableValue& set)
 : pValue(pVariable), description(description), get(get), set(set) {}

std::string ns::getString(Context&, ProgramVariable* pVar) {
	return *static_cast<std::string*>(pVar->pValue);
}

void ns::setString(Context&, ProgramVariable* pVar, const std::string& str) {
	*static_cast<std::string*>(pVar->pValue) = str;
}


void ns::setUnsignedLongLong(Context&, ProgramVariable* pVar, const std::string& str) {
	try {
		*static_cast<unsigned long long*>(pVar->pValue) = std::stoull(str);
	} catch (...) {}
}


void ns::setFloat(Context&, ProgramVariable* pVar, const std::string& str) {
	try {
		*static_cast<float*>(pVar->pValue) = std::stof(str);
	} catch (...) {}
}

void ns::setDouble(Context&, ProgramVariable* pVar, const std::string& str) {
	try {
		*static_cast<double*>(pVar->pValue) = std::stod(str);
	} catch (...) {}
}

void ns::setLongDouble(Context&, ProgramVariable* pVar, const std::string& str) {
	try {
		*static_cast<long double*>(pVar->pValue) = std::stold(str);
	} catch (...) {}
}


void ns::setChar(Context&, ProgramVariable* pVar, const std::string& str) {
	try {
		*static_cast<char*>(pVar->pValue) = (char)std::stoi(str);
	} catch (...) {}
}

void ns::setShort(Context&, ProgramVariable* pVar, const std::string& str) {
	try {
		*static_cast<short*>(pVar->pValue) = (short)std::stoi(str);
	} catch (...) {}
}

void ns::setInteger(Context&, ProgramVariable* pVar, const std::string& str) {
	try {
		*static_cast<int*>(pVar->pValue) = std::stoi(str);
	} catch (...) {}
}

void ns::setLong(Context&, ProgramVariable* pVar, const std::string& str) {
	try {
		*static_cast<long*>(pVar->pValue) = std::stol(str);
	} catch (...) {}
}

void ns::setLongLong(Context&, ProgramVariable* pVar, const std::string& str) {
	try {
		*static_cast<long long*>(pVar->pValue) = std::stoll(str);
	} catch (...) {}
}

#if BUILD_SHARED == 1 && BUILD_EXPORT == 1
template<typename T>
void ns::setUnsigned(Context&, ProgramVariable* pVar, const std::string& str) {
	try {
		*static_cast<T*>(pVar->pValue) = (T)std::stoul(str);
	} catch (...) {}
}

template void ns::setUnsigned<uint8_t>(Context&, ProgramVariable*, const std::string&);
template void ns::setUnsigned<uint16_t>(Context&, ProgramVariable*, const std::string&);
template void ns::setUnsigned<uint32_t>(Context&, ProgramVariable*, const std::string&);
template void ns::setUnsigned<uint64_t>(Context&, ProgramVariable*, const std::string&);
#endif