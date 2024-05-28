#include <config.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

int main(void) {
  char* input;
  puts("Hello World!");
  puts("This is " PACKAGE_STRING ".");
  input = readline("Enter your name: ");
  printf("Hello, %s!\n", input);
  free(input);
  return 0;
}
