
#include <zengine.h>

#define MAX_CONSOLE_COMMAND_TYPES 256

struct ZConsoleCmd
{
	char* name;
	char* description;
	i32 bExternal;
	ZE_ConsoleCallback functionPtr;
};

ze_internal ZConsoleCmd g_commands[MAX_CONSOLE_COMMAND_TYPES];


ze_external zErrorCode ZE_RegisterConsoleCommand(
    char *name, char *description, ZE_ConsoleCallback callback)
{
	return ZERROR_CODE_NONE;	
}

ze_external void ZE_QueueConsoleCommand(char* cmd)
{
    
}
