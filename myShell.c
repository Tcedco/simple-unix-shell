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
    pid_t PID = fork();

    if (PID == -1) {
        perror("Failed to fork");
        exit(-1);
    }

    else if (PID == 0) {
        if (execvp(args[0], args) == -1) {
            perror("Failed to execute command");
            exit(-1);
        }

        exit(EXIT_FAILURE);
    }

    else {
        wait(NULL);
    }
}


int main()
{
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_NUM_ARGS];

    while (1) {
        printf("elsayedShell> ");
        read_command(command);
        parse_command(command, args);
        execute_command(args);
    }

    return 0;
}