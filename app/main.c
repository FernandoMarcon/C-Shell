#include <stdio.h>
#include <string.h>

int main() {
    while (1) {

      // Uncomment this block to pass the first stage
      printf("$ ");
      fflush(stdout);

      // Wait for user input
      char input[100];
      fgets(input, 100, stdin);

      input[strlen(input) - 1] = '\0';

      if (strcmp(input, "exit 0") == 0) {
          break;
      } else {
          printf("%s: command not found\n", input);
      }
    }
  return 0;
}
