#if !defined(DX_SCENE_H_INCLUDED)
#define DX_SCENE_H_INCLUDED

#include "dx/core.h"
#include "dx/val/context.h"

/// The base of all "Scenes".
///
/// "Scenes" are independent units of (audials & visuals & haptical) input and output.
/// "Scenes" are created, destroyed, and operated by "scene execution environment".
/// "Scenes" have a contract with the "Scene Execution Environment" (short: "SEE").
/// 1) the Scene is created by the SEE
/// 2) dx_scene_startup is called for the Scene by the SEE
/// If the call succeeds, go to 3) or 4).
/// Otherwise got to 2) or 5).
/// 3) dx_scene_render is called for the Scene by the SEE.
/// If the call succeeds, go to 3) or 4).
/// Otherwise go to 4).
/// 4) dx_scene_shutdown is called for the Scene by the SEE.
/// Go to 5.
/// 5) the Scene is destroyed by the Scene.
DX_DECLARE_OBJECT_TYPE("dx.scene", dx_scene, dx_object)

static inline dx_scene* DX_SCENE(void *p) {
  return (dx_scene*)p;
}

struct dx_scene {
  dx_object _parent;
  dx_context* context;
  int (*startup)(dx_scene* self, dx_context* context);
  int (*render)(dx_scene* self, dx_context* context, dx_f32 delta_seconds, dx_i32 canvas_width, dx_i32 canvas_height);
  int (*shutdown)(dx_scene* self, dx_context* context);
};

/// @brief Construct this scene.
/// @param self A pointer to this scene.
/// @return The zero value on success. A non-zero value on failure.
int dx_scene_construct(dx_scene* self);

/// @brief Callback. Invoked by the execution environment.
/// @param self A pointer to this scene.
/// @return The zero value on success. A non-zero value on failure.
int dx_scene_startup(dx_scene* self, dx_context* context);

/// @brief Callback. Invoked by the execution environment.
/// @param self A pointer to this scene.
/// @param context A pointer to the GL context.
/// @param delta_seconds The delta, in seconds, the visualization time has advanced.
/// @param canvas_width, canvas_height The width and height of the canvas.
/// @return The zero value on success. A non-zero value on failure.
int dx_scene_render(dx_scene* self, dx_context* context, dx_f32 delta_seconds, dx_i32 canvas_width, dx_i32 canvas_height);

/// @brief Callback. Invoked by the execution environment.
/// @param self A pointer to this scene.
/// @return The zero value on success. A non-zero value on failure.
int dx_scene_shutdown(dx_scene* self, dx_context* context);

#endif // DX_SCENE_H_INCLUDED
