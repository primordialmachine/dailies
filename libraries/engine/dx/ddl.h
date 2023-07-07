/// @file dx/ddl.h
/// @brief Invocation of the Data Definition Language ("DDL") compiler.
/// @author Michael Heilmann (michaelheilmann@primordialmachine.com)

#if !defined(DX_DDL_H_INCLUDED)
#define DX_ADL_SYNTACTICAL_H_INCLUDED

#include "dx/adl/diagnostics.h"
#include "dx/ddl/node.h"
#include "dx/ddl/parser.h"

dx_ddl_node* dx_ddl_compile(char const* p, dx_size n);

#endif // DX_DDL_H_INCLUDED
