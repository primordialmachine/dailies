#include "dx/val/viewer.h"

#include "dx/asset/optics.h"

DX_DEFINE_OBJECT_TYPE("dx.val.viewer",
                      dx_val_viewer,
                      dx_object)

static void dx_val_viewer_destruct(dx_val_viewer* self) {
  DX_UNREFERENCE(self->asset_viewer_instance);
  self->asset_viewer_instance = NULL;
}

int dx_val_viewer_construct(dx_val_viewer* self, dx_asset_viewer_instance* asset_viewer_instance) {
  dx_rti_type* _type = dx_val_viewer_get_type();
  if (!_type) {
    return 1;
  }
  self->source = DX_ASSET_VIEWER(asset_viewer_instance->viewer_reference->object)->source;
  self->target = DX_ASSET_VIEWER(asset_viewer_instance->viewer_reference->object)->target;
  self->up = DX_ASSET_VIEWER(asset_viewer_instance->viewer_reference->object)->up;
  self->asset_viewer_instance = asset_viewer_instance;
  dx_mat4_set_identity(&self->view_matrix);
  dx_mat4_set_identity(&self->projection_matrix);
  DX_REFERENCE(asset_viewer_instance);
  DX_OBJECT(self)->type = _type;
  return 0;
}

dx_val_viewer* dx_val_viewer_create(dx_asset_viewer_instance* asset_viewer_instance) {
  dx_val_viewer* self = DX_VAL_VIEWER(dx_object_alloc(sizeof(dx_val_viewer)));
  if (!self) {
    return NULL;
  }
  if (dx_val_viewer_construct(self, asset_viewer_instance)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}

int dx_val_viewer_get_projection_matrix(DX_MAT4* result, dx_val_viewer* self, dx_i32 canvas_width, dx_i32 canvas_height) {
  if (!result || !self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  dx_asset_optics* optics = DX_ASSET_VIEWER(self->asset_viewer_instance->viewer_reference->object)->optics;
  if (!optics) {
    return 1;
  }
  if (dx_rti_type_is_leq(DX_OBJECT(optics)->type, dx_asset_optics_perspective_get_type())) {
    dx_asset_optics_perspective* optics1 = DX_ASSET_OPTICS_PERSPECTIVE(optics);
    // use actual aspect ratio
    if (optics1->aspect_ratio) {
      dx_memory_deallocate(optics1->aspect_ratio);
      optics1->aspect_ratio = NULL;
    }
    dx_f32 aspect_ratio = (dx_f32)canvas_width / (dx_f32)canvas_height;
    if (optics1->aspect_ratio) {
      aspect_ratio = *optics1->aspect_ratio;
    }
    dx_mat4_set_perspective(&self->projection_matrix, optics1->field_of_view_y, aspect_ratio, optics1->near, optics1->far);
  } else if (dx_rti_type_is_leq(DX_OBJECT(optics)->type, dx_asset_optics_orthographic_get_type())) {
    dx_asset_optics_orthographic* optics1 = DX_ASSET_OPTICS_ORTHOGRAPHIC(optics);
    dx_f32 left = -1.f;
    dx_f32 right = +1.f;
    if (optics1->scale_x) {
      left *= *optics1->scale_x;
      right *= *optics1->scale_x;
    }
    dx_f32 bottom = -1.f;
    dx_f32 top = +1.f;
    if (optics1->scale_y) {
      bottom *= *optics1->scale_y;
      top *= *optics1->scale_y;
    }
    dx_mat4_set_ortho(&self->projection_matrix, left, right, bottom, top, optics1->near, optics1->far);
  } else {
    return 1;
  }
  *result = self->projection_matrix;
  return 0;
}

int dx_val_viewer_get_view_matrix(DX_MAT4* result, dx_val_viewer* self, dx_i32 canvas_width, dx_i32 canvas_height) {
  if (!result || !self) {
    dx_set_error(DX_INVALID_ARGUMENT);
    return 1;
  }
  dx_mat4_set_look_at(&self->view_matrix, &self->source, &self->target, &self->up);
  *result = self->view_matrix;
  return 0;
}

