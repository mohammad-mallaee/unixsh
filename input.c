#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct command
{
    char **items;
    int length;
    int bg_length;
};

int count_background_commands(char *input);

struct command parse_command(char *input, char delim)
{
    struct command cmd;
    cmd.bg_length = 0;

    char **items = (char **)malloc(8 * sizeof(char *));
    if (items == NULL)
    {
        perror("malloc failed");
        exit(1);
    }
    char seperator[2] = {delim, '\0'};
    char *parsed = strtok(input, seperator);
    int idx = 0;
    while (parsed != NULL)
    {
        items[idx] = parsed;
        idx++;
        parsed = strtok(NULL, seperator);
    }
    items[idx] = NULL;

    cmd.items = items;
    cmd.length = idx;
    return cmd;
}

struct command parse_input(char *input)
{
    int bg_length = count_background_commands(input);
    struct command cmd = parse_command(input, '&');
    cmd.bg_length = bg_length;
    return cmd;
}

int count_background_commands(char *input)
{
    int k = 0;
    char last_char;
    for (int i = 0; input[i] != '\0'; i++)
    {
        if (input[i] != last_char && input[i] == '&')
            k++;
        last_char = input[i];
    }
    return k;
}