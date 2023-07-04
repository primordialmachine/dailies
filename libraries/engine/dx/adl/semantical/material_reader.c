#include "dx/adl/semantical/material_reader.h"

#include "dx/asset/material.h"
#include "dx/adl/semantical/read.h"

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(state->names, dx_semantical_name_index_##name)

static int _read_color_instance(dx_adl_node* node, dx_adl_semantical_state* state, DX_RGB_U8* target);

static dx_asset_texture* _read_texture(dx_adl_node* node, dx_adl_semantical_state* state);

static dx_asset_material* _read_material(dx_adl_node* node, dx_adl_semantical_state* state);

static dx_object* read(dx_adl_semantical_material_reader* self, dx_adl_node* node, dx_adl_semantical_state* state);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical.material_reader",
                      dx_adl_semantical_material_reader,
                      dx_adl_semantical_reader)

static int _read_color_instance(dx_adl_node* node, dx_adl_semantical_state* state, DX_RGB_U8* target) {
  dx_string* expected_type = NAME(color_instance_type);
  dx_string* received_type = dx_adl_semantical_read_type(node, state);
  if (!received_type) {
    return 1;
  }
  if (!dx_string_is_equal_to(received_type, expected_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_SEMANTICAL_ERROR);
    return 1;
  }
  dx_string* value = dx_adl_semantical_read_string(node, NAME(reference_key), state->names);
  if (!value) {
    return 1;
  }
  dx_asset_palette_entry* palette_entry = dx_asset_palette_get(state->scene->palette, value);
  DX_UNREFERENCE(value);
  value = NULL;
  if (!palette_entry) {
    return 1;
  }
  (*target) = palette_entry->value;
  return 0;
}

static dx_asset_texture* _read_texture(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_asset_texture* asset = NULL;
  dx_string* expected_type = NULL;
  dx_string* received_type = NULL;

  received_type = dx_adl_semantical_read_type(node, state);
  if (!received_type) {
    return NULL;
  }
  expected_type = NAME(texture_type);
  if (dx_string_is_equal_to(expected_type, received_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&state->readers, expected_type);
    if (!reader) {
      return NULL;
    }
    asset = DX_ASSET_TEXTURE(dx_adl_semantical_reader_read(reader, node, state));
    if (!asset) {
      return NULL;
    }
  } else {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_SEMANTICAL_ERROR);
    goto END;
  }
END:
  return asset;
}

static dx_asset_material* _read_material(dx_adl_node* node, dx_adl_semantical_state* state) {
  dx_asset_material* material_value = NULL;
  dx_string* name_value = NULL;
  // name
  {
    name_value = dx_adl_semantical_read_name(node, state);
    if (!name_value) {
      goto END;
    }
  }

  dx_asset_material* material_value_1 = dx_asset_material_create(name_value);
  DX_UNREFERENCE(name_value);
  name_value = NULL;
  if (!material_value_1) {
    goto END;
  }

  // ambientColor?
  {
    dx_string* name = NAME(ambient_color_key);
    dx_error old_error = dx_get_error();
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    if (child_node) {
      DX_RGB_U8 ambient_color_u8;
      if (_read_color_instance(child_node, state, &ambient_color_u8)) {
        goto END;
      }
      DX_VEC4 ambient_color_f32;
      ambient_color_f32.e[0] = ((dx_f32)ambient_color_u8.r) / 255.f;
      ambient_color_f32.e[1] = ((dx_f32)ambient_color_u8.g) / 255.f;
      ambient_color_f32.e[2] = ((dx_f32)ambient_color_u8.b) / 255.f;
      ambient_color_f32.e[3] = 1.f;
      if (dx_asset_material_set_ambient_color(material_value_1, &ambient_color_f32)) {
        goto END;
      }
    } else {
      dx_set_error(old_error);
    }
  }
  // ambientTexture?
  {
    dx_string* name = NAME(ambient_texture_key);
    dx_error old_error = dx_get_error();
    dx_adl_node* child_node = dx_adl_node_map_get(node, name);
    if (child_node) {
      dx_asset_texture* ambient_texture = _read_texture(child_node, state);
      if (!ambient_texture) {
        goto END;
      }
      if (dx_asset_material_set_ambient_texture(material_value_1, ambient_texture)) {
        DX_UNREFERENCE(ambient_texture);
        ambient_texture = NULL;
        goto END;
      }
      DX_UNREFERENCE(ambient_texture);
      ambient_texture = NULL;
    } else {
      dx_set_error(old_error);
    }
  }
  material_value = material_value_1;
END:
  if (name_value) {
    DX_UNREFERENCE(name_value);
    name_value = NULL;
  }
  return material_value;
}

static dx_object* read(dx_adl_semantical_material_reader* self, dx_adl_node* node, dx_adl_semantical_state* state) {
  return DX_OBJECT(_read_material(node, state));
}

int dx_adl_semantical_material_reader_construct(dx_adl_semantical_material_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_material_reader_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object*(*)(dx_adl_semantical_reader*, dx_adl_node*, dx_adl_semantical_state*))&read;
  DX_OBJECT(self)->type = _type;
  return 0;
}

static void dx_adl_semantical_material_reader_destruct(dx_adl_semantical_material_reader* self)
{/*Intentionally empty.*/}

dx_adl_semantical_material_reader* dx_adl_semantical_material_reader_create() {
  dx_adl_semantical_material_reader* self = DX_ADL_SEMANTICAL_MATERIAL_READER(dx_object_alloc(sizeof(dx_adl_semantical_material_reader)));
  if (!self) {
    return NULL;
  }
  if (dx_adl_semantical_material_reader_construct(self)) {
    DX_UNREFERENCE(self);
    self = NULL;
    return NULL;
  }
  return self;
}
