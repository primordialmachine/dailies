
static int on_checkerboard_pattern_fill_image_operation(dx_asset_image* self, OFFSET2 offset, EXTEND2 extend, dx_asset_image_operations_checkerboard_pattern_fill* image_operation) {
  dx_asset_image_operations_color_fill* first = dx_asset_image_operations_color_fill_create();
  dx_asset_image_operations_color_fill_set_color(first, image_operation->first_checker_color);
  if (!first) {
    return 1;
  }

  dx_asset_image_operations_color_fill* second = dx_asset_image_operations_color_fill_create();
  dx_asset_image_operations_color_fill_set_color(second, image_operation->second_checker_color);
  if (!second) {
    DX_UNREFERENCE(first);
    first = NULL;
    return 1;
  }

  for (dx_size y = 0; y < image_operation->number_of_checkers_vertical; ++y) {
    dx_size t = offset.top + y * image_operation->checker_size_vertical,
            h = image_operation->checker_size_vertical;
    // Fast clip.
    if (t + h > offset.top + extend.height) {
      continue;
    }
    // Slow clip.
    if (t + h > offset.top + extend.height) {
      dx_size delta = (t + h) - (offset.top + extend.height);
      h -= delta;
    }
    for (dx_size x = 0; x < image_operation->number_of_checkers_horizontal; ++x) {
      dx_size l = offset.left + x * image_operation->checker_size_horizontal,
              w = image_operation->checker_size_horizontal;
      // Fast clip.
      if (l > offset.left + extend.width) {
        continue;
      }
      // Slow clip.
      if (l + w > offset.left + extend.width) {
        dx_size delta = (l + w) - (offset.left + extend.width);
        w -= delta;
      }
      int even_x = x % 2 == 0;
      int even_y = y % 2 == 0;
      if (even_x != even_y) {
        dx_asset_image_apply(self, l, t, w, h, DX_ASSET_IMAGE_OPERATION(first));
      } else {
        dx_asset_image_apply(self, l, t, w, h, DX_ASSET_IMAGE_OPERATION(second));
      }
    }
  }

  DX_UNREFERENCE(second);
  second = NULL;

  DX_UNREFERENCE(first);
  first = NULL;

  return 0;
}
