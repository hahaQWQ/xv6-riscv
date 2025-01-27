#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"  // For MAXARG
#define NULL 0
char** split_command(char* cmd, int* count)
{
    int cmd_count = 0;
    char* temp = cmd;
    while (*temp) {
        while (*temp == ' ') {
            temp++;
        }
        if (*temp == '\0') {
            break;
        }
        cmd_count++;
        while (*temp != ' ' && *temp != '\0') {
            temp++;
        }
    }

    char** args = (char**)malloc(sizeof(char*) * (cmd_count + 1));
    if (args == NULL) {
        return NULL;
    }

    temp = cmd;
    int idx = 0;
    while (*temp) {
        while (*temp == ' ') {
            temp++;
        }
        if (*temp == '\0') {
            break;
        }
        char* start = temp;
        while (*temp != ' ' && *temp != '\0') {
            temp++;
        }
        int len = temp - start;
        args[idx] = (char*)malloc(sizeof(char) * (len + 1));
        if (args[idx] == NULL) {
            // Free previously allocated memory on failure.
            for (int i = 0; i < idx; i++) {
                free(args[i]);
            }
            free(args);
            return NULL;
        }
        memcpy(args[idx], start, len);
        args[idx][len] = '\0';
        idx++;
    }
    args[cmd_count] = 0;
    *count = cmd_count;
    return args;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(2, "Usage: xargs command [args...]\n");
        exit(1);
    }

    char buf[512];
    int c = read(0, buf, sizeof(buf));
    if (c < 0) {
        fprintf(2, "xargs: read error\n");
        exit(1);
    }
    if (c == 0) {
        exit(0);  // No input.
    }
    buf[c - 1] = '\0';  // Replace newline with null terminator.

    int count;
    char** args = split_command(buf, &count);
    if (args == NULL) {
        fprintf(2, "xargs: split_command failed\n");
        exit(1);
    }

    int n = 1;  // Default: one argument per command.
    int exec_idx = 1;  // Index of the command in argv.

    // Parse -n option.
    if (argc >= 3 && strcmp(argv[1], "-n") == 0) {
        n = atoi(argv[2]);
        if (n <= 0) {
            fprintf(2, "xargs: invalid argument for -n\n");
            exit(1);
        }
        exec_idx = 3;
    }

    // Execute the command for each group of arguments.
    for (int i = 0; i < count; i += n) {
        // Allocate memory for the arguments.
        char** args_exec = (char**)malloc(sizeof(char*) * (MAXARG));
        if (args_exec == NULL) {
            fprintf(2, "xargs: malloc failed\n");
            exit(1);
        }

        // Copy the command and its arguments.
        int j;
        for (j = 0; j < argc - exec_idx; j++) {
            args_exec[j] = argv[exec_idx + j];
        }

        // Copy the additional arguments.
        for (int k = 0; k < n && i + k < count; k++) {
            args_exec[j + k] = args[i + k];
        }
        args_exec[j + n] = 0;  // Null-terminate the array.

        // Fork and execute the command.
        if (fork() == 0) {
            exec(args_exec[0], args_exec);
            fprintf(2, "xargs: exec failed\n");
            exit(1);
        } else {
            wait(0);
        }

        // Free the allocated memory.
        free(args_exec);
    }

    // Free the allocated memory.
    for (int i = 0; i < count; i++) {
        free(args[i]);
    }
    free(args);

    exit(0);
}