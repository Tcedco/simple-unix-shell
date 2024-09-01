# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sched.h>

// Constants

// Maximum length of a command
# define MAX_COMMAND_LENGTH 1024

// Maximum number of arguments in a command
# define MAX_NUM_ARGS 64

/**
 * Read the command given by the user.
 * 
 * @param command: The command given by the user.
 * 
 * @return void
 */
void read_command(char *command)
{
    // Read the command from the user. If the command is longer than MAX_COMMAND_LENGTH, it will be truncated.
    if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
        perror("Failed to read command");
        exit(-1);
    }

    // Remove newline character
    command[strcspn(command, "\n")] = '\0';
}

/**
 * Parse the command given by the user.
 * 
 * @param command: The command given by the user.
 * @param args: Array of strings to store the command and its arguments.
 * 
 * @return void
 */
void parse_command(char *command, char **args)
{
    // Split the command into tokens
    int index = 0;
    args[index] = strtok(command, " ");

    // Store the tokens in the args array
    while (args[index] != NULL) {
        args[++index] = strtok(NULL, " ");
    }
}

/**
 * Execute the command given by the user.
 * 
 * @param args: Array of strings containing the command and its arguments.
 * 
 * @return void
 */
void execute_command(char **args)
{
    // Check if the command is empty
    if (args[0] == NULL) {
        return;
    }

    // Check if the command is a built-in command
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

    // Exit the shell
    else if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    // Display help message
    else if (strcmp(args[0], "help") == 0) {
        printf("helioshell: A simple shell implementation.\n\nSupported commands:\n");
        printf("   cd <directory>: Change the current working directory.\n");
        printf("   exit: Exit the shell.\n");
        printf("   help: Display this help message.\n");
        printf("You can also run any other command available in your system. Note that some commands may not work.\n");
    }

    else {
        pid_t PID = fork();

        // Check if fork failed
        if (PID == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        // Execute the command in the child process
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

    // Main loop. Read, parse, and execute commands.
    while (1) {
        printf("helioshell> ");
        read_command(command);
        parse_command(command, args);
        execute_command(args);
    }

    return 0;
}