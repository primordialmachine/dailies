#include "dx/adl/reader.h"

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical_reader",
                      dx_adl_semantical_reader,
                      dx_object)

int dx_adl_semantical_reader_construct(dx_adl_semantical_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_reader_get_type();
  if (!_type) {
    return 1;
  }
  self->resolve = NULL;
  self->read = NULL;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_reader_destruct(dx_adl_semantical_reader* self)
{/*Intentionally empty.*/}

int dx_adl_semantical_reader_resolve(dx_adl_semantical_reader* self,
                                     dx_adl_symbol* symbol,
                                     dx_adl_context* context)
{
  assert(NULL != self);
  assert(NULL != self->resolve);
  return self->resolve(self, symbol, context);
}

dx_object* dx_adl_semantical_reader_read(dx_adl_semantical_reader* self,
                                         dx_ddl_node* node,
                                         dx_adl_context* context)
{
  assert(NULL != self);
  assert(NULL != self->read);
  return self->read(self, node, context); 
}
