// Copyright [2024] <Sebastián Orozco>

#include <stdio.h>
#include <stdlib.h>
#include "controller.h"

/** @mainpage This program is a simulation of heat transfer
 * 
 */

/**
 @brief This is the main procedure, this is to validate the argument input is
 correct and call the principal function of program.
 @param argc Number of arguments.
 @param argv Array of elements(chars).
 @return An error code:
   0 for success.
   1 if numbers of arguments are 3 or less, no exist arguments.
   2 if a numbers of arguments are more than 3.
*/

// start main
int main(int argc, char *argv[]) {
  if (argc == 4) {
    printf("Welcome\n");
    init_controller(argv);
  } else if (argc <= 3) {
    fprintf(stderr, "Error: this program need three argument to work\n");
    return 1;
  } else if (argc > 3) {
    fprintf(stderr,
            "Error: this program need three arguments no more(because this "
            "is the serial version)\n");
    return 2;
  }
  return 0;
}  // end main
