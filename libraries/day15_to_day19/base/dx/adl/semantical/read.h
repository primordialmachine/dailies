#if !defined(DX_ADL_SEMANTICAL_READ_H_INCLUDED)
#define DX_ADL_SEMANTICAL_READ_H_INCLUDED

#include "dx/adl/syntactical.h"
#include "dx/adl/semantical/names.h"

/// @brief Parse
/// @code
/// <name> ':' <number>
/// @endcode
/// @param node A pointer to a map node.
/// @param name The key.
/// @param [out] target A pointer to a <code>dx_n8</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_adl_semantical_read_n8(dx_adl_node* node, dx_string* name, dx_n8* target);

/// @brief Parse
/// @code
/// <name> ':' <number>
/// @endcode
/// @param node A pointer to a map node.
/// @param name The key.
/// @param [out] target A pointer to a <code>dx_size</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_adl_semantical_read_sz(dx_adl_node* node, dx_string* name, dx_size* target);

/// @brief Parse
/// @code
/// <name> ':' <number>
/// @endcode
/// @param node A pointer to a map node.
/// @param name The key.
/// @param [out] target A pointer to a <code>dx_f32</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_adl_semantical_read_f32(dx_adl_node* node, dx_string* name, dx_f32* target);

/// @brief Parse
/// @code
/// <name> ':' <number>
/// @endcode
/// @param node A pointer to a map node.
/// @param name The key.
/// @param [out] target A pointer to a <code>dx_f64</code> variable.
/// @return The zero value on success. A non-zero value on failure.
int dx_adl_semantical_read_f64(dx_adl_node* node, dx_string* name, dx_f64* target);

/// @brief Parse
/// @code
/// <name> ':' <string>
/// @endcode
/// @param node A pointer to a map node.
/// @param name The key.
/// @param [out] target A pointer to a <code>dx_f64</code> variable.
/// @return A pointer to the value on success. The null pointer on failure.
dx_string* dx_adl_semantical_read_string(dx_adl_node* node, dx_string* name, dx_adl_semantical_names* names);

#endif // DX_ADL_SEMANTICAL_READ_H_INCLUDED
