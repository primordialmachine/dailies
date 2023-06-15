#if !defined(DX_SCENES_MESH_VIEWER_SCENE_H_INCLUDED)
#define DX_SCENES_MESH_VIEWER_SCENE_H_INCLUDED

#include "dx/scene.h"
#include "dx/val/command.h"
#include "dx/val/mesh.h"
#include "dx/val/mesh_instance.h"
#include "dx/asset/mesh_instance.h"
#include "dx/asset/scene.h"

/// A scene for viewing a mesh.
typedef struct dx_mesh_viewer_scene dx_mesh_viewer_scene;
static inline dx_mesh_viewer_scene* DX_MESH_VIEWER_SCENE(void *p) {
  return (dx_mesh_viewer_scene*)p;
}

struct dx_mesh_viewer_scene {
  dx_scene _parent;
  
  char* name;
  /// @brief The scene asset loaded.
  dx_asset_scene* asset_scene;
  /// @brief List of dx_mesh_instance objects to render.
  dx_object_array mesh_instances;

  /// @brief The commands to be executed when entering a frame.
  dx_command_list* commands;
  float angle;
  DX_VEC3 viewer_position;
  DX_MAT4 projection_matrix;
  DX_MAT4 view_matrix;
};

/// @brief Construct this mesh viewer scene.
/// @param scene A pointer to this mesh viewer scene.
/// @param path The path to the ADL file to load the scene from.
/// @return The zero value on success. A non-zero value on failure.
/// @remarks If an "on mesh loaded callback" is provided, that callback is invoked 
/// after the mesh was loaded to allow for custom post-processing on the mesh.
int dx_mesh_viewer_scene_construct(dx_mesh_viewer_scene* scene, char const *path);

/// @brief Destruct this scene.
/// @param scene A pointer to this scene.
void dx_mesh_viewer_scene_destruct(dx_mesh_viewer_scene* scene);

dx_mesh_viewer_scene* dx_mesh_viewer_scene_create(char const *path);

#endif // DX_SCENES_MESH_VIEWER_SCENE_H_INCLUDED
