#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct history
{
    char **lines;
    int size;
    int length;
};

struct history create_history(int initial_size)
{
    struct history h;
    h.size = initial_size;
    h.length = 0;
    h.lines = (char **)malloc(h.size * sizeof(char *));
    return h;
}

void add_to_history(struct history *history, char *line)
{
    if (history->length >= history->size)
    {
        history->lines = (char **)realloc(history->lines, history->size * sizeof(char *) * 2);
        history->size *= 2;
    }
    history->lines[history->length] = line;
    history->length++;
}

void free_history(struct history* history) {
    for (int i=0; i<history->length; i++) {
        free(history->lines[i]);
    }
    free(history->lines);
}