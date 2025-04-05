#include "ProgramVariable.h"

#include <stdlib.h>

#include "Context.h"
#include "Utils.h"

ns::ProgramVariable::ProgramVariable() {}

ns::ProgramVariable::ProgramVariable(void* pVariable, const sds_view& description, const GetProgramVariableValue& get, const SetProgramVariableValue& set)
 : pValue(pVariable), description(description), get(get), set(set) {}

sds ns::getString(NikiContext*, ProgramVariable* pVar) {
	return *static_cast<sds*>(pVar->pValue);
}

void ns::setString(NikiContext*, ProgramVariable* pVar, const sds str) {
	*static_cast<sds*>(pVar->pValue) = str;
}


void ns::setUnsignedLongLong(NikiContext*, ProgramVariable* pVar, const sds str) {
	try {
		*static_cast<unsigned long long*>(pVar->pValue) = std::stoull(str);
	} catch (...) {}
}


void ns::setFloat(NikiContext*, ProgramVariable* pVar, const sds str) {
	try {
		*(float*)(pVar->pValue) = atof(str);
	} catch (...) {}
}

void ns::setDouble(NikiContext*, ProgramVariable* pVar, const sds str) {
	try {
		*static_cast<double*>(pVar->pValue) = std::stod(str);
	} catch (...) {}
}

void ns::setLongDouble(NikiContext*, ProgramVariable* pVar, const sds str) {
	try {
		*static_cast<long double*>(pVar->pValue) = std::stold(str);
	} catch (...) {}
}


void ns::setChar(NikiContext*, ProgramVariable* pVar, const sds str) {
	try {
		*static_cast<char*>(pVar->pValue) = (char)std::stoi(str);
	} catch (...) {}
}

void ns::setShort(NikiContext*, ProgramVariable* pVar, const sds str) {
	try {
		*static_cast<short*>(pVar->pValue) = (short)std::stoi(str);
	} catch (...) {}
}

void ns::setInteger(NikiContext*, ProgramVariable* pVar, const sds str) {
	try {
		*static_cast<int*>(pVar->pValue) = std::stoi(str);
	} catch (...) {}
}

void ns::setLong(NikiContext*, ProgramVariable* pVar, const sds str) {
	try {
		*static_cast<long*>(pVar->pValue) = std::stol(str);
	} catch (...) {}
}

void ns::setLongLong(NikiContext*, ProgramVariable* pVar, const sds str) {
	try {
		*static_cast<long long*>(pVar->pValue) = std::stoll(str);
	} catch (...) {}
}

#if BUILD_SHARED == 1 && BUILD_EXPORT == 1
template<typename T>
void ns::setUnsigned(NikiContext*, ProgramVariable* pVar, const sds str) {
	try {
		*static_cast<T*>(pVar->pValue) = (T)std::stoul(str);
	} catch (...) {}
}

template void ns::setUnsigned<uint8_t>(NikiContext*, ProgramVariable*, const sds);
template void ns::setUnsigned<uint16_t>(NikiContext*, ProgramVariable*, const sds);
template void ns::setUnsigned<uint32_t>(NikiContext*, ProgramVariable*, const sds);
template void ns::setUnsigned<uint64_t>(NikiContext*, ProgramVariable*, const sds);
#endif