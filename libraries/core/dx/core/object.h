#if !defined(DX_CORE_OBJECT_H_INCLUDED)
#define DX_CORE_OBJECT_H_INCLUDED

#include "dx/core/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Initialize the runtime type system.
/// @return The zero valueon success. A non-zero value on failure.
int dx_rti_initialize();

/// @brief Uninitialize the runtime type system.
void dx_rti_unintialize();

/// @brief The opaque c representation of runtime type information on a type.
typedef struct dx_rti_type dx_rti_type;

/// @brief Get if a type is a fundemental type.
/// @param type A pointer to the type.
/// @return @a true if the type is a fundamental type. @a false otherwise.
/// @undefined @a type does not point to a type.
/// @undefined The runtime type system is not initialized.
dx_bool dx_rti_type_is_fundamental(dx_rti_type* type);

/// @brief Get if a type is an enumeration type.
/// @param type A pointer to the type.
/// @return @a true if the type is an enumeration type. @a false otherwise.
/// @undefined @a type does not point to a type.
/// @undefined The runtime type system is not initialized.
dx_bool dx_rti_type_is_enumeration(dx_rti_type* type);

/// @brief Get if a type is an object type.
/// @param type A pointer to the type.
/// @return @a true if the type is an object type. @a false otherwise.
/// @undefined @a type does not point to a type.
/// @undefined The runtime type system is not initialized.
dx_bool dx_rti_type_is_object(dx_rti_type* type);

/// @brief Used to register types like ee_(i|n)(8|16|32|64), ee_f(32|64), ee_size, and ee_boolean.
/// @param p, n An UTF-8 string. Must be a valid type name.
/// @param value_size The size of a value of that type.
/// @return A pointer to the dx_rti_type object representing the type on success. The null pointer on failure.
/// @undefined The runtime type system is not initialized.
/// @undefined @a p/@a n is not a valid type name.
/// @failure This function has set the error variable.
/// In particular, the following error codes are returned:
/// - #DX_ALLOCATION_FAILED an allocation failed
/// - #DX_EXISTS a type of the same name already exists
dx_rti_type* dx_get_or_create_fundamental(char const *p, size_t n, void(*on_type_destroyed)(), dx_size value_size);

#define DX_DECLARE_ENUMERATION_TYPE(NAME, C_NAME) \
  dx_rti_type* C_NAME##_get_type(); \
  typedef enum C_NAME C_NAME;

#define DX_DEFINE_ENUMERATION_TYPE(NAME, C_NAME, C_PARENT_NAME) \
  static dx_rti_type* C_NAME##_type = NULL; \
\
  static void C_NAME##_on_type_destroyed() { \
    C_NAME##_type = NULL; \
  } \
\
  dx_rti_type* C_NAME##_get_type() { \
    if (!C_NAME##_type) { \
      C_NAME##_type = dx_rti_get_or_create_fundamental(NAME, sizeof(NAME) - 1, &C_NAME##_on_type_destroyed, value_size); \
    } \
    return C_NAME##_type; \
  }


/// @brief Used to register enumeration types.
/// @param p, n An UTF-8 string. Must be a valid type name.
/// @return A pointer to the dx_rti_type object representing the type on success. The null pointer on failure.
/// @undefined The runtime type system is not initialized.
/// @undefined @a p/@a n is not a valid type name.
/// @failure This function has set the error variable.
/// In particular, the following error codes are returned:
/// - #DX_ALLOCATION_FAILED an allocation failed
/// - #DX_EXISTS a type of the same name already exists
dx_rti_type* dx_rti_get_or_create_enumeration(char const* p, size_t n, void(*on_type_destroyed)());

#define DX_DECLARE_FUNDAMENTAL_TYPE(NAME, C_NAME, C_PARENT_NAME) \
  dx_rti_type* C_NAME##_get_type(); \
  typedef struct C_NAME C_NAME;

#define DX_DEFINE_FUNDAMENTAL_TYPE(NAME, C_NAME, C_PARENT_NAME) \
  static dx_rti_type* C_NAME##_type = NULL; \
\
  static void C_NAME##_on_type_destroyed() { \
    C_NAME##_type = NULL; \
  } \
\
  dx_rti_type* C_NAME##_get_type() { \
    if (!C_NAME##_type) { \
      C_NAME##_type = dx_rti_get_or_create_enumeration(NAME, sizeof(NAME) - 1, &C_NAME##_on_type_destroyed); \
    } \
    return C_NAME##_type; \
  }

/// @brief Used to register object types.
/// @param p, n An UTF-8 string. Must be a valid type name.
/// @param parent A pointer to the parent type or a null pointer.
/// @param value_size The value size.
/// @return A pointer to the dx_rti_type object representing the type on success. The null pointer on failure.
/// @undefined The runtime type system is not initialized.
/// @undefined @a p/@a n is not a valid type name.
/// @failure This function has set the error variable.
/// In particular, the following error codes are returned:
/// - #DX_ALLOCATION_FAILED an allocation failed
/// - #DX_EXISTS a type of the same name already exists
dx_rti_type* dx_rti_get_or_create_object(char const* p, size_t n, void (*on_type_destroyed)(), dx_size value_size, dx_rti_type* parent, void (*destruct)(void*));

#define DX_DECLARE_OBJECT_TYPE(NAME, C_NAME, C_PARENT_NAME) \
  dx_rti_type* C_NAME##_get_type(); \
  typedef struct C_NAME C_NAME;

#define DX_DEFINE_OBJECT_TYPE(NAME, C_NAME, C_PARENT_NAME) \
  static dx_rti_type* C_NAME##_type = NULL; \
\
  /*To be defined by the developer.*/ \
  static void C_NAME##_destruct(C_NAME* self); \
\
  static void C_NAME##_on_type_destroyed() { \
    C_NAME##_type = NULL; \
  } \
\
  dx_rti_type* C_NAME##_get_type() { \
    if (!C_NAME##_type) { \
      dx_rti_type* parent = C_PARENT_NAME##_get_type(); \
      if (!parent) { \
        return NULL; \
      } \
      C_NAME##_type = dx_rti_get_or_create_object(NAME, sizeof(NAME) - 1, &C_NAME##_on_type_destroyed, sizeof(C_NAME), parent, (void(*)(void*))&C_NAME##_destruct); \
    } \
    return C_NAME##_type; \
  }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @brief Get the "DX.Object" type.
/// @return A pointer to the "DX.Object" type on success. The null pointer on failure.
dx_rti_type* dx_object_get_type();

/// @brief
/// The base of non-POD objects.
typedef struct dx_object dx_object;

/// @brief
/// Unconditionally cast a <code>void</code> pointer into a <code>dx_object</code> pointer.
/// @param p The <code>void</code> pointer.
/// @return The <code>dx_object</code> pointer.
static inline dx_object* DX_OBJECT(void *p) {
  return (dx_object*)p;
}

struct dx_object {
  /// @brief The reference count of the object.
  dx_reference_counter reference_count;
  /// @brief A pointer to the destructor of the object or a null pointer.
  void (*destruct)(dx_object* object);
#if _DEBUG && 1 == DX_OBJECT_WITH_MAGIC_BYTES
  char magic_bytes[4];
#endif
};

void DX_DEBUG_CHECK_MAGIC_BYTES(void* p);

/// @brief
/// Allocate a dx_object.
/// @param size
/// The size, in Bytes, of the object. Must be greater than or equal to <code>sizeof(dx_object)</code>.
/// @return
/// A pointer to the object on success. A null pointer on failure.
/// @post
/// On success, a pointer to dx_object object is returned.
/// The object is initialized such that <code>dx_object::destructor</code> is assigned a null pointer and <code>dx_object::reference_count</code> is assigned @a 1.
dx_object* dx_object_alloc(size_t size);

/// @brief
/// Increment the reference count of a dx_object object by @a 1.
/// @param object
/// A pointer to the dx_object object.
void dx_object_reference(dx_object *object);

/// @brief
/// Increment the reference count of a dx_object object by @a 1.
/// @param object
/// A pointer to the dx_object object.
void dx_object_unreference(dx_object* object);

static inline void DX_REFERENCE(void *p) {
  dx_object_reference(DX_OBJECT(p));
}

static inline void DX_UNREFERENCE(void *p) {
  dx_object_unreference(DX_OBJECT(p));
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_OBJECT_H_INCLUDED
