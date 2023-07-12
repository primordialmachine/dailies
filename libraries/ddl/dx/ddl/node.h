/// @file dx/ddl/node.h
/// @brief Output of the syntactical analyses of the Data Definition Language (DDL) programs.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @copyright Copyright (c) 2022-2023 Michael Heilmann. All rights reserved.
#if !defined(DX_DDL_NODE_H_INCLUDED)
#define DX_DDL_NODE_H_INCLUDED

#include "dx/ddl/node_kind.h"

/// @brief An node for programs of the Data Definition Language (DDL).
DX_DECLARE_OBJECT_TYPE("dx.ddl.node",
                       dx_ddl_node,
                       dx_object)

static inline dx_ddl_node* DX_DDL_NODE(void* p) {
  return (dx_ddl_node*)p;
}

struct dx_ddl_node {
  dx_object _parent;
  dx_ddl_node_kind kind;
  union {
    dx_pointer_array list;
    dx_pointer_hashmap map;
    dx_string* string;
    dx_string* number;
  };
};

/// @brief Construct this ADL node with the specified type.
/// @param self A pointer to this ADL node.
/// @param kind The kind of this ADL node.
/// @success The node was assigned default values for the specified type.
/// Default values are:
/// - #dx_ddl_node_kind_empty: -
/// - #dx_ddl_node_kind_string: the empty string
/// - #dx_ddl_node_kind_number: the zero integer number
/// - #dx_ddl_node_kind_map: the empty map
/// - #dx_ddl_node_kind_list: the empty list
/// @default-failure
int dx_ddl_node_construct(dx_ddl_node* self, dx_ddl_node_kind kind);

/// @brief Create this ADL node with the specified type.
/// @param kind The kind of this ADL node.
/// @return A pointer to this ADL node. The null pointer on failure.
/// @success The node was assigned default values for the specified type.
/// See dx_ddl_node_construct for details.
/// @default-failure
dx_ddl_node* dx_ddl_node_create(dx_ddl_node_kind kind);

/// @brief Get the node kind of this ADL node.
/// @param self A pointer to this ADL node.
/// @return The node kind. This may be #dx_node_kind_error. #dx_node_kind_error is also returned on failure.
/// @failure This function has set the the error variable.
dx_ddl_node_kind dx_ddl_node_get_kind(dx_ddl_node const* self);

/// @brief Add or update the mapping from name to value in this ADL map node.
/// @param self A pointer to this ADL node.
/// @param name The name.
/// @param value The value.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a value is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_map.
int dx_ddl_node_map_set(dx_ddl_node* self, dx_string* name, dx_ddl_node* value);

/// @brief Get a mapping from a name to a value in this ADL map node.
/// @param self A pointer to this ADL node.
/// @param name The name.
/// @return A pointer to the the value on success.
/// The null pointer on failure.
/// @failure This function has set the the error variable. In particular the following error codes are set
/// - #DX_NOT_FOUND no entry for the specified key was found
/// - #DX_INVALID_ARGUMENT @a self was a null pointer
/// - #DX_INVALID_ARGUMENT @a name was a null pointer
dx_ddl_node* dx_ddl_node_map_get(dx_ddl_node const* self, dx_string* name);

/// @brief Append an ADL node to this ADL list node.
/// @param self A pointer to this ADL node.
/// @param value A pointer to the ADL node to append.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a value is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_list.
int dx_ddl_node_list_append(dx_ddl_node* self, dx_ddl_node* value);

/// @brief Prepend an ADL node to this ADL list node.
/// @param self A pointer to this ADL node.
/// @param value A pointer to the ADL node to prepend.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a value is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_list.
int dx_ddl_node_list_prepend(dx_ddl_node* self, dx_ddl_node* value);

/// @brief Insert an ADL node into this ADL list node.
/// @param self A pointer to this ADL node.
/// @param value A pointer to the ADL node to insert.
/// @param index The index at which to insert the ADL node.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a value is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_list.
int dx_ddl_node_list_insert(dx_ddl_node* self, dx_ddl_node* value, dx_size index);

/// @brief Get an ADL node at the specified index in this ADL list node.
/// @param self A pointer to this ADL node.
/// @param index The index at which to get the ADL node.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a index is greater than or equal to the length of this ADL list node
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_list.
dx_ddl_node* dx_ddl_node_list_get(dx_ddl_node* self, dx_size index);

/// @brief Get an ADL node at the specified index in this ADL list node.
/// @param self A pointer to this ADL node.
/// @param index The index at which to get the ADL node.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a index is greater than or equal to the length of this ADL list node
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_list.
dx_size dx_ddl_node_list_get_size(dx_ddl_node* self);

/// @brief Get the string value of this ADL node.
/// @param self A pointer to this ADL node.
/// @return The string value on success. The null pointer on failure.
/// @success The caller acquired a reference to the returned dx_string object.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_string.
dx_string* dx_ddl_node_get_string(dx_ddl_node const* self);

/// @brief Set the string value of this ADL node.
/// @param self A pointer to this ADL node.
/// @param string A pointer to the string value.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a string is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_string.
/// @undefined The string is not well-formatted.
int dx_ddl_node_set_string(dx_ddl_node* self, dx_string* string);

/// @brief Get the number value of this ADL node.
/// @param self A pointer to this ADL node.
/// @return The number value on success. The null pointer on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_number.
dx_string* dx_ddl_node_get_number(dx_ddl_node const* self);

/// @brief Set the number value of this ADL node.
/// @param self A pointer to this ADL node.
/// @param number A pointer to the string value.
/// @return The zero value on success. A non-zero value on failure.
/// @failure This function has set the the error variable. In particular, the following error codes are set:
/// - #DX_INVALID_ARGUMENT @a self is a null pointer
/// - #DX_INVALID_ARGUMENT @a number is a null pointer
/// - #DX_INVALID_OPERATION this node is not of node kind #dx_ddl_node_kind_number.
/// @undefined The string is not well-formatted.
int dx_ddl_node_set_number(dx_ddl_node* self, dx_string* number);

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_DDL_NODE_H_INCLUDED
