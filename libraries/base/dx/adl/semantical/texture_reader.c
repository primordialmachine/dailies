#include "dx/adl/semantical/texture_reader.h"

#include "dx/asset/texture.h"
#include "dx/adl/semantical/read.h"

static inline dx_string* _get_name(dx_adl_semantical_names* names, size_t index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(state->names, dx_semantical_name_index_##name)

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static dx_asset_image* _parse_image(dx_adl_node* node, dx_adl_semantical_state* state);

static dx_asset_texture* _parse_texture(dx_adl_node* node, dx_adl_semantical_state* state);

static dx_object* read(dx_adl_semantical_texture_reader*,
                       dx_adl_node* node,
                       dx_adl_semantical_state*);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical_texture_reader",
                      dx_adl_semantical_texture_reader,
                      dx_adl_semantical_reader)

static dx_asset_image* _parse_image(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_string* received_type = dx_adl_semantical_read_type(node, state->names);
  if (!received_type) {
    return NULL;
  }
  if (dx_string_is_equal_to(received_type, NAME(image_type))) {
    dx_adl_semantical_reader* image_reader = dx_pointer_hashmap_get(&state->readers, received_type);
    if (!image_reader) {
      return NULL;
    }
    dx_asset_image* asset = DX_ASSET_IMAGE(dx_adl_semantical_reader_read(image_reader, node, state));
    return asset;
  } else {
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
}

static dx_asset_texture* _parse_texture(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_asset_texture* texture = NULL;
  dx_asset_image* image = NULL;
  // image
  {
    dx_string* name = NAME(image_key);
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    if (!child_node) {
      goto END;
    }
    image = _parse_image(child_node, state);
    if (!image) {
      goto END;
    }
  }
  texture = dx_asset_texture_create(image);
  DX_UNREFERENCE(image);
  image = NULL;
  if (!texture) {
    goto END;
  }
END:
  if (image) {
    DX_UNREFERENCE(image);
    image = NULL;
  }
  return texture;
}

static dx_object* read(dx_adl_semantical_texture_reader* self, dx_adl_node* node, dx_adl_semantical_state* state) {
  return DX_OBJECT(_parse_texture(node, state));
}

int dx_adl_semantical_texture_reader_construct(dx_adl_semantical_texture_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_texture_reader_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object*(*)(dx_adl_semantical_reader*, dx_adl_node*, dx_adl_semantical_state*))&read;

  DX_OBJECT(self)->type = _type;
  DX_OBJECT(self)->destruct = (void(*)(dx_object*))&dx_adl_semantical_texture_reader_destruct;
  return 0;
}

void dx_adl_semantical_texture_reader_destruct(dx_adl_semantical_texture_reader* self) {
  dx_adl_semantical_reader_destruct(DX_ADL_SEMANTICAL_READER(self));
}

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