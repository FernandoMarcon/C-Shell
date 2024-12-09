#include <stdio.h>
#include <string.h>


int starts_with(const char *str, const char *prefix) {
    return strncmp(str, prefix, strlen(prefix));
}

char* get_args(const char *input, const char *command) {
    static char args[100];
    size_t start_idx = strlen(command) + 1;
    size_t end_idx   = strlen(input);

    memcpy(args, input+start_idx, end_idx - start_idx);
    args[end_idx - start_idx] = '\0';

    return args;
}

int main() {
    while (1) {

      printf("$ ");
      fflush(stdout);

      // Wait for user input
      char input[100];
      fgets(input, 100, stdin);

      input[strlen(input) - 1] = '\0';

      if (starts_with(input, "exit") == 0){
          break;
      } else if (starts_with(input, "echo") == 0) {
          char *args = get_args(input, "echo");
          printf("%s\n", args);
      } else {
          printf("%s: command not found\n", input);
      }
    }
  return 0;
}
