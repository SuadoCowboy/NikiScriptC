#pragma once

#include <stdint.h>

#include <sds.h>

#include "DLLExport.h"
#include "Command.h"

struct NikiContext;

#ifndef NIKI_COMMANDS_SIZE_TYPE
#define NIKI_COMMANDS_SIZE_TYPE uint32_t
#endif

// TODO: FASTEST search case for commands(maybe hash map but research is better to be sure)
typedef struct {
	NikiCommand* pCommands;
	NIKI_COMMANDS_SIZE_TYPE size;
} NikiCommands;

NikiCommand* nikiGetCommand(const NikiCommands* pCommands, sds name);

/**
 * @brief adds command to commands unordered_map
 * @param command name of the will be mapped in the commands variable
 * @return true if command name is okay
 * @return 0 if command with this name already exists
 */
uint8_t nikiAddCommand(NikiCommands* pCommands, const NikiCommand* pCommand);

void nikiRemoveCommand(NikiCommands* pCommands, const NikiCommand* pCommand);