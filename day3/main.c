// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// Author: Michael Heilmann (michaelheilmann@primordialmachine.com
// Copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
#include "dx/core.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int startup();

static int shutdown();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

static int startup() {
  dx_log("enter: startup\n", sizeof("enter: startup\n"));
  dx_log("leave: startup\n", sizeof("leave: startup\n"));
  return 0;
}

static int shutdown() {
  dx_log("enter: shutdown\n", sizeof("enter: shutdown\n"));
  dx_log("leave: shutdown\n", sizeof("leave: shutdown\n"));
  return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int main(int argc, char **argv) {
  if (startup()) {
    return EXIT_FAILURE;
  }
  dx_log("Hello, World!\n", sizeof("Hello, World!\n"));
  if (shutdown()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
