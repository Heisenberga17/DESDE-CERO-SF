/*
 * Web (Emscripten) entry point for Star Fox 64.
 *
 * This file wraps the original main.cpp without modifying it.
 * It replaces the blocking while-loop with emscripten_set_main_loop
 * and adjusts asset paths for the Emscripten virtual filesystem.
 *
 * Macro redirects used (all undone after include):
 *  - checkVersion        → no-op (WebGL version string breaks sscanf parser)
 *  - glfwJoystickPresent → browser Gamepad API check
 *  - glfwJoystickIsGamepad → browser Gamepad API check (standard mapping)
 *  - glfwGetGamepadState → browser Gamepad API read
 *  - glfwGetGamepadName  → browser Gamepad API id string
 */
#include <emscripten.h>

// --- Macro redirects (applied before including main.cpp) ---

// WebGL returns "OpenGL ES 3.0 (WebGL 2.0)" for GL_VERSION,
// which GLSL::checkVersion() can't parse (sscanf fails), causing exit(0).
namespace GLSL { inline void checkVersion_web() {} }
#define checkVersion checkVersion_web

// Emscripten's GLFW port doesn't implement the gamepad API.
// Redirect all gamepad calls to our browser-API-backed implementations.
#define glfwJoystickPresent  web_glfwJoystickPresent
#define glfwJoystickIsGamepad web_glfwJoystickIsGamepad
#define glfwGetGamepadState  web_glfwGetGamepadState
#define glfwGetGamepadName   web_glfwGetGamepadName

// Rename original main() so we can provide our own
#define main original_main
#include "../src/main.cpp"
#undef main
#undef checkVersion
#undef glfwJoystickPresent
#undef glfwJoystickIsGamepad
#undef glfwGetGamepadState
#undef glfwGetGamepadName

// --- Browser Gamepad API bridge via EM_JS ---

EM_JS(int, js_joystickPresent, (int jid), {
    var gamepads = navigator.getGamepads ? navigator.getGamepads() : [];
    var gp = gamepads[jid];
    return (gp && gp.connected) ? 1 : 0;
});

EM_JS(int, js_joystickIsGamepad, (int jid), {
    var gamepads = navigator.getGamepads ? navigator.getGamepads() : [];
    var gp = gamepads[jid];
    return (gp && gp.connected) ? 1 : 0;
});

EM_JS(int, js_getGamepadState, (int jid, unsigned char* buttons, float* axes), {
    var gamepads = navigator.getGamepads ? navigator.getGamepads() : [];
    var gp = gamepads[jid];
    if (!gp || !gp.connected) return 0;

    // Standard gamepad mapping matches GLFW button order for the first 15.
    var nb = Math.min(gp.buttons.length, 15);
    for (var i = 0; i < nb; i++) {
        HEAPU8[buttons + i] = gp.buttons[i].pressed ? 1 : 0;
    }
    for (var i = nb; i < 15; i++) {
        HEAPU8[buttons + i] = 0;
    }

    // Axes 0-3: left stick X/Y, right stick X/Y (1:1 mapping).
    var na = Math.min(gp.axes.length, 4);
    for (var i = 0; i < na; i++) {
        HEAPF32[(axes >> 2) + i] = gp.axes[i];
    }
    for (var i = na; i < 4; i++) {
        HEAPF32[(axes >> 2) + i] = 0.0;
    }

    // GLFW axes 4/5 = left/right trigger.
    // Browser standard mapping: buttons[6]/[7] are triggers with analog value.
    // GLFW trigger range: -1.0 (released) to +1.0 (fully pressed).
    HEAPF32[(axes >> 2) + 4] = gp.buttons.length > 6
        ? (gp.buttons[6].value * 2.0 - 1.0) : -1.0;
    HEAPF32[(axes >> 2) + 5] = gp.buttons.length > 7
        ? (gp.buttons[7].value * 2.0 - 1.0) : -1.0;

    return 1;
});

EM_JS(void, js_getGamepadName, (int jid, char* buf, int bufLen), {
    var gamepads = navigator.getGamepads ? navigator.getGamepads() : [];
    var gp = gamepads[jid];
    var name = (gp && gp.id) ? gp.id : "";
    stringToUTF8(name, buf, bufLen);
});

// --- Implementations of the redirected GLFW functions ---

static char s_gamepadName[128] = "";

extern "C" {

int web_glfwJoystickPresent(int jid) {
    return js_joystickPresent(jid);
}

int web_glfwJoystickIsGamepad(int jid) {
    return js_joystickIsGamepad(jid);
}

int web_glfwGetGamepadState(int jid, GLFWgamepadstate* state) {
    return js_getGamepadState(jid, state->buttons, state->axes);
}

const char* web_glfwGetGamepadName(int jid) {
    js_getGamepadName(jid, s_gamepadName, sizeof(s_gamepadName));
    return s_gamepadName;
}

} // extern "C"

// --- Main loop and entry point ---

static Application* g_application = nullptr;
static WindowManager* g_windowManager = nullptr;

static double g_lastTime = 0.0;
static double g_accumulator = 0.0;
static const double FIXED_DT = 1.0 / 60.0;
static const double MAX_FRAME_TIME = 0.25;

void main_loop() {
    double now = glfwGetTime();
    double frameTime = now - g_lastTime;
    g_lastTime = now;

    if (frameTime > MAX_FRAME_TIME) frameTime = MAX_FRAME_TIME;

    g_accumulator += frameTime;

    while (g_accumulator >= FIXED_DT) {
        g_application->render();
        g_accumulator -= FIXED_DT;
    }

    glfwSwapBuffers(g_windowManager->getHandle());
    glfwPollEvents();
}

int main(int argc, char** argv) {
    // Emscripten virtual filesystem paths (set by --preload-file)
    std::string shaderDir = "/shaders";
    std::string resourceDir = "/resources";

    g_application = new Application();

    g_windowManager = new WindowManager();
    g_windowManager->init(1024, 768);
    g_windowManager->setEventCallbacks(g_application);
    g_application->windowManager = g_windowManager;

    g_application->initProg(shaderDir);
    g_application->initTex(resourceDir);
    g_application->initGeom(resourceDir);
    g_application->initGame();

    srand(static_cast<unsigned>(time(0)));

    g_lastTime = glfwGetTime();

    // 0 = use requestAnimationFrame, 1 = simulate infinite loop
    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}
