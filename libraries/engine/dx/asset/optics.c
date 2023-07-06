#include "dx/asset/optics.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.optics",
                      dx_asset_optics,
                      dx_object)

static void dx_asset_optics_destruct(dx_asset_optics* self)
{/*Intentionally empty.*/}

int dx_asset_optics_construct(dx_asset_optics* self) {
  dx_rti_type* _type = dx_asset_optics_get_type();
  if (!_type) {
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.optics_orthographic",
                      dx_asset_optics_orthographic,
                      dx_asset_optics)

static void dx_asset_optics_orthographic_destruct(dx_asset_optics_orthographic* self) {

  if (self->scale_y) {
    dx_memory_deallocate(self->scale_y);
    self->scale_y = NULL;
  }
  if (self->scale_x) {
    dx_memory_deallocate(self->scale_x);
    self->scale_x = NULL;
  }
  if (self->aspect_ratio) {
    dx_memory_deallocate(self->aspect_ratio);
    self->aspect_ratio = NULL;
  }
}

int dx_asset_optics_orthographic_construct(dx_asset_optics_orthographic* self) {
  dx_rti_type* _type = dx_asset_optics_orthographic_get_type();
  if (!_type) {
    return 1;
  }
  self->aspect_ratio = NULL;
  self->scale_x = dx_memory_allocate(sizeof(dx_f32));
  self->scale_y = dx_memory_allocate(sizeof(dx_f32));
  if (!self->scale_x || !self->scale_y) {
    if (self->scale_y) {
      dx_memory_deallocate(self->scale_y);
      self->scale_y = NULL;
    }
    if (self->scale_x) {
      dx_memory_deallocate(self->scale_x);
      self->scale_x = NULL;
    }
    return 1;
  }
  *self->scale_x = 1.f;
  *self->scale_y = 1.f;
  self->near = 0.1f;
  self->far = 1000.f;
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_optics_orthographic* dx_asset_optics_orthographic_create() {
  dx_rti_type* _type = dx_asset_optics_orthographic_get_type();
  if (!_type) {
    return NULL;
  }
  dx_asset_optics_orthographic* self = DX_ASSET_OPTICS_ORTHOGRAPHIC(dx_object_alloc(sizeof(dx_asset_optics_orthographic)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_optics_orthographic_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self; 
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

DX_DEFINE_OBJECT_TYPE("dx.asset.optics_perspective",
                      dx_asset_optics_perspective,
                      dx_asset_optics)

static void dx_asset_optics_perspective_destruct(dx_asset_optics_perspective* self) {
  if (self->aspect_ratio) {
    dx_memory_deallocate(self->aspect_ratio);
    self->aspect_ratio = NULL;
  }
}

int dx_asset_optics_perspective_construct(dx_asset_optics_perspective* self) {
  dx_rti_type* _type = dx_asset_optics_perspective_get_type();
  if (!_type) {
    return 1;
  }
  self->aspect_ratio = dx_memory_allocate(sizeof(dx_f32));
  if (!self->aspect_ratio) {
    return 1;
  }
  *self->aspect_ratio = 1.33f;
  self->field_of_view_y = 60.f;
  self->near = 0.1f;
  self->far = 1000.f;
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_asset_optics_perspective* dx_asset_optics_perspective_create() {
  dx_rti_type* _type = dx_asset_optics_perspective_get_type();
  if (!_type) {
    return NULL;
  }
  dx_asset_optics_perspective* self = DX_ASSET_OPTICS_PERSPECTIVE(dx_object_alloc(sizeof(dx_asset_optics_perspective)));
  if (!self) {
    return NULL;
  }
  if (dx_asset_optics_perspective_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self; 
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
