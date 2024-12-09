#include <stdio.h>
#include <string.h>


int starts_with(const char *str, const char *prefix) {
    return strncmp(str, prefix, strlen(prefix));
}

int main() {
    while (1) {

      // Uncomment this block to pass the first stage
      printf("$ ");
      fflush(stdout);

      // Wait for user input
      char input[100];
      fgets(input, 100, stdin);

      input[strlen(input) - 1] = '\0';

      if (starts_with(input, "exit") == 0){
          break;
      } else if (starts_with(input, "echo") == 0) {
          char args[100];
          size_t start_idx = 5;
          size_t end_idx = strlen(input);

          memcpy(args, input+start_idx, end_idx - start_idx);
          args[end_idx - start_idx] = '\0';
          printf("%s\n", args);
      } else {
          printf("%s: command not found\n", input);
      }
      /* if (strcmp(input, "exit 0") == 0) { */
      /*     break; */
      /* } else if (strcmp(input, "echo ") == 0) { */
      /*     /1* echo_str *1/ */
      /*     printf("ECHO"); */
      /* } else { */
      /*     printf("%s: command not found\n", input); */
      /* } */
    }
  return 0;
}
