#ifndef SHELL_H_
#define SHELL_H_

int shell(char *input);
int count_arguments(char *input);
void parse_arguments(char *input, char **parsed_input);
void wait_for_all_children();
void cd(char *path);

#endif
