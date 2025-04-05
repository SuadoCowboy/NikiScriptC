#include "ProgramVariable.h"

#include <stdlib.h>

#include "Context.h"
#include "Utils.h"

sds nikiProgramVariableGetString(NikiContext*, NikiProgramVariable* pVar) {
	return *(sds*)(pVar->pValue);
}

void nikiProgramVariableSetString(NikiContext*, NikiProgramVariable* pVar, const sds str) {
	*(sds*)(pVar->pValue) = str;
}


void nikiProgramVariableSetUnsignedLongLong(NikiContext*, NikiProgramVariable* pVar, const sds str) {
	try {
		*static_cast<unsigned long long*>(pVar->pValue) = std::stoull(str);
	} catch (...) {}
}


void nikiProgramVariableSetFloat(NikiContext*, NikiProgramVariable* pVar, const sds str) {
	try {
		*(float*)(pVar->pValue) = atof(str);
	} catch (...) {}
}

void nikiProgramVariableSetDouble(NikiContext*, NikiProgramVariable* pVar, const sds str) {
	try {
		*static_cast<double*>(pVar->pValue) = std::stod(str);
	} catch (...) {}
}

void nikiProgramVariableSetLongDouble(NikiContext*, NikiProgramVariable* pVar, const sds str) {
	try {
		*static_cast<long double*>(pVar->pValue) = std::stold(str);
	} catch (...) {}
}


void nikiProgramVariableSetChar(NikiContext*, NikiProgramVariable* pVar, const sds str) {
	try {
		*static_cast<char*>(pVar->pValue) = (char)std::stoi(str);
	} catch (...) {}
}

void nikiProgramVariableSetShort(NikiContext*, NikiProgramVariable* pVar, const sds str) {
	try {
		*static_cast<short*>(pVar->pValue) = (short)std::stoi(str);
	} catch (...) {}
}

void nikiProgramVariableSetInteger(NikiContext*, NikiProgramVariable* pVar, const sds str) {
	try {
		*static_cast<int*>(pVar->pValue) = std::stoi(str);
	} catch (...) {}
}

void nikiProgramVariableSetLong(NikiContext*, NikiProgramVariable* pVar, const sds str) {
	try {
		*static_cast<long*>(pVar->pValue) = std::stol(str);
	} catch (...) {}
}

void nikiProgramVariableSetLongLong(NikiContext*, NikiProgramVariable* pVar, const sds str) {
	try {
		*static_cast<long long*>(pVar->pValue) = std::stoll(str);
	} catch (...) {}
}

#if BUILD_SHARED == 1 && BUILD_EXPORT == 1
template<typename T>
void nikiProgramVariableSetUnsigned(NikiContext*, NikiProgramVariable* pVar, const sds str) {
	try {
		*static_cast<T*>(pVar->pValue) = (T)std::stoul(str);
	} catch (...) {}
}

template void nikiProgramVariableSetUnsigned<uint8_t>(NikiContext*, NikiProgramVariable*, const sds);
template void nikiProgramVariableSetUnsigned<uint16_t>(NikiContext*, NikiProgramVariable*, const sds);
template void nikiProgramVariableSetUnsigned<uint32_t>(NikiContext*, NikiProgramVariable*, const sds);
template void nikiProgramVariableSetUnsigned<uint64_t>(NikiContext*, NikiProgramVariable*, const sds);
#endif