#include "dx/adl/semantical/material_reader.h"

#include "dx/asset/material.h"
#include "dx/asset/color.h"
#include "dx/adl/semantical/read.h"
#include "dx/adl/enter.h"

static inline dx_string* _get_name(dx_adl_semantical_names* names, dx_size index) {
  DX_DEBUG_ASSERT(NULL != names);
  DX_DEBUG_ASSERT(index < DX_SEMANTICAL_NAMES_NUMBER_OF_NAMES);
  dx_string* name = names->names[index];
  DX_DEBUG_ASSERT(NULL != name);
  return name;
}

#define NAME(name) _get_name(context->names, dx_semantical_name_index_##name)

static dx_asset_color* _read_color_instance(dx_ddl_node* node, dx_adl_context* context);

static dx_asset_texture* _read_texture(dx_ddl_node* node, dx_adl_context* context);

static dx_asset_reference* _read_texture_reference(dx_ddl_node* node, dx_adl_context* context);

static dx_asset_reference* _read_texture_reference_field(dx_ddl_node* node, bool optional, dx_string* key, dx_adl_context* context);

static dx_asset_material* _read_material(dx_ddl_node* node, dx_adl_context* context);

static int resolve(dx_adl_semantical_material_reader* self, dx_adl_symbol* symbol, dx_adl_context* context);

static dx_object* read(dx_adl_semantical_material_reader* self, dx_ddl_node* node, dx_adl_context* context);

DX_DEFINE_OBJECT_TYPE("dx.adl.semantical.material_reader",
                      dx_adl_semantical_material_reader,
                      dx_adl_semantical_reader)

static dx_asset_color* _read_color_instance(dx_ddl_node* node, dx_adl_context* context) {
  dx_string* expected_type = NAME(color_instance_type);
  dx_string* received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  if (!dx_string_is_equal_to(received_type, expected_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_set_error(DX_SEMANTICAL_ERROR);
    return NULL;
  }
  dx_string* value = dx_adl_semantical_read_string(node, NAME(reference_key), context->names);
  if (!value) {
    return NULL;
  }
  // TODO: Check type of definitions. Handle cases of definitions not found and definition of the wrong type.
  dx_adl_symbol* sym = DX_ADL_SYMBOL(dx_asset_definitions_get(context->definitions, value));
  dx_asset_color* asset_color = DX_ASSET_COLOR(sym->asset);
  DX_UNREFERENCE(value);
  value = NULL;
  if (!asset_color) {
    return NULL;
  }
  return asset_color;
}

static dx_asset_texture* _read_texture(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_texture* asset = NULL;
  dx_string* expected_type = NULL;
  dx_string* received_type = NULL;

  received_type = dx_adl_semantical_read_type(node, context);
  if (!received_type) {
    return NULL;
  }
  expected_type = NAME(texture_type);
  if (dx_string_is_equal_to(expected_type, received_type)) {
    DX_UNREFERENCE(received_type);
    received_type = NULL;
    dx_adl_semantical_reader* reader = dx_pointer_hashmap_get(&context->readers, expected_type);
    if (!reader) {
      return NULL;
    }
    asset = DX_ASSET_TEXTURE(dx_adl_semantical_reader_read(reader, node, context));
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

static dx_asset_reference* _read_texture_reference(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_texture* texture = _read_texture(node, context);
  if (!texture) {
    return NULL;
  }
  dx_asset_reference* texture_reference = dx_asset_reference_create(texture->name);
  texture_reference->object = DX_OBJECT(texture);
  return texture_reference;
}

static dx_asset_reference* _read_texture_reference_field(dx_ddl_node* node, bool optional, dx_string* key, dx_adl_context* context) {
  dx_error old_error = dx_get_error();
  dx_ddl_node* child_node = dx_ddl_node_map_get(node, key);
  if (!child_node) {
    if (DX_NOT_FOUND != dx_get_error()) {
      return NULL;
    } else {
      dx_set_error(optional ? DX_NO_ERROR : DX_SEMANTICAL_ERROR);
    }
    return NULL;
  }
  return _read_texture_reference(child_node, context);
}

static dx_asset_material* _read_material(dx_ddl_node* node, dx_adl_context* context) {
  dx_asset_material* material_value = NULL;
  dx_string* name_value = NULL;
  // name
  {
    name_value = dx_adl_semantical_read_name(node, context);
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
    dx_asset_color* ambient_color_u8 = dx_adl_semantical_read_color_instance_field(node, true, NAME(ambient_color_key), context);
    if (!ambient_color_u8) {
      if (dx_get_error()) {
        DX_UNREFERENCE(material_value_1);
        material_value_1 = NULL;
        goto END;
      }
    } else {
      DX_VEC4 ambient_color_f32;
      ambient_color_f32.e[0] = ((dx_f32)ambient_color_u8->value.r) / 255.f;
      ambient_color_f32.e[1] = ((dx_f32)ambient_color_u8->value.g) / 255.f;
      ambient_color_f32.e[2] = ((dx_f32)ambient_color_u8->value.b) / 255.f;
      ambient_color_f32.e[3] = 1.f;
      DX_UNREFERENCE(ambient_color_u8);
      ambient_color_u8 = NULL;
      if (dx_asset_material_set_ambient_color(material_value_1, &ambient_color_f32)) {
        DX_UNREFERENCE(material_value_1);
        material_value_1 = NULL;
        goto END;
      }
    }
  }
  // ambientTexture?
  {
    dx_asset_reference* texture_reference = dx_adl_semantical_read_texture_instance_field(node, true, NAME(ambient_texture_key), context);
    //dx_asset_reference* texture_reference = _read_texture_reference_field(node, true, NAME(ambient_texture_key), context);
    if (!texture_reference) {
      if (dx_get_error()) {
        DX_UNREFERENCE(material_value_1);
        material_value_1 = NULL;
        goto END;
      }
    } else {
      if (dx_asset_material_set_ambient_texture(material_value_1, texture_reference)) {
        DX_UNREFERENCE(texture_reference);
        texture_reference = NULL;
        DX_UNREFERENCE(material_value_1);
        material_value_1 = NULL;
        goto END;
      }
      DX_UNREFERENCE(texture_reference);
      texture_reference = NULL;
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

static int resolve(dx_adl_semantical_material_reader* self, dx_adl_symbol* symbol, dx_adl_context* context) {
  if (symbol->resolved) {
    return 0;
  }
  dx_asset_material* material = DX_ASSET_MATERIAL(symbol->asset);
  if (!material->ambient_texture_reference) {
    symbol->resolved = true;
    return 0;
  }
  if (material->ambient_texture_reference->object) {
    symbol->resolved = true;
    return 0;
  }
  dx_adl_symbol* referenced_symbol = dx_asset_definitions_get(context->definitions, material->ambient_texture_reference->name);
  if (!referenced_symbol) {
    return 1;
  }
  material->ambient_texture_reference->object = referenced_symbol->asset;
  if (!material->ambient_texture_reference->object) {
    return 1;
  }
  symbol->resolved = true;
  return 0;
}

static dx_object* read(dx_adl_semantical_material_reader* self, dx_ddl_node* node, dx_adl_context* context) {
  return DX_OBJECT(_read_material(node, context));
}

int dx_adl_semantical_material_reader_construct(dx_adl_semantical_material_reader* self) {
  dx_rti_type* _type = dx_adl_semantical_material_reader_get_type();
  if (!_type) {
    return 1;
  }
  if (dx_adl_semantical_reader_construct(DX_ADL_SEMANTICAL_READER(self))) {
    return 1;
  }
  DX_ADL_SEMANTICAL_READER(self)->resolve = (int (*)(dx_adl_semantical_reader*, dx_adl_symbol*, dx_adl_context*))&resolve;
  DX_ADL_SEMANTICAL_READER(self)->read = (dx_object*(*)(dx_adl_semantical_reader*, dx_ddl_node*, dx_adl_context*))&read;
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
