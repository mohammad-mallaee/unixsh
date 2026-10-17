#ifndef builtin_h
#define builtin_h

struct builtin
{
    char *command;
    int (*function)(char **);
};

struct builtin *get_builtin(char *);

int cd(char **);
int help(char **);

#endif