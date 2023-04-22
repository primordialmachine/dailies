#include "dx/gl/wgl/context.h"

static void* link(char const* name);

static void* link(char const* name) {
  void* p = (void*)wglGetProcAddress(name);
  if (!p) {
    // This technique is required for OpenGL 1.0 core functions.
    HMODULE module = LoadLibraryA("opengl32.dll");
    if (!module) {
      return NULL;
    }
    p = (void*)GetProcAddress(module, name);
  }
  return p;
}

int dx_gl_wgl_context_construct(dx_gl_wgl_context* ctx) {
  if (dx_gl_context_construct(DX_GL_CONTEXT(ctx), &link)) {
    return 1;
  }
  DX_OBJECT(ctx)->destruct = (void(*)(dx_object*)) & dx_gl_wgl_context_destruct;
  return 0;
}

void dx_gl_wgl_context_destruct(dx_gl_wgl_context* ctx) {
  dx_gl_context_destruct(DX_GL_CONTEXT(ctx));
}

dx_gl_wgl_context* dx_gl_wgl_context_create() {
  dx_gl_wgl_context* ctx = DX_GL_WGL_CONTEXT(dx_object_alloc(sizeof(dx_gl_wgl_context)));
  if (!ctx) {
    return NULL;
  }
  if (dx_gl_wgl_context_construct(ctx)) {
    DX_UNREFERENCE(ctx);
    return NULL;
  }
  return ctx;
}
