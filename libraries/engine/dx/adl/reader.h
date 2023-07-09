#if !defined(DX_ADL_READER_H_INCLUDED)
#define DX_ADL_READER_H_INCLUDED

#include "dx/adl/symbols.h"
#include "dx/asset/scene.h"
#include "dx/ddl.h"
#include "dx/adl/semantical/names.h"
typedef struct dx_adl_context dx_adl_context;
typedef struct dx_adl_semantical_reader dx_adl_semantical_reader;

DX_DECLARE_OBJECT_TYPE("dx.adl.semantical_reader",
                       dx_adl_semantical_reader,
                       dx_object)

static inline dx_adl_semantical_reader* DX_ADL_SEMANTICAL_READER(void* p) {
  return (dx_adl_semantical_reader*)p;
}

struct dx_adl_semantical_reader {
  dx_object _parent;
  int (*complete)(dx_adl_semantical_reader* self,
                  dx_adl_symbol* symbol,
                  dx_adl_context* context);
  dx_object* (*read)(dx_adl_semantical_reader* self,
                     dx_ddl_node* node,
                     dx_adl_context* context);
};

int dx_adl_semantical_reader_construct(dx_adl_semantical_reader* self);

int dx_adl_semantical_reader_complete(dx_adl_semantical_reader* self,
                                      dx_adl_symbol* symbol,
                                      dx_adl_context* context);

dx_object* dx_adl_semantical_reader_read(dx_adl_semantical_reader* self,
                                         dx_ddl_node* node,
                                         dx_adl_context* context);

#endif // DX_ADL_READER_H_INCLUDED