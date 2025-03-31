#pragma once

#include <vector>
#include <string_view>
#include <string>

#include "DLLExport.h"

namespace ns {
	struct Context;

	typedef void(*CommandCallback)(Context& ctx);

	struct NIKIAPI Command {
		std::string_view name;
		unsigned char minArgs = 0, maxArgs = 0;
		CommandCallback callback = nullptr;
		std::string_view description;

		/**
		 * @note odd = name
		 * @note even = description
		 */
		std::vector<std::string_view> argsDescriptions{};

		Command();
		/**
		 * @param name
		 * @param minArgs
		 * @param maxArgs
		 * @param callback
		 * @param description Command description
		 * @param argsDescriptions Arguments description. Should have 2 strings for each argument, where the first one is argument name and the second is argument description.
		 * @see ns::registerCommands for code example
		 */
		Command(const std::string_view& name, unsigned char minArgs, unsigned char maxArgs, CommandCallback callback, const std::string_view& description, const std::vector<std::string_view>& argsDescriptions);
	
		std::string getArgumentsNames();

		/**
		 * @brief prints usage, description and argsDescriptions all like a data tree
		 */
		void printAsDataTree();
	};
}