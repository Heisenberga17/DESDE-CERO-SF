/*
 * GLAD shim for Emscripten/WebGL builds.
 * Redirects to GLES3 headers and provides a no-op gladLoadGL().
 */
#ifndef GLAD_H
#define GLAD_H

#include <GLES3/gl3.h>

/* No-op: Emscripten provides GL functions automatically */
static inline int gladLoadGL(void) { return 1; }

/* glPointSize does not exist in GLES3/WebGL 2 — point size is set
   via gl_PointSize in vertex shaders instead. Provide a no-op. */
static inline void glPointSize(float size) { (void)size; }

/* These constants are used in GLSL.cpp but not defined in GLES3 headers */
#ifndef GL_STACK_OVERFLOW
#define GL_STACK_OVERFLOW 0x0503
#endif
#ifndef GL_STACK_UNDERFLOW
#define GL_STACK_UNDERFLOW 0x0504
#endif

#endif /* GLAD_H */
