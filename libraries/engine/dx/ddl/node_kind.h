#if !defined(DX_DDL_NODE_KIND_H_INCLUDED)
#define DX_DDL_NODE_KIND_H_INCLUDED

#include "dx/core.h"

DX_DECLARE_ENUMERATION_TYPE("dx.ddl.node_kind",
                            dx_ddl_node_kind)

enum dx_ddl_node_kind {
#define DEFINE(SYMBOL, STRING) \
  SYMBOL,
#include "dx/ddl/node_kind.i"
#undef DEFINE
};

#endif // DX_DDL_NODE_KIND_H_INCLUDED
