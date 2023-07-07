#if !defined(DX_ADL_ENTER_H_INCLUDED)
#define DX_ADL_ENTER_H_INCLUDED

#include "dx/core.h"
#include "dx/ddl.h"
#include "dx/asset/scene.h"
#include "dx/adl/context.h"

DX_DECLARE_OBJECT_TYPE("dx.adl.enter",
                       dx_adl_enter,
                       dx_object)

static inline dx_adl_enter* DX_ADL_ENTER(void* p) {
  return (dx_adl_enter*)p;
}

struct dx_adl_enter {
  dx_object _parent;
  dx_adl_context* context;
};

int dx_adl_enter_construct(dx_adl_enter* self, dx_adl_context* context);

dx_adl_enter* dx_adl_enter_create(dx_adl_context* context);

int dx_adl_enter_run(dx_adl_enter* self, dx_ddl_node* source, dx_adl_context* context);

int dx_adl_enter_on_color(dx_adl_enter* self, dx_ddl_node* source, dx_adl_context* context);

int dx_adl_enter_on_scene(dx_adl_enter* self, dx_ddl_node* source, dx_adl_context* context);

int dx_adl_enter_on_image(dx_adl_enter* self, dx_ddl_node* source, dx_adl_context* context);

int dx_adl_enter_on_mesh(dx_adl_enter* self, dx_ddl_node* source, dx_adl_context* context);

int dx_adl_enter_on_mesh_instance(dx_adl_enter* self, dx_ddl_node* source, dx_adl_context* context);

int dx_adl_enter_on_material(dx_adl_enter* self, dx_ddl_node* source, dx_adl_context* context);

int dx_adl_enter_on_viewer_instance(dx_adl_enter* self, dx_ddl_node* source, dx_adl_context* context);

int dx_adl_enter_on_viewer(dx_adl_enter* self, dx_ddl_node* source, dx_adl_context* context);

int dx_adl_enter_on_texture(dx_adl_enter* self, dx_ddl_node* source, dx_adl_context* context);

#endif // DX_ADL_ENTER_H_INCLUDED
