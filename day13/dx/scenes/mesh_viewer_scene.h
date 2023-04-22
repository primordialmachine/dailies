#if !defined(DX_SCENES_MESH_VIEWER_SCENE_H_INCLUDED)
#define DX_SCENES_MESH_VIEWER_SCENE_H_INCLUDED

#include "dx/scene.h"
#include "dx/asset/mesh.h"
#include "dx/vbinding.h"
#include "dx/buffer.h"
#include "dx/program.h"
#include "dx/command.h"

/// A scene for viewing a mesh.
typedef struct dx_mesh_viewer_scene dx_mesh_viewer_scene;
static inline dx_mesh_viewer_scene* DX_MESH_VIEWER_SCENE(void *p) {
  return (dx_mesh_viewer_scene*)p;
}

struct dx_mesh_viewer_scene {
  dx_scene _parent;
  char* name;

  dx_command_list* commands;
  DX_MAT4 projection_matrix;
  DX_MAT4 view_matrix;
  DX_MAT4 world_matrix;

  float time;
};

/// @brief Construct this mesh viewer scene.
/// @param scene A pointer to this mesh viewer scene.
/// @param name The name of the mesh to view.
/// @return @a 0 on success. A non-zero value on failure.
int dx_mesh_viewer_scene_construct(dx_mesh_viewer_scene* scene, char const *name);

/// @brief Destruct this scene.
/// @param scene A pointer to this scene.
void dx_mesh_viewer_scene_destruct(dx_mesh_viewer_scene* scene);

dx_mesh_viewer_scene* dx_mesh_viewer_scene_create(char const *name);

#endif // DX_SCENES_MESH_VIEWER_SCENE_H_INCLUDED
