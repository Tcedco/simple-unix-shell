# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sched.h>

# define MAX_COMMAND_LENGTH 1024
# define MAX_NUM_ARGS 64

void read_command(char *command)
{
    if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
        perror("Failed to read command");
        exit(-1);
    }

    // Remove newline character
    command[strcspn(command, "\n")] = '\0';
}


void parse_command(char *command, char **args)
{
    int index = 0;
    args[index] = strtok(command, " ");

    while (args[index] != NULL) {
        args[++index] = strtok(NULL, " ");
    }
}


void execute_command(char **args)
{
    if (args[0] == NULL) {
        return;
    }

    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "helioshell: expected argument to \"cd\"\n");
        }

        else {
            if (chdir(args[1]) != 0) {
                perror("helioshell");
            }
        }
    }

    else if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    else if (strcmp(args[0], "help") == 0) {
        printf("helioshell: A simple shell implementation.\n\nSupported commands:\n");
        printf("   cd <directory>: Change the current working directory.\n");
        printf("   exit: Exit the shell.\n");
        printf("   help: Display this help message.\n");
        printf("You can also run any other command available in your system. Note that some commands may not work.\n");
    }

    else {
        pid_t PID = fork();

        if (PID == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        else if (PID == 0) {
            // Child process
            if (execvp(args[0], args) == -1) {
                perror("helioshell");
            }

            exit(EXIT_FAILURE);
        }

        else {
            // Parent process
            wait(NULL); // Wait for child process to finish
        }
    }
}


int main()
{
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_NUM_ARGS];

    while (1) {
        printf("helioshell> ");
        read_command(command);
        parse_command(command, args);
        execute_command(args);
    }

    return 0;
}