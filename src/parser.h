#ifndef PARSER_H
#define PARSER_H

#define MAX_TOKENS 128

typedef struct command
{
  char **argv;
  char *input_redirect;
  char *output_redirect;
  int background;
  int is_exec;
  struct command *pipe_to;
} command_t;

command_t *parse_command(const char *input);
void free_command(command_t *cmd);

#endif
