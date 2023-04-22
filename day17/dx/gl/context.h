#if !defined(DX_GL_CONTEXT_H_INCLUDED)
#define DX_GL_CONTEXT_H_INCLUDED

#include "dx/context.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h> /// @todo Should be removed. This is about GL not WGL.

typedef void (APIENTRY*PFNGLVIEWPORTPROC)(GLint, GLint, GLsizei, GLsizei);
typedef void (APIENTRY*PFNGLCLEARPROC)(GLbitfield);
typedef void (APIENTRY*PFNGLCLEARCOLORPROC)(GLclampf, GLclampf, GLclampf, GLclampf);
typedef void (APIENTRY* PFNGLCLEARDEPTHPROC)(GLdouble);
typedef GLenum (APIENTRY*PFNGLGETERRORPROC)();
typedef void (APIENTRY*PFNGLDRAWARRAYSPROC)(GLenum, GLint, GLsizei);
typedef void (APIENTRY*PFNGLSCISSORPROC)(GLint, GLint, GLsizei, GLsizei);
typedef void (APIENTRY* PFNGLGENTEXTURESPROC)(GLsizei, GLuint*);
typedef void (APIENTRY* PFNGLDELETETEXTURESPROC)(GLsizei, const GLuint*);
typedef void (APIENTRY* PFNGLTEXPARAMETERIPROC)(GLenum, GLenum, GLint);
typedef void (APIENTRY* PFNGLTEXPARAMETERFVPROC)(GLenum, GLenum, const GLfloat*);
typedef void (APIENTRY* PFNGLBINDTEXTUREPROC)(GLenum, GLuint);
typedef void (APIENTRY* PFNGLTEXIMAGE2DPROC)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*);
typedef void (APIENTRY* PFNGLGETINTEGERVPROC)(GLenum, GLint*);
typedef void (APIENTRY* PFNGLPIXELSTOREIPROC)(GLenum, GLint);
typedef void (APIENTRY* PFNGLENABLEPROC)(GLenum);
typedef void (APIENTRY* PFNGLDISABLEPROC)(GLenum);
typedef void (APIENTRY* PFNGLDEPTHFUNCPROC)(GLenum);
typedef void (APIENTRY* PFNGLCULLFACEPROC)(GLenum);

typedef struct dx_gl_context dx_gl_context;
static inline dx_gl_context* DX_GL_CONTEXT(void* p) {
  return (dx_gl_context*)p;
}

struct dx_gl_context {
  dx_context _parent;
#define DEFINE(TYPE, NAME) TYPE NAME;
#include "dx/gl/functions.i"
#undef DEFINE
};

int dx_gl_context_construct(dx_gl_context* ctx, void *(*link)(char const* name));

void dx_gl_context_destruct(dx_gl_context* ctx);

#endif // DX_GL_CONTEXT_H_INCLUDED
