static int on_mirror_horizontal_operation(dx_asset_image* self) {
  for (dx_size row = 0; row < self->height / 2; ++row) {
	_swap_rows(self, row, self->height - 1 - row);
  }
  return dx_get_error();
}
