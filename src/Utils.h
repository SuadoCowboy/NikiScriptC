#pragma once

#include <stdint.h>

#include <sds.h>

#include "DLLExport.h"

/**
 * @brief 
 * @tparam  
 * @param format 
 * @param buf 
 */
// template<typename ...>
// static void _formatStringValue(const sds format, sdsstream& buf) {
// 	buf << format;
// }

/**
 * @warning this function is not meant to be used outside this header
 * @tparam T 
 * @tparam Args 
 * @param format 
 * @param buf 
 * @param value 
 * @param args 
 */
// template<typename T, typename ... Args>
// static void _formatStringValue(const sds format, sdsstream& buf, T value, Args& ... args) {
// 	uint64_t idx = format.find("{}");
// 	if (idx == sds::npos) {
// 		buf << format;
// 		return;
// 	}

// 	buf << format.substr(0, idx) << value;
	
// 	_formatStringValue(format.substr(idx+2), buf, args...);
// }

/**
 * @brief format string using {}. Example: `formatString("Hello, {}!", "World")` -> `"Hello, World!"`
 * @tparam Args
 * @param format
 * @param args
 * @return sds
 */
template<typename ... Args>
sds formatString(const sds format, Args ... args) {
	sdsstream buf{};
	_formatStringValue(format, buf, args...);
	return buf.str();
}

/**
	 * @brief trim from start
	 * @param s string to trim the beginning
	 * @see ns::trim
	 * @see ns::rtrim
	 * @note https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
	 */
NIKIAPI void ltrim(sds &s);

/**
 * @brief trim from end
 * @param s string to trim the end
 * @see ns::trim
 * @see ns::ltrim
 * @note https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
 */
NIKIAPI void rtrim(sds &s);

/**
 * @brief trim from start and end
 * @param s string to trim
 * @see ns::ltrim
 * @see ns::rtrim
 * @note https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
 */
NIKIAPI void trim(sds s);

/**
 * @brief call isspace but does not accept newline
 * @param c char to check if is space
 * @return 1 if is space
 * @return 0 if is not space
 */
NIKIAPI uint8_t isSpaceNotNewline(char c);