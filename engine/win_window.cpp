
// third party
#include <win_zengine.h>
#include <glad.h>
#include <glfw3.h>

ze_internal i32 g_bConsoleInit = NO;
ze_internal HWND consoleHandle;

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

ze_external void Platform_ErrorBox(char* message)
{
	MessageBoxA(0,
        (LPCSTR)message,
        (LPCSTR) "Error",
        MB_OK | MB_ICONINFORMATION
    );
}

////////////////////////////////////////////////////////
// initialisation
////////////////////////////////////////////////////////
ze_external void Platform_InitConsole()
{
	if (g_bConsoleInit)
	{
		return;
	}
	g_bConsoleInit = TRUE;
	// init live debug console
	FILE *stream;
	AllocConsole();
	freopen_s(&stream, "conin$", "r", stdin);
	freopen_s(&stream, "conout$", "w", stdout);
	freopen_s(&stream, "conout$", "w", stderr);
	consoleHandle = GetConsoleWindow();
	MoveWindow(consoleHandle, 1, 1, 680, 600, 1);
	printf("[%s] Console initialized.\n", __FILE__);
}

ze_external void Platform_SetCursorLock(i32 bLocked)
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

////////////////////////////////////////////////////////
// Input
////////////////////////////////////////////////////////

static i32 Win_GlfwToZEKey(i32 glfwKeyCode)
{
    switch (glfwKeyCode)
    {
        // mouse events handled via separate callbacks
        //case GLFW_KEY_: return Z_INPUT_CODE_NULL;
        //case GLFW_KEY_: return Z_INPUT_CODE_MOUSE_1;
        //case GLFW_KEY_: return Z_INPUT_CODE_MOUSE_2;
        //case GLFW_KEY_: return Z_INPUT_CODE_MOUSE_3;
        //case GLFW_KEY_: return Z_INPUT_CODE_MOUSE_4;
        //case GLFW_KEY_: return Z_INPUT_CODE_MOUSE_5;
        //case GLFW_KEY_: return Z_INPUT_CODE_MWHEELUP;
        //case GLFW_KEY_: return Z_INPUT_CODE_MWHEELDOWN;
        //case GLFW_KEY_: return Z_INPUT_CODE_MOUSE_POS_X;
        //case GLFW_KEY_: return Z_INPUT_CODE_MOUSE_POS_Y;
        //case GLFW_KEY_: return Z_INPUT_CODE_MOUSE_MOVE_X;
        //case GLFW_KEY_: return Z_INPUT_CODE_MOUSE_MOVE_Y;
        case GLFW_KEY_A: return Z_INPUT_CODE_A;
        case GLFW_KEY_B: return Z_INPUT_CODE_B;
        case GLFW_KEY_C: return Z_INPUT_CODE_C;
        case GLFW_KEY_D: return Z_INPUT_CODE_D;
        case GLFW_KEY_E: return Z_INPUT_CODE_E;
        case GLFW_KEY_F: return Z_INPUT_CODE_F;
        case GLFW_KEY_G: return Z_INPUT_CODE_G;
        case GLFW_KEY_H: return Z_INPUT_CODE_H;
        case GLFW_KEY_I: return Z_INPUT_CODE_I;
        case GLFW_KEY_J: return Z_INPUT_CODE_J;
        case GLFW_KEY_K: return Z_INPUT_CODE_K;
        case GLFW_KEY_L: return Z_INPUT_CODE_L;
        case GLFW_KEY_M: return Z_INPUT_CODE_M;
        case GLFW_KEY_N: return Z_INPUT_CODE_N;
        case GLFW_KEY_O: return Z_INPUT_CODE_O;
        case GLFW_KEY_P: return Z_INPUT_CODE_P;
        case GLFW_KEY_Q: return Z_INPUT_CODE_Q;
        case GLFW_KEY_R: return Z_INPUT_CODE_R;
        case GLFW_KEY_S: return Z_INPUT_CODE_S;
        case GLFW_KEY_T: return Z_INPUT_CODE_T;
        case GLFW_KEY_U: return Z_INPUT_CODE_U;
        case GLFW_KEY_V: return Z_INPUT_CODE_V;
        case GLFW_KEY_W: return Z_INPUT_CODE_W;
        case GLFW_KEY_X: return Z_INPUT_CODE_X;
        case GLFW_KEY_Y: return Z_INPUT_CODE_Y;
        case GLFW_KEY_Z: return Z_INPUT_CODE_Z;
        case GLFW_KEY_SPACE: return Z_INPUT_CODE_SPACE;
        case GLFW_KEY_LEFT_SHIFT: return Z_INPUT_CODE_LEFT_SHIFT;
        case GLFW_KEY_RIGHT_SHIFT: return Z_INPUT_CODE_RIGHT_SHIFT;
        case GLFW_KEY_LEFT_CONTROL: return Z_INPUT_CODE_LEFT_CONTROL;
        case GLFW_KEY_RIGHT_CONTROL: return Z_INPUT_CODE_RIGHT_CONTROL;
        case GLFW_KEY_ESCAPE: return Z_INPUT_CODE_ESCAPE;
        case GLFW_KEY_ENTER: return Z_INPUT_CODE_RETURN;
        case GLFW_KEY_KP_ENTER: return Z_INPUT_CODE_ENTER;
        case GLFW_KEY_0: return Z_INPUT_CODE_0;
        case GLFW_KEY_1: return Z_INPUT_CODE_1;
        case GLFW_KEY_2: return Z_INPUT_CODE_2;
        case GLFW_KEY_3: return Z_INPUT_CODE_3;
        case GLFW_KEY_4: return Z_INPUT_CODE_4;
        case GLFW_KEY_5: return Z_INPUT_CODE_5;
        case GLFW_KEY_6: return Z_INPUT_CODE_6;
        case GLFW_KEY_7: return Z_INPUT_CODE_7;
        case GLFW_KEY_8: return Z_INPUT_CODE_8;
        case GLFW_KEY_9: return Z_INPUT_CODE_9;
        case GLFW_KEY_UP: return Z_INPUT_CODE_UP;
        case GLFW_KEY_DOWN: return Z_INPUT_CODE_DOWN;
        case GLFW_KEY_LEFT: return Z_INPUT_CODE_LEFT;
        case GLFW_KEY_RIGHT: return Z_INPUT_CODE_RIGHT;
        case GLFW_KEY_F1: return Z_INPUT_CODE_F1;
        case GLFW_KEY_F2: return Z_INPUT_CODE_F2;
        case GLFW_KEY_F3: return Z_INPUT_CODE_F3;
        case GLFW_KEY_F4: return Z_INPUT_CODE_F4;
        case GLFW_KEY_F5: return Z_INPUT_CODE_F5;
        case GLFW_KEY_F6: return Z_INPUT_CODE_F6;
        case GLFW_KEY_F7: return Z_INPUT_CODE_F7;
        case GLFW_KEY_F8: return Z_INPUT_CODE_F8;
        case GLFW_KEY_F9: return Z_INPUT_CODE_F9;
        case GLFW_KEY_F10: return Z_INPUT_CODE_F10;
        case GLFW_KEY_F11: return Z_INPUT_CODE_F11;
        case GLFW_KEY_F12: return Z_INPUT_CODE_F12;
        case GLFW_KEY_DELETE: return Z_INPUT_CODE_DELETE;
		case 162: return Z_INPUT_CODE_BACKSLASH;
		case GLFW_KEY_BACKSLASH: return Z_INPUT_CODE_BACKSLASH;
        default:
        printf("Found no match for GLFW Key %d\n", glfwKeyCode);
        return Z_INPUT_CODE_NULL;
    }
}

static i32 handle_engine_key_event(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    i32 bEscapeOverridden = ZEGetApp().flags & APP_FLAG_OVERRIDE_ESCAPE_KEY;
    if (key == GLFW_KEY_F8 || (!bEscapeOverridden && key == GLFW_KEY_ESCAPE))
    {
        Platform_Shutdown();
    }
    return 0;
}

static void key_callback(GLFWwindow *window, int glfwKey, int scancode, int action, int mods)
{
    // check for internal debug input handling
    if (handle_engine_key_event(window, glfwKey, scancode, action, mods) == YES)
    {
        return;
    }
    
    if (ZEGetApp().keyCallback != NULL)
    {
        i32 zeKey = Win_GlfwToZEKey(glfwKey);
        ZEGetApp().keyCallback(zeKey, action, mods);
    }
    else
    {
        printf("Unhandled key %d, scancode %d, action %d, mods %d\n",
            glfwKey, scancode, action, mods);
    }
}

static void window_close_callback(GLFWwindow *window)
{
    Platform_Shutdown();
}

static void mouse_position_callback(
	GLFWwindow* window,
	double posX,
	double posY)
{

}

static void mouse_button_callback(
	GLFWwindow* window,
	int button,
	int action,
	int mods)
{
    
}

ze_external void Platform_PollEvents()
{
    glfwPollEvents();
}

////////////////////////////////////////////////////////
// DISPLAY
////////////////////////////////////////////////////////

extern "C" void Platform_SwapBuffers()
{    
    glfwSwapBuffers(g_window);
}

extern "C" zErrorCode Platform_CreateWindow(const char* windowName)
{
    printf("Spawn window\n");
    Win_InitTimer();
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
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        scrWidth = g_pendingWidth;
        scrHeight = g_pendingHeight;
    }
    else
    {
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    }
    
	// Hide the window if 'special mode' is one shot mode
	if (ZEGetSpecialMode() == SPECIAL_APP_MODE_SINGLE_SHOT)
	{
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	}

    // record screen size
    g_windowSize[0] = scrWidth;
    g_windowSize[1] = scrHeight;
    g_windowAspect = (f32)scrWidth / (f32)scrHeight;
    printf("Aspect ratio %.3f\n", g_windowAspect);

    const char* name = "Zealous Engine";
    if (windowName != NULL)
    {
        name = windowName;
    }
    g_window = glfwCreateWindow(scrWidth, scrHeight, name, NULL, NULL);
    
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

    printf("Init window callbacks\n");
    glfwSetWindowCloseCallback(g_window, window_close_callback);
    glfwSetKeyCallback(g_window, key_callback);
    glfwSetMouseButtonCallback(g_window, mouse_button_callback);
    glfwSetCursorPosCallback(g_window, mouse_position_callback);

    // enable raw mouse if available
    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(g_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
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
