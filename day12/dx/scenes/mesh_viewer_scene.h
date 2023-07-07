#if !defined(DX_SCENES_MESH_VIEWER_SCENE_H_INCLUDED)
#define DX_SCENES_MESH_VIEWER_SCENE_H_INCLUDED

#include "dx/scene.h"

#include "dx/val/command.h"
#include "dx/val/mesh.h"
#include "dx/val/mesh_instance.h"

#include "dx/asset/scene.h"
#include "dx/asset/mesh_instance.h"
#include "dx/asset/viewer_instance.h"

/// A scene for viewing a mesh.
DX_DECLARE_OBJECT_TYPE("dx.mesh_viewer_scene",
                       dx_mesh_viewer_scene,
                       dx_scene)

static inline dx_mesh_viewer_scene* DX_MESH_VIEWER_SCENE(void *p) {
  return (dx_mesh_viewer_scene*)p;
}

struct dx_mesh_viewer_scene {
  dx_scene _parent;

  /// @brief A pointer to the dx_asset_scene.
  dx_asset_scene* asset_scene;
  /// @brief A pointer to an UTF-8 C string. The path of the ADL file to load.
  char* name;

  dx_f32 time;
  /// @brief List of dx_val_mesh_instance objects to render.
  dx_object_array mesh_instances;
  /// @brief List of dx_val_viewer objects.
  dx_object_array viewers;

  /// @brief The commands to be executed when entering a frame.
  dx_command_list* commands;
};

/// @brief Construct this mesh viewer scene.
/// @param self A pointer to this mesh viewer scene.
/// @param path A pointer to an UTF-8 C string. The path of the ADL file to load.
/// @return The zero value on success. A non-zero value on failure.
/// @remarks If an "on mesh loaded callback" is provided, that callback is invoked
/// after the mesh was loaded to allow for custom post-processing on the mesh.
int dx_mesh_viewer_scene_construct(dx_mesh_viewer_scene* self, char const *path);

dx_mesh_viewer_scene* dx_mesh_viewer_scene_create(char const *path);

#endif // DX_SCENES_MESH_VIEWER_SCENE_H_INCLUDED
