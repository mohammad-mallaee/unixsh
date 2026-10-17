struct command {
    char **items;
    int length;
    int bg_length;
};

struct command parse_input(char *input);
struct command parse_command(char *input, char delim);
int count_background_commands(char *input);