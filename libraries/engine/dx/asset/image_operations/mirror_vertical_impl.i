static int on_mirror_vertical_operation(dx_asset_image *self) {
  for (dx_size column = 0; column < self->width / 2; ++column) {
    _swap_columns(self, column, self->width - 1 - column);
  }
  return dx_get_error();
}
