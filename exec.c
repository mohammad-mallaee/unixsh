#include "unistd.h"
#include "builtin/builtin.h"
#include "input.h"
#include <unistd.h>
#include <stdio.h>
#include "history.h"
#include "string.h"
#include <stdlib.h>
#include <sys/wait.h>

void execute_cmd(struct command parsed_input, int i)
{
    if (i >= parsed_input.length) return;
    struct command pipe_commands = parse_command(parsed_input.items[i], '|');
    int pidx = 0, input_fd = -1;
    pid_t children[pipe_commands.length ? pipe_commands.length : 1];
    int is_bg_cmd = i < parsed_input.bg_length;
    fflush(NULL);
    while (pipe_commands.items[pidx] != NULL)
    {
        struct command cmd = parse_command(pipe_commands.items[pidx], ' ');
        if (!cmd.length) { free(cmd.items); break; }
        if (pipe_commands.length == 1 && strcmp(cmd.items[0], "exit") == 0)
        {
            free(cmd.items);
            exit(0);
        }

        int fd[2], has_next = pipe_commands.items[pidx + 1] != NULL;
        if (has_next && pipe(fd) < 0)
        {
            perror("pipe failed");
            exit(1);
        }
        pid_t child_pid = fork();
        if (child_pid < 0)
        {
            perror("fork failed");
            exit(1);
        }

        if (child_pid == 0)
        {
            if ((input_fd != -1 && dup2(input_fd, STDIN_FILENO) < 0) ||
                (has_next && dup2(fd[1], STDOUT_FILENO) < 0))
            {
                perror("dup2 failed");
                exit(1);
            }
            if (input_fd != -1) close(input_fd);
            if (has_next) { close(fd[0]); close(fd[1]); }
            if (strcmp(cmd.items[0], "exit") == 0) exit(0);
            struct builtin *builtin_ptr = get_builtin(cmd.items[0]);
            if (builtin_ptr != NULL)
            {
                if (builtin_ptr->function(cmd.items) < 0)
                {
                    perror(cmd.items[1]);
                }
            }
            else if (execvp(cmd.items[0], cmd.items) < 0)
            {
                perror(cmd.items[0]);
                exit(1);
            }
            exit(0);
        }
        else
        {
            children[pidx] = child_pid;
            if (input_fd != -1) close(input_fd);
            if (has_next) close(fd[1]);
            input_fd = has_next ? fd[0] : -1;
            free(cmd.items);
        }
        pidx++;
    }
    if (input_fd != -1) close(input_fd);
    if (!is_bg_cmd)
        for (int j = 0; j < pidx; j++)
            waitpid(children[j], NULL, WUNTRACED);
    free(pipe_commands.items);
    if (i < parsed_input.length - 1)
        execute_cmd(parsed_input, i + 1);
}
