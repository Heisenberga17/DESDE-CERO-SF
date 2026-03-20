/*
 * Web (Emscripten) entry point for Star Fox 64.
 *
 * This file wraps the original main.cpp without modifying it.
 * It replaces the blocking while-loop with emscripten_set_main_loop
 * and adjusts asset paths for the Emscripten virtual filesystem.
 */
#include <emscripten.h>

// Rename original main() so we can provide our own
#define main original_main
#include "../src/main.cpp"
#undef main

// Emscripten's GLFW port does not implement the gamepad API.
// Provide no-op stubs so the code links (gamepad just won't work in browser).
extern "C" {
    int glfwGetGamepadState(int jid, GLFWgamepadstate* state) {
        (void)jid; (void)state;
        return GLFW_FALSE;
    }
    const char* glfwGetGamepadName(int jid) {
        (void)jid;
        return "";
    }
}

// Global pointers for the main loop callback
static Application* g_application = nullptr;
static WindowManager* g_windowManager = nullptr;

void main_loop() {
    g_application->render();
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

    // 0 = use requestAnimationFrame, 1 = simulate infinite loop
    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}
