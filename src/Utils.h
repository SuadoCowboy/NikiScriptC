#pragma once

#include <stdint.h>
#include <string>
#include <sstream>

#include "DLLExport.h"

namespace ns {
	/**
	 * @warning this function is not meant to be used outside this header
	 * @tparam  
	 * @param format 
	 * @param buf 
	 */
	 template<typename ...>
	static void _formatStringValue(const std::string& format, std::stringstream& buf) {
		buf << format;
	}

	/**
	 * @warning this function is not meant to be used outside this header
	 * @tparam T 
	 * @tparam Args 
	 * @param format 
	 * @param buf 
	 * @param value 
	 * @param args 
	 */
	template<typename T, typename ... Args>
	static void _formatStringValue(const std::string& format, std::stringstream& buf, T value, Args& ... args) {
		uint64_t idx = format.find("{}");
		if (idx == std::string::npos) {
			buf << format;
			return;
		}

		buf << format.substr(0, idx) << value;
		
		_formatStringValue(format.substr(idx+2), buf, args...);
	}

	/**
	 * @brief format string using {}. Example: `formatString("Hello, {}!", "World")` -> `"Hello, World!"`
	 * @tparam Args 
	 * @param format 
	 * @param args 
	 * @return std::string 
	 */
	template<typename ... Args>
	std::string formatString(const std::string& format, Args ... args) {
		std::stringstream buf{};
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
	NIKIAPI void ltrim(std::string &s);
	
	/**
	 * @brief trim from end
	 * @param s string to trim the end
	 * @see ns::trim
	 * @see ns::ltrim
	 * @note https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
	 */
	NIKIAPI void rtrim(std::string &s);
	
	/**
	 * @brief trim from start and end
	 * @param s string to trim
	 * @see ns::ltrim
	 * @see ns::rtrim
	 * @note https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
	 */
	NIKIAPI void trim(std::string& s);

	/**
	 * @brief call isspace but does not accept newline
	 * @param c char to check if is space
	 * @return true if is space
	 * @return false if is not space
	 */
	NIKIAPI bool isSpaceNotNewline(char c);
}