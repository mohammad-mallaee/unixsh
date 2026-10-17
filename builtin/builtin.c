#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "carlo.h"
#include "sudoku.h"

int cd(char **command);
int help(char **command);
int pwd(char **command);

struct builtin
{
    char *command;
    int (*function)(char **);
};

struct builtin commands[6] = {
    {"cd", cd},
    {"help", help},
    {"pwd", pwd},
    {"sudoku", sudoku_validator},
    {"carlo", mont_carlo},
    {NULL, NULL},
};

struct builtin *get_builtin(char *name)
{
    struct builtin *builtin_ptr = commands;
    while (builtin_ptr->command != NULL)
    {
        if (strcmp(builtin_ptr->command, name) == 0)
        {
            return builtin_ptr;
        }
        builtin_ptr++;
    }
    return NULL;
}

int cd(char **command)
{
    return chdir(command[1]);
}

int pwd(char **command)
{
    char buf[1000];
    char *a = getwd(buf);
    if (a == NULL)
        return 1;
    printf("%s\n", buf);
    return 0;
}

int help(char **command)
{
    printf("-- help\n");
    printf("This shell is the best thing that ever happened to humanity. \n");
    printf("-- \n");
    return 0;
}