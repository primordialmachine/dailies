#include "dx/gl/wgl/context.h"

DX_DEFINE_OBJECT_TYPE("dx.gl.wgl.context",
                      dx_gl_wgl_context,
                      dx_gl_context)

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

int dx_gl_wgl_context_construct(dx_gl_wgl_context* self) {
  dx_rti_type* _type = dx_gl_wgl_context_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_gl_context_construct(DX_GL_CONTEXT(self), &link)) {
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_gl_wgl_context_destruct(dx_gl_wgl_context* self)
{/*Intentionally empty.*/}

dx_gl_wgl_context* dx_gl_wgl_context_create() {
  dx_gl_wgl_context* self = DX_GL_WGL_CONTEXT(dx_object_alloc(sizeof(dx_gl_wgl_context)));
  if (!self) {
    return NULL;
  }
  if (dx_gl_wgl_context_construct(self)) {
    DX_UNREFERENCE(self);
    return NULL;
  }
  return self;
}
