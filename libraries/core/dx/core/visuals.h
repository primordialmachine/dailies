/// @file dx/core/visuals.h
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)
/// @brief Core functionality with a focus on visuals.
#if !defined(DX_CORE_VISUALS_H_INCLUDED)
#define DX_CORE_VISUALS_H_INCLUDED

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core-visuals
/// @brief Formats of vertices.
typedef enum DX_VERTEX_FORMAT {

  /// @brief A vertex of the format position.
  DX_VERTEX_FORMAT_POSITION_XYZ = 1,

  /// @brief A vertex of the format color.
  DX_VERTEX_FORMAT_AMBIENT_RGBA = 2,

  /// @brief A vertex of the format texture.
  DX_VERTEX_FORMAT_AMBIENT_UV = 4,

  /// @brief A vertex of the format position first, color second.
  DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_RGBA = (DX_VERTEX_FORMAT_POSITION_XYZ | DX_VERTEX_FORMAT_AMBIENT_RGBA),

  /// @brief A vertex of the format position first, texture second.
  DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_UV = (DX_VERTEX_FORMAT_POSITION_XYZ | DX_VERTEX_FORMAT_AMBIENT_UV),

  /// @brief A vertex of the format position first, color second, texture third.
  DX_VERTEX_FORMAT_POSITION_XYZ_AMBIENT_RGBA_AMBIENT_UV = (DX_VERTEX_FORMAT_POSITION_XYZ | DX_VERTEX_FORMAT_AMBIENT_RGBA | DX_VERTEX_FORMAT_AMBIENT_UV),

} DX_VERTEX_FORMAT;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/// @ingroup core-visuals
/// @brief Formats of pixels.
typedef enum DX_PIXEL_FORMAT {

  /// @brief A red component, a green component, and a blue component.
  /// The component type is uint8_t.
  /// A component value of 0 indicates least intensity.
  /// A component value of 255 indicates greatest intensity.
  DX_PIXEL_FORMAT_RGB_U8,

} DX_PIXEL_FORMAT;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#endif // DX_CORE_VISUALS_H_INCLUDED
