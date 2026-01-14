
#include <zengine.h>

ze_internal i32 _argc;
ze_internal char** _argv;

ze_internal void DumpArgv(i32 argc, char** argv)
{
	if (argv == NULL)
	{
		return;
	}
	printf("Args (%d): ", argc);
	for (i32 i = 0; i < argc; ++i)
	{
		printf("%s,", argv[i]);
	}
	printf("\n");
}

ze_external void ZE_CommandLineInit(i32 argc, char** argv)
{
    _argc = argc;
    _argv = argv;
	DumpArgv(argc, argv);
}

ze_external i32 ZE_FindCommandLineIndex(char* shortName, char* longName)
{

    for (i32 i = 0; i < _argc; ++i)
    {
        char* arg = _argv[i];
        i32 result = -1;
        if (shortName != NULL && strcmp(shortName, arg) == 0) { result = i; }
        if (longName != NULL && strcmp(longName, arg) == 0) { result = i; }
        if (result >= 0) { return result; }
        printf("%s does not equal %s or %s\n", arg, shortName, longName);
    }
    return -1;
}


ze_external i32 ZE_GetCommandLineArgCount()
{
    return _argc;
}

ze_external char* ZE_GetCommandLineArg(i32 index)
{
    if (index < 0) { return NULL; }
    if (index >= _argc) { return NULL; }
    return _argv[index];
}
