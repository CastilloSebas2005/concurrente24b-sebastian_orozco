// Copyright [2024] <Sebastián Orozco>

#include <stdio.h>

/**
 @brief This is the main procedure, this is to validate the argument input is correct and call the principal function of program.
 @param argc Number of arguments.
 @param argv Array of elements(chars).
 @return An error code:
   0 for success.
   1 if numbers of arguments are 1, no exist arguments.
   2 if a numbers of arguments are more than 2.
*/

// start main
int main(int argc, char *argv[]) {
  (void)argv;
  if (argc == 2) {
    printf("Welcome\n");
  } else if (argc == 1) {
    fprintf(stderr, "Error: this program need one argument to work\n");
    return 1;
  } else if (argc > 1) {
    fprintf(stderr, "Error: this program just need one argument(because this "
                    "is the serial version)\n");
    return 2;
  }
  return 0;
}  // end main
