#include "dx/gl/texture.h"

#include "dx/asset/image.h"

DX_DEFINE_OBJECT_TYPE("dx.gl.texture",
                      dx_gl_texture,
                      dx_val_texture)

static int dx_gl_texture_set_data(dx_gl_texture* self, dx_asset_texture* texture) {
  dx_gl_context* context = DX_GL_CONTEXT(DX_VAL_TEXTURE(self)->context);
  switch (DX_ASSET_IMAGE(texture->image_reference->object)->pixel_format) {
  case DX_PIXEL_FORMAT_RGB_U8: {
    context->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    context->glBindTexture(GL_TEXTURE_2D, self->id);
    context->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DX_ASSET_IMAGE(texture->image_reference->object)->width, DX_ASSET_IMAGE(texture->image_reference->object)->height, 0, GL_RGB, GL_UNSIGNED_BYTE, DX_ASSET_IMAGE(texture->image_reference->object)->pixels);
  } break;
  default: {
    dx_set_error(DX_ENVIRONMENT_FAILED);
    return 1;
  } break;
  }
  return 0;
}

int dx_gl_texture_construct(dx_gl_texture* self, dx_gl_context* context) {
  dx_rti_type* _type = dx_gl_texture_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_val_texture_construct(DX_VAL_TEXTURE(self), DX_CONTEXT(context))) {
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
  dx_string* name = dx_string_create("<default image>", sizeof("<default image>") - 1);
  if (!name) {
    context->glDeleteTextures(1, &self->id);
    self->id = 0;
    return 0;
  }
  dx_asset_image* image = dx_asset_image_create(name, DX_PIXEL_FORMAT_RGB_U8, 8, 8, &dx_colors_amber);
  DX_UNREFERENCE(name);
  name = NULL;
  if (!image) {
    context->glDeleteTextures(1, &self->id);
    self->id = 0;
    return 1;
  }
  context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  context->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  dx_f32 borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
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
  DX_VAL_TEXTURE(self)->set_data = (int(*)(dx_val_texture*, dx_asset_texture*)) & dx_gl_texture_set_data;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_gl_texture_destruct(dx_gl_texture* self) {
  if (self->id) {
    dx_gl_context* context = DX_GL_CONTEXT(DX_VAL_TEXTURE(self)->context);
    context->glDeleteBuffers(1, &self->id);
    self->id = 0;
  }
}

dx_gl_texture* dx_gl_texture_create(dx_gl_context* context) {
  dx_gl_texture* self = DX_GL_TEXTURE(dx_object_alloc(sizeof(dx_gl_texture)));
  if (!self) {
    return NULL;
  }
  if (dx_gl_texture_construct(self, context)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
