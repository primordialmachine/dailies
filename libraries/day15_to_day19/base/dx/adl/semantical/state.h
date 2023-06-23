#if !defined(DX_ADL_SEMANTICAL_STATE_H_INCLUDED)
#define DX_ADL_SEMANTICAL_STATE_H_INCLUDED

#include "dx/asset/palette.h"
#include "dx/asset/scene.h"
#include "dx/adl/syntactical.h"

/// @brief The state maintained while iterating over the ADL nodes in the semantical analyses stage.
DX_DECLARE_OBJECT_TYPE("dx.adl.semantical_state",
                       dx_adl_semantical_state,
                       dx_object)

static inline dx_adl_semantical_state* DX_ADL_SEMANTICAL_STATE(void* p) {
  return (dx_adl_semantical_state*)p;
}

struct dx_adl_semantical_state {
  dx_object _parent;
  /// @brief Map from names (dx_string) to ADL syntax tree nodes (dx_adl_node).
  dx_pointer_hashmap named_nodes;
  /// @brief The scene object that is being built.
  dx_asset_scene* scene;
};

int dx_adl_semantical_state_construct(dx_adl_semantical_state* self);

void dx_adl_semantical_state_destruct(dx_adl_semantical_state* self);

dx_adl_semantical_state* dx_adl_semantical_state_create();

/// @brief Add a map entry (name, node) to the map from names (dx_string) to ADL syntax tree nodes (dx_adl_node).
/// @param self This semantical analyses state.
/// @param name The name.
/// @param node The node.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the error variable. In particular the following error codes are set:
/// - #DX_ALREADY_EXISTS A map entry for the specified name already exists.
/// - #DX_ALLOCATION_FAILED An allocation failed.
/// - #DX_INVALID_ARGUMENT @a self, @a name, or @a node is a null pointer.
int dx_adl_semantical_add_named_node(dx_adl_semantical_state* self, dx_string* name, dx_adl_node* node);

#endif // DX_ADL_SEMANTICAL_STATE_H_INCLUDED
