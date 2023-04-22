#if !defined(DX_APPLICATION_H_INCLUDED)
#define DX_APPLICATION_H_INCLUDED

#include "dx/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// The representation of an application.
typedef struct dx_application dx_application;

static dx_application* DX_APPLICATION(void* p) {
  return (dx_application*)p;
}

struct dx_application {
  dx_object _parent;
};

int dx_application_construct(dx_application* application);

void dx_application_destruct(dx_application* application);

/// @brief Create the singleton application object with an initial reference count of @a 1.
/// For a successful invocation of this function, the caller must invoke dx_application_shutdown.
/// @param msq_queue Pointer to the message queue object the application shall use.
/// @return A zero value on success. A non-zero value on failure.
/// @warning
/// This function will fail
/// - if @a msg_queue is a null pointer or
//  - if the application is already started up.
int dx_application_startup(dx_msg_queue* msg_queue);

/// @brief Decrement the reference count of the singleton application object by @a 1.
/// @remarks
/// Try to shutdown the application.
/// The application will not shut down until all references to the dx_application object are released.
void dx_application_shutdown();

/// @brief Get the singleton application object.
/// @return Returns null if the application does not exist.
/// Otherwise a pointer to the application object is returned an the reference count of the application object is incremented by @a 1.
dx_application* dx_application_get();

#endif // DX_APPLICATION_H_INCLUDED
