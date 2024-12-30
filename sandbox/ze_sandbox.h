#ifndef ZE_SANDBOX_H
#define ZE_SANDBOX_H

#include <zengine.h>

static void Sandbox_Run()
{
    printf("Zealous Engine Sandbox\n");
    ZAssets_AllocTex(64, 64, "foo.png");
}

#endif // ZE_SANDBOX_H