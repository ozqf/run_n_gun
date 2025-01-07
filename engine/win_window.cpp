
// third party
#include <win_zengine.h>
#include <glad.h>
#include <glfw3.h>

// Version of Opengl that will be requested
const i32 MAJOR_VERSION_REQ = 3;//4;
const i32 MINOR_VERSION_REQ = 3;

internal GLFWwindow *g_window;

internal i32 g_bWindowed = YES;
internal i32 g_bCursorLocked = NO;

// dev sizes - 1366x768, 1024x576
internal i32 g_pendingWidth = 1024;
internal i32 g_pendingHeight = 576;

internal i32 g_monitorSize[2];
internal f32 g_monitorAspect;

internal i32 g_windowSize[2];
internal f32 g_windowAspect;

// performance counts per second
// Read Quad Part for a complete 64 bit integer
static LARGE_INTEGER g_timerFrequency;
// used to calculate time since application startup.
// NEVER change once set!
static i64 g_clockBase;

// TODO: App thread can cause a 'GLFW is not initialised' error here
// by call into main thread during shutdown. Make sure App has closed before
// closing GLFW
static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
	//Platform_Fatal(description);
}

extern "C" void Platform_SetCursorLock(i32 bLocked)
{
    g_bCursorLocked = bLocked;
    if (g_window == NULL)
    {
        return;
    }
	if (g_bCursorLocked)
    {
		printf("Window - Set cursor disabled\n"); 
        glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
		printf("Window - Set cursor enabled\n");
        glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

ze_external ZEWindowInfo Platform_GetWindowInfo()
{
    ZEWindowInfo info;
    info.width = g_windowSize[0];
    info.height = g_windowSize[1];
    info.aspectRatio = g_windowAspect;
    
    info.scrWidth = g_monitorSize[0];
    info.scrHeight = g_monitorSize[1];
    info.scrAspectRatio = g_monitorAspect;
    return info;
}

/*
internal zErrorCode InitRenderer()
{
    // v-sync - off
    // want to cap frame rate manually... v-sync gives me jip.
    // const i32 swapInterval = 1;
    const i32 swapInterval = 0;
    printf("Swap interval - %d\n", swapInterval);
    glfwSwapInterval(swapInterval);
    
    // Depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    return ZERROR_CODE_NONE;
}
*/

////////////////////////////////////////////////////////
// Timing
////////////////////////////////////////////////////////
internal void Win_InitTimer()
{
	// Counts per second of performance frequency
	// eg 2742188
	QueryPerformanceFrequency(&g_timerFrequency);
	printf("Precision timer freq %lld\n", g_timerFrequency.QuadPart);
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	g_clockBase = counter.QuadPart;
}

ze_external f64 Platform_QueryClock()
{
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
	i64 ticksSinceStart = counter.QuadPart - g_clockBase;
	f64 secondsSinceStart =
		(f64)((f64)ticksSinceStart / (f64)g_timerFrequency.QuadPart);
	//printf("%f seconds since startup\n", secondsSinceStart);
	return secondsSinceStart;
}

ze_external void Platform_Sleep(i32 milliseconds)
{
	Sleep(milliseconds);
}

extern "C" void Platform_SwapBuffers()
{    
    glfwSwapBuffers(g_window);
}

extern "C" zErrorCode Platform_CreateWindow()
{
    Win_InitTimer();
    printf("Spawn window\n");
    glfwSetErrorCallback(glfw_error_callback);

    // Check glfw is okay
    if (!glfwInit())
    {
        printf("GLFW init failed - %s - %d\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    // global window settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_VERSION_REQ);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_VERSION_REQ);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    i32 scrWidth = mode->width;
    i32 scrHeight = mode->height;

    // set monitor size - necessary to get the ratio between
    // full screen size and window size for mouse input
    g_monitorSize[0] = scrWidth;
    g_monitorSize[1] = scrHeight;
    g_monitorAspect = (f32)scrWidth / (f32)scrHeight;

    if (g_bWindowed == YES)
    {
        // Resolution locked window
        // Disable decoration and set window size to desktop size
        // to have borderless fullscreen
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        scrWidth = g_pendingWidth;
        scrHeight = g_pendingHeight;
    }
    else
    {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    }
    // record screen size
    g_windowSize[0] = scrWidth;
    g_windowSize[1] = scrHeight;
    g_windowAspect = (f32)scrWidth / (f32)scrHeight;
    printf("Aspect ratio %.3f\n", g_windowAspect);

    g_window = glfwCreateWindow(scrWidth, scrHeight, "Zealous Engine", NULL, NULL);
	printf("Created window\n");

    i32 monitorX, monitorY;
    glfwGetMonitorPos(monitor, &monitorX, &monitorY);
    i32 x = monitorX;
    i32 y = monitorY;
    if (g_bWindowed)
    {
        x += 100;
        y += 100;
    }
    glfwSetWindowPos(g_window, x, y);

    if (!g_window)
    {
        printf("g_window is null - %s - %d\n", __FILE__, __LINE__);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // opengl shizzle now
    glfwMakeContextCurrent(g_window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    
    //////////////////////////////////////////////////////////////////
    // Opengl enabled from here on
    //////////////////////////////////////////////////////////////////
    i32 majorVer;
    i32 minorVer;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVer);
    glGetIntegerv(GL_MINOR_VERSION, &minorVer);
    printf("Requested Opengl ver %d.%d\n", MAJOR_VERSION_REQ, MINOR_VERSION_REQ);
    printf("Opengl ver read as %d.%d\n", majorVer, minorVer);
	
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, scrWidth, scrHeight);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers(g_window);

    return ZERROR_CODE_NONE;
}
