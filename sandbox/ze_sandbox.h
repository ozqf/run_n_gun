#ifndef ZE_SANDBOX_H
#define ZE_SANDBOX_H

#include <stdio.h>
#include <stdlib.h>

#include <zengine.h>

ze_external void Sandbox_Run(i32 argc, char** argv);

ze_external void PhysicsTest_On();
ze_external void PhysicsTestFrame(ZEFrame frame);

#endif // ZE_SANDBOX_H
