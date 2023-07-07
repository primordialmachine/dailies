#if !defined(DX_ADL_CONTEXT_H_INCLUDED)
#define DX_ADL_CONTEXT_H_INCLUDED

#include "dx/adl/reader.h"
#include "dx/asset/scene.h"
#include "dx/ddl.h"
#include "dx/adl/semantical/names.h"

/// @brief The state maintained of a compiler when compiling an ADL program from a DDL program.
DX_DECLARE_OBJECT_TYPE("dx.adl.context",
                       dx_adl_context,
                       dx_object)

static inline dx_adl_context* DX_ADL_CONTEXT(void* p) {
  return (dx_adl_context*)p;
}

struct dx_adl_context {
  dx_object _parent;
  /// @brief The definitions.
  dx_asset_definitions* definitions;
  /// @brief Map from names (dx_string) to readers (dx_adl_semantical_reader).
  dx_pointer_hashmap readers;
  /// @brief The scene object that is being built.
  dx_asset_scene* scene;
  /// @brief A pointer to the names.
  dx_adl_semantical_names* names;
};

int dx_adl_context_construct(dx_adl_context* self);

dx_adl_context* dx_adl_context_create();

/// @brief Add a map entry (name, node) to the map from names (dx_string) to ADL syntax tree nodes (dx_adl_node).
/// @param self This semantical analyses state.
/// @param name The name.
/// @param node The node.
/// @return The zero value on success. A non-zero value on failure.
/// @default-failure In particular the following error codes are set:
/// - #DX_ALREADY_EXISTS A map entry for the specified name already exists.
/// - #DX_ALLOCATION_FAILED An allocation failed.
/// - #DX_INVALID_ARGUMENT @a self, @a name, or @a node is a null pointer.
int dx_adl_context_add_reader(dx_adl_context* self, dx_string* name, dx_adl_semantical_reader* reader);

/// @brief Get a definition by its name.
/// @param self This compiler context.
/// @param name The name of the definition.
/// @return A pointer to the definition on success. The null pointer on failure.
/// @default-failure In particular the following error codes are set:
/// - #DX_NOT_FOUND no definition of the specified name was found
/// - #DX_INVALID_ARGUMENT @a self is a null pointer.
/// - #DX_INVALID_ARGUMENT @a name is a null pointer
dx_ddl_node* dx_adl_context_get_definition_by_name(dx_adl_context* self, dx_string* name);

#endif // DX_ADL_CONTEXT_H_INCLUDED
