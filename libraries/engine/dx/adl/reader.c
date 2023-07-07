#include "dx/adl/reader.h"

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical_reader",
                      dx_adl_semantical_reader,
                      dx_object)

int dx_adl_semantical_reader_construct(dx_adl_semantical_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_reader_get_type();
  if (!_type) {
    return 1;
  }
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_reader_destruct(dx_adl_semantical_reader* self)
{/*Intentionally empty.*/}

dx_object* dx_adl_semantical_reader_read(dx_adl_semantical_reader* self,
                                         dx_ddl_node* node,
                                         dx_adl_context* context)
{ return self->read(self, node, context); }
