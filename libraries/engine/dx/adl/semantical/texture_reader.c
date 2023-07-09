#include "dx/adl/semantical/texture_reader.h"

#include "dx/asset/texture.h"
#include "dx/adl/semantical/read.h"

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_semantical_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_image* _read_image(dx_ddl_node* node, dx_adl_context* context);

static dx_asset_texture* _read_texture(dx_ddl_node* node, dx_adl_context* context);

static int complete(dx_adl_semantical_reader* self,
                    dx_adl_symbol* symbol,
                    dx_adl_context* context);

static dx_object* read(dx_adl_semantical_texture_reader* self,
                       dx_ddl_node* node,
                       dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical.texture_reader",
                      dx_adl_semantical_texture_reader,
                      dx_adl_semantical_reader)

static dx_asset_image* _read_image(dx_ddl_node* node, dx_adl_context* context) {
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(image_type))) {
    dx_adl_semantical_reader* image_reader = dx_pointer_hashmap_get(&context->readers, received_type);
    if (!image_reader) {
      return NULL;
    }
    dx_asset_image* asset = DX_ASSET_IMAGE(dx_adl_semantical_reader_read(image_reader, node, context));
    return asset;
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
}

static dx_asset_texture* _read_texture(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_texture* texture_value = NULL;
  dx_string* name_value = NULL;
  dx_asset_reference* image_reference_value = NULL;
  // name
  {
    name_value = dx_adl_semantical_read_name(node, context);
    if (!name_value) {
      goto END;
    }
  }
  // image
  {
    image_reference_value = dx_adl_semantical_read_image_instance_field(node, false, NAME(image_key), context);
    if (!image_reference_value) {
      goto END;
    }
  }
  //
  texture_value = dx_asset_texture_create(name_value, image_reference_value);
  DX_UNREFERENCE(image_reference_value);
  image_reference_value = NULL;
  DX_UNREFERENCE(name_value);
  name_value = NULL;
  if (!texture_value) {
    goto END;
  }
END:
  if (name_value) {
    DX_UNREFERENCE(name_value);
    name_value = NULL;
  }
  if (image_reference_value) {
    DX_UNREFERENCE(image_reference_value);
    image_reference_value = NULL;
  }
  return texture_value;
}

static int complete(dx_adl_semantical_reader* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  dx_asset_texture* texture = DX_ASSET_TEXTURE(symbol->asset);
  if (texture->image_reference->object) {
    return 0;
  }
  dx_adl_symbol* referenced_symbol = dx_asset_definitions_get(context->definitions, texture->image_reference->name);
  if (!referenced_symbol) {
    return 1;
  }
  texture->image_reference->object = referenced_symbol->asset;
  if (!texture->image_reference->object) {
    return 0;
  }
  return 0;
}

static dx_object* read(dx_adl_semantical_texture_reader* self, dx_ddl_node* node, dx_adl_context* context) {
  return DX_OBJECT(_read_texture(node, context));
}

int dx_adl_semantical_texture_reader_construct(dx_adl_semantical_texture_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_texture_reader_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->complete = (int(*)(dx_adl_semantical_reader*, dx_adl_symbol*, dx_adl_context*))&complete;
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object*(*)(dx_adl_semantical_reader*, dx_ddl_node*, dx_adl_context*))&read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_texture_reader_destruct(dx_adl_semantical_texture_reader* self)
{/*Intentionally empty.*/}

dx_adl_semantical_texture_reader* dx_adl_semantical_texture_reader_create() {
  dx_adl_semantical_texture_reader* self = DX_ADL_SEMANTICAL_TEXTURE_READER(dx_object_alloc(sizeof(dx_adl_semantical_texture_reader)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_texture_reader_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
