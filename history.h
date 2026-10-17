struct history
{
    char **lines;
    int size;
    int length;
};

struct history create_history(int initial_size);
void add_to_history(struct history *history, char *line);
void free_history(struct history* history);