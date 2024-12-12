#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Structure to hold the commands and its args
typedef struct {
    char* cmd;
    char* args;
} Input;

// Function for parsing the input, and extracts the cmds and args
Input parse_input(const char* input) {
    Input result = {NULL, NULL};
    char* input_copy = strdup(input);
    int len = strlen(input_copy);

    // Skip leading whitespace
    char* start = input_copy;
    while (*start && (*start == ' ' || *start == '\t')) start++;

    // Check if starts with quote
    char quote = (*start == '\'' || *start == '"') ? *start : '\0';
    char* cmd_start = quote ? start + 1 : start;

    // Find end of command
    char* cmd_end;
    if (quote) {
        cmd_end = strchr(cmd_start, quote);
        if (!cmd_end) {
            // Unbalanced quote
            free(input_copy);
            return result;
        }
        *cmd_end = '\0'; // Terminate cmd
        result.cmd = strdup(cmd_start);

        // Find start of args (after closing quote)
        char* args_start = cmd_end +1;
        while (*args_start && *args_start == ' ') args_start++;
        if (*args_start) {
            result.args = strdup(args_start);
        }
    } else {
        // Regular space-separated parsing
        char* token = strtok(input_copy, " ");
        if (token) {
            result.cmd = strdup(token); // Allocate memory for cmd
            token = strtok(NULL, "\0");
            if (token) {
                result.args = strdup(token); // Allocate mem for args
            }
        }
    }
    

    // Clean up
    free(input_copy);
    return result;
}

// Function for checking if the CMD is builtin
int is_builtin (const char* cmd) {
    char builtin[4][10] = {"echo","type", "exit", "pwd"};
    int num_builtin = sizeof(builtin)/sizeof(builtin[0]);
    
    for (int i = 0; i < num_builtin; i++) {
        if (strcmp(cmd, builtin[i]) == 0) {
            return 1;
        }
    }
    return 0;
}
        
// Check if CMD is in PATH
// 1. INPUT: cmd
// 2. get PATH var
// 3. for each path in PATH:
//      - concat ( path + cmd ) get_cmd_path() => cmd_path
//      - check: file_exists(cmd_path) => (0,1)

char* get_cmd_path(const char* path, const char* cmd) {
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", path, cmd);
    return strdup(full_path);
}

int file_exists(const char* fpath) {
    FILE *file = fopen(fpath, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

char* cmd_in_path(const char* cmd) {
    char *path = getenv("PATH");
    if (path == NULL) return NULL;
    
    char *path_copy = strdup(path);
    char *dir;
    char full_path[1024];

    dir = strtok(path_copy, ":");
    while (dir != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        if (file_exists(full_path)) {
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
        fgets(input, sizeof(input), stdin);

        // Parse input
        input[strlen(input) - 1] = '\0';
        if (input[0] == '\0') { // Skip empty lines
            continue;
        }

        Input parsed = parse_input(input);
        char *cmd_path = cmd_in_path(parsed.cmd);
        
        /* printf("INPUT: %s\n", input); */
        /* printf("CMD: %s\n", parsed.cmd); */
        /* printf("ARGS: %s\n", parsed.args); */
        /* printf("CMD PATH: %s\n\n\n", cmd_path); */
        
        // Check if CMD is builin
        if (strcmp(parsed.cmd, "exit") == 0) {
            break;
        } else if (strcmp(parsed.cmd, "type") == 0) {
            if (parsed.args != NULL){

                char *args_path = cmd_in_path(parsed.args);

                if (is_builtin(parsed.args)) {
                    printf("%s is a shell builtin\n", parsed.args);

                } else if (args_path != NULL) {

                    printf("%s is %s\n", parsed.args, args_path);
                } else {
                    printf("%s: not found\n", parsed.args);
                }
            }
        } else if (strcmp(parsed.cmd, "pwd") == 0) {
            char wd[1024];
            printf("%s\n", getcwd(wd, sizeof(wd)));

        } else if (strcmp(parsed.cmd, "cd") == 0) {
            char *path;
            
            if (strcmp(parsed.args, "~") == 0){
                path = getenv("HOME");
            } else {
                path = strdup(parsed.args);
            }

            if (path == NULL) {
                fprintf(stderr, "cd: Unable to determine path\n");
                break;
            }

            if (chdir(path) != 0) {
                /* perror("cd: No such file or directory\n"); */
                printf("cd: %s: No such file or directory\n", path);
            }

        } else if (cmd_path != NULL) {
            /* printf("run: %s\n", cmd_path); */
            printf("running system\n");
            system(input);
        } else {
            printf("%s: command not found\n", input);
        }
        
        // Clean up mem
        free(parsed.cmd);
        free(parsed.args);

    }

    return 0;
}


