#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PROMPT "lambda-shell$ "

#define MAX_TOKENS 100
#define COMMANDLINE_BUFSIZE 1024
#define DEBUG 0  // Set to 1 to turn on some debugging output, or 0 to turn off

/**
 * Parse the command line.
 *
 * YOU DON'T NEED TO MODIFY THIS!
 * (But you should study it to see how it works)
 *
 * Takes a string like "ls -la .." and breaks it down into an array of pointers
 * to strings like this:
 *
 *   args[0] ---> "ls"
 *   args[1] ---> "-la"
 *   args[2] ---> ".."
 *   args[3] ---> NULL (NULL is a pointer to address 0)
 *
 * @param str {char *} Pointer to the complete command line string.
 * @param args {char **} Pointer to an array of strings. This will hold the result.
 * @param args_count {int *} Pointer to an int that will hold the final args count.
 *
 * @returns A copy of args for convenience.
 */
char **parse_commandline(char *str, char **args, int *args_count)
{
    char *token;
    
    *args_count = 0;

    token = strtok(str, " \t\n\r");

    while (token != NULL && *args_count < MAX_TOKENS - 1) {
        args[(*args_count)++] = token;

        token = strtok(NULL, " \t\n\r");
    }

    args[*args_count] = NULL;

    return args;
}

/**
 * Main
 */
int main(void)
{
    // Holds the command line the user types in
    char commandline[COMMANDLINE_BUFSIZE];

    // Holds the parsed version of the command line
    char *args[MAX_TOKENS];

    // How many command line args the user typed
    int args_count;

    // Shell loops forever (until we tell it to exit)
    while (1) {
        // Print a prompt
        printf("%s", PROMPT);
        fflush(stdout); // Force the line above to print

        // Read input from keyboard
        fgets(commandline, sizeof commandline, stdin);

        // Exit the shell on End-Of-File (CRTL-D)
        if (feof(stdin)) {
            break;
        }

        // Parse input into individual arguments
        parse_commandline(commandline, args, &args_count);

        if (args_count == 0) {
            // If the user entered no commands, do nothing
            continue;
        }

        // Exit the shell if args[0] is the built-in "exit" command
        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        #if DEBUG

        // Some debugging output

        // Print out the parsed command line in args[]
        for (int i = 0; args[i] != NULL; i++) {
            printf("%d: '%s'\n", i, args[i]);
        }

        #endif
        
        /* Add your code for implementing the shell's logic here */

        // cd command needs priority handling, check for and handle it first
        if (strcmp(args[0], "cd") == 0) {
            if (args_count != 2) {
                printf("'cd' usage: cd dirname -or- cd path/dirname\n");
                // continue;
            } else {
                int status = chdir(args[1]);
                if (status == -1) {
                    perror("chdir");
                }
            }
            continue;
        }
        
        // command wasn't cd, proceed with normal forking and execution of command
        int rc = fork();

        if (rc < 0) {
            fprintf(stderr, "Fork failed, exiting program.\n");
            exit(1);
        } else if (rc == 0) {
            int redirect = 0;
            int redirect_index = 0;
            FILE *fd;
            for (int i = 0; i < args_count; i++) {
                if (strcmp(args[i], ">") == 0) {
                    redirect = 1;
                    redirect_index = i;
                    printf("redirect: %d\n", redirect);
                }
            }
            if (redirect) {
                printf("passed the redirect test: %d\n", redirect);
                if (redirect_index == 0) {
                    printf("'>' usage: > (redirect output) must follow at least one other argument.\n");
                } else {
                    printf("*** Found a usable >\n");
                    char *output = strdup(args[redirect_index + 1]);
                    printf("*** Redirect output to %s\n", output);
                    args[redirect_index] = NULL;
                    fd = open(output, "w");
                    dup2(fd, 1);
                }
                redirect = 0;
                redirect_index = 0;
            }

            execvp(args[0], args);
        } else {
            waitpid(rc, NULL, 0);
        };        
    }

    return 0;
}
