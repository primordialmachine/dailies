// Copyright (c) 2023 Michael Heilmann. All rights reserved.
// Author: Michael Heilmann (michaelheilmann@primordialmachine.com
// Copyright Copyright (c) 2023 Michael Heilmann. All rights reserved.

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

// EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// fwrite
#include <stdio.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int main(int argc, char **argv) {
  static const char TEXT[] = "Hello, World!";
  if (sizeof(TEXT) != fwrite(TEXT, 1, sizeof(TEXT), stdout)) {
    return EXIT_FAILURE;
  } else {
    return EXIT_SUCCESS;
  }
}
