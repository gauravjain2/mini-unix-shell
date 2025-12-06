#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static command_t *alloc_cmd()
{
  command_t *cmd = calloc(1, sizeof(command_t));
  cmd->argv = calloc(MAX_TOKENS, sizeof(char *));
  return cmd;
}

static char *copy_token(const char *start, int len)
{
  char *t = malloc(len + 1);
  strncpy(t, start, len);
  t[len] = '\0';
  return t;
}

static void free_tokens(char **tokens)
{
  for (int i = 0; tokens[i]; i++)
  {
    free(tokens[i]);
  }
  free(tokens);
}

static command_t *parse_tokens(char **tokens)
{
  command_t *cmd = alloc_cmd();
  int argc = 0;

  command_t *cur = cmd;

  for (int i = 0; tokens[i]; i++)
  {
    char *t = tokens[i];

    if (tokens[i] == NULL)
    {
      break;
    }

    if (strcmp(t, "<") == 0 && tokens[i + 1])
    {
      cur->input_redirect = strdup(tokens[++i]);
    }
    else if (strcmp(t, ">") == 0 && tokens[i + 1])
    {
      cur->output_redirect = strdup(tokens[++i]);
    }
    else if (strcmp(t, "&") == 0)
    {
      cur->background = 1;
    }
    else if (strcmp(t, "|") == 0)
    {
      cur->pipe_to = alloc_cmd();
      cur = cur->pipe_to;
      argc = 0;
    }
    else
    {
      cur->argv[argc++] = strdup(t);
    }
  }

  return cmd;
}

static void set_exec(command_t *cmd)
{
  if (strcmp(cmd->argv[0], "cd") == 0 || strcmp(cmd->argv[0], "exit") == 0)
    cmd->is_exec = 0;
  else
    cmd->is_exec = 1;
}

static char **tokenize(const char *input)
{
  char **tokens = calloc(MAX_TOKENS, sizeof(char *));
  int t = 0;

  int i = 0, n = strlen(input);

  while (i < n)
  {
    while (isspace(input[i]))
      i++;

    // special operators
    if (input[i] == '&' || input[i] == '|' || input[i] == '<' || input[i] == '>')
    {
      tokens[t++] = copy_token(&input[i], 1);
      i++;
      continue;
    }

    // quoted string
    if (input[i] == '"')
    {
      int start = ++i;
      while (i < n && input[i] != '"')
        i++;
      tokens[t++] = copy_token(input + start, i - start);
      i++;
      continue;
    }

    // normal word
    int start = i;
    while (i < n &&
           !isspace(input[i]) &&
           input[i] != '&' &&
           input[i] != '|' &&
           input[i] != '<' &&
           input[i] != '>')
      i++;

    tokens[t++] = copy_token(input + start, i - start);
  }

  tokens[t] = NULL;
  return tokens;
}

void free_command(command_t *cmd)
{
  if (!cmd)
    return;

  for (int i = 0; cmd->argv && cmd->argv[i]; i++)
    free(cmd->argv[i]);

  free(cmd->argv);
  free(cmd->input_redirect);
  free(cmd->output_redirect);

  if (cmd->pipe_to)
    free_command(cmd->pipe_to);

  free(cmd);
}

command_t *parse_command(const char *input)
{
  char **tokens = tokenize(input);
  command_t *cmd = parse_tokens(tokens);

  set_exec(cmd);
  if (cmd->pipe_to)
  {
    set_exec(cmd->pipe_to);
  }

  free_tokens(tokens);
  return cmd;
}
