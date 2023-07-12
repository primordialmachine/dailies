#include "dx/ddl.h"

static dx_ddl_parser* _create_parser() {
  dx_ddl_diagnostics* diagnostics = dx_ddl_diagnostics_create();
  if (!diagnostics) {
    return NULL;
  }
  dx_ddl_scanner* scanner = dx_ddl_scanner_create(diagnostics);
  if (!scanner) {
    DX_UNREFERENCE(diagnostics);
    diagnostics = NULL;
    return NULL;
  }
  dx_ddl_parser* parser = dx_ddl_parser_create(scanner, diagnostics);
  DX_UNREFERENCE(scanner);
  scanner = NULL;
  DX_UNREFERENCE(diagnostics);
  diagnostics = NULL;
  return parser;
}

dx_ddl_node* dx_ddl_compile(char const* p, dx_size n) {
  dx_ddl_parser* parser = _create_parser();
  if (!parser) {
    return NULL;
  }
  if (dx_ddl_parser_set(parser, p, n)) {
    DX_UNREFERENCE(parser);
    parser = NULL;
    return NULL;
  }
  dx_ddl_node* node = dx_ddl_parser_run(parser);
  DX_UNREFERENCE(parser);
  parser = NULL;
  return node;
}
