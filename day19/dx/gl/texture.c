#include "dx/gl/texture.h"

// malloc, free
#include <malloc.h>
#include "dx/asset/image.h"

static int dx_gl_texture_set_data(dx_gl_texture* self, dx_asset_texture* texture) {
  dx_gl_context* context = DX_GL_CONTEXT(DX_TEXTURE(self)->context);
  switch (texture->image->pixel_format) {
  case DX_PIXEL_FORMAT_RGB_U8: {
    context->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    context->glBindTexture(GL_TEXTURE_2D, self->id);
    context->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->image->width, texture->image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->image->pixels);
  } break;
  default: {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    return 1;
  } break;
  }
  return 0;
}

int dx_gl_texture_construct(dx_gl_texture* self, dx_gl_context* context) {
  if (dx_texture_construct(DX_TEXTURE(self), DX_CONTEXT(context))) {
    return 1;
  }
  context->glGetError(); // clear the error variable
  context->glGenTextures(1, &self->id);
  if (GL_NO_ERROR != context->glGetError()) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    return 1;
  }
  context->glBindTexture(GL_TEXTURE_2D, self->id);
  // Create the default 8x8 pixels amber texture.
  dx_asset_image* image = dx_asset_image_create_rgb_u8(DX_PIXEL_FORMAT_RGB_U8, 8, 8, &dx_colors_amber);
  if (!image) {
    context->glDeleteTextures(1, &self->id);
    self->id = 0;
    return 1;
  }
  context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
  context->glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  switch (image->pixel_format) {
  case DX_PIXEL_FORMAT_RGB_U8: {
    context->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    context->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
  } break;
  default: {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    DX_UNREFERENCE(image);
    image = NULL;
    context->glDeleteTextures(1, &self->id);
    self->id = 0;
    return 1;
  } break;
  }
  DX_UNREFERENCE(image);
  image = NULL;
  if (GL_NO_ERROR != context->glGetError()) {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    context->glDeleteTextures(1, &self->id);
    self->id = 0;
    return 1;
  }
  DX_TEXTURE(self)->set_data = (int(*)(dx_texture*, dx_asset_texture*)) & dx_gl_texture_set_data;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*)) & dx_gl_texture_destruct;
  return 0;
}

void dx_gl_texture_destruct(dx_gl_texture* texture) {
  if (texture->id) {
    dx_gl_context* context = DX_GL_CONTEXT(DX_TEXTURE(texture)->context);
    context->glDeleteBuffers(1, &texture->id);
    texture->id = 0;
  }
  dx_texture_destruct(DX_TEXTURE(texture));
}

dx_gl_texture* dx_gl_texture_create(dx_gl_context* context) {
  dx_gl_texture* texture = DX_GL_TEXTURE(dx_object_alloc(sizeof(dx_gl_texture)));
  if (!texture) {
    return NULL;
  }
  if (dx_gl_texture_construct(texture, context)) {
    DX_UNREFERENCE(texture);
    return NULL;
  }
  return texture;
}
