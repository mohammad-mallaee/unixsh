#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include "input.h"
#include "history.h"
#include "string.h"
#include "exec.h"

int main()
{
    char *input;
    struct history history = create_history(5);
    struct command parsed_input;

    while (1)
    {
        input = readline("uinxsh> ");
        if (strcmp(input, "!!") == 0)
        {
            if (history.length == 0)
            {
                printf("There is no history!!\n");
                continue;
            }
            else
            {
                printf("[!!]: %s\n", history.lines[history.length - 1]);
                input = strdup(history.lines[history.length - 1]);
            }
        }
        char *input_dup = strdup(input);
        add_to_history(&history, input_dup);
        parsed_input = parse_input(input);
        execute_cmd(parsed_input, 0);
    }

    free_history(&history);
    return 0;
}