#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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

int file_exists ( const char *path ) {
    FILE *file = fopen(path, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

int command_in_dir(const char *command, const char *dir) {
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s%s", dir, command);

    return file_exists(full_path);
}

char* find_command_path(const char* command) {
    char *path = getenv("PATH");
    if (path == NULL) return NULL;

    char *path_copy = strdup(path);
    char *dir;
    char full_path[1024];

    dir = strtok(path_copy, ":");
    while ( dir != NULL ) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);

        if ( access(full_path, X_OK) == 0 ){
            free(path_copy);
            return strdup(full_path);
        }

        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

int main() {
    while (1) {

      printf("$ ");
      fflush(stdout);

      char input[100];
      fgets(input, 100, stdin);

      input[strlen(input) - 1] = '\0';

      if (starts_with(input, "exit") == 0){
          break;
      } else if (starts_with(input, "echo") == 0) {
          char *args = get_args(input, "echo");
          printf("%s\n", args);

      } else if (starts_with(input, "type") == 0) {
          char *cmd = get_args(input, "type");
          char *cmd_path = find_command_path(cmd);

          if (is_builtin(input, cmd)) {
              printf("%s is a shell builtin\n", cmd);
          } else if (cmd_path) {
              printf("%s is %s\n",cmd, cmd_path);
              free(cmd_path);
          } else {
              printf("%s: not found\n", cmd);
          }          

      } else {
          printf("%s: command not found\n", input);
      }
    }
  return 0;
}
