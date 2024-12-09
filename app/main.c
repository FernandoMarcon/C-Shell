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

int is_builtin (char *input, char *args) {
    char builtin[3][10] = {"echo","exit","type"};
    int  num_builtin = 3;
    
    int found = 0;
      
    for (int i = 0; i < num_builtin; i++) {
        if (strcmp(builtin[i], args) == 0) {
            found = 1;
            break;
        }
    }
    return found;
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

      } else if (starts_with(input, "type") == 0) {
          char *args = get_args(input, "type");
          
          if (is_builtin(input, args)) {
              printf("%s is a shell builtin\n", args);
          } else {
              printf("%s: not found\n", args);
          }          

      } else {
          printf("%s: command not found\n", input);
      }
    }
  return 0;
}
