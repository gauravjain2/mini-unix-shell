#include "parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * alloc_cmd
 *
 * Allocate and initialize a new command_t structure.
 *
 * Returns:
 *   A pointer to a zero-initialized command_t on success.
 *   The caller owns the returned pointer and must free it and its nested
 *   allocations using free_command().
 */
static command_t *alloc_cmd()
{
  command_t *cmd = calloc(1, sizeof(command_t));
  cmd->argv = calloc(MAX_TOKENS, sizeof(char *));
  return cmd;
}

/**
 * copy_token
 *
 * Create a null-terminated heap-allocated copy of a substring.
 *
 * Parameters:
 *   start - pointer to the first character of the token substring.
 *   len   - length of the substring to copy (number of bytes).
 *
 * Returns:
 *   A newly allocated char* containing the copied characters followed by a
 *   terminating '\0'. The caller is responsible for freeing the returned
 *   string with free().
 */
static char *copy_token(const char *start, int len)
{
  char *t = malloc(len + 1);
  strncpy(t, start, len);
  t[len] = '\0';
  return t;
}

/**
 * free_tokens
 *
 * Free a NULL terminated array of strings previously returned by tokenize().
 *
 * Parameters:
 *   tokens - NULL terminated array of heap-allocated char* pointers.
 *
 * Behavior:
 *   - Frees each non-NULL string tokens[i] and then frees the array itself.
 *   - Safe to call with tokens == NULL (no-op).
 */
static void free_tokens(char **tokens)
{
  for (int i = 0; tokens[i]; i++)
  {
    free(tokens[i]);
  }
  free(tokens);
}

/**
 * parse_tokens
 *
 * Convert a NULL-terminated array of tokens into a linked command_t structure.
 *
 * Parameters:
 *   tokens - NULL-terminated array of token strings.
 *
 * Returns:
 *   A pointer to the head of a command_t structure representing the parsed
 *   command pipeline. Memory for command nodes and duplicated strings is
 *   allocated by parse_tokens and must be freed using free_command().
 */
static command_t *parse_tokens(char **tokens)
{
  command_t *cmd = alloc_cmd();
  int argc = 0;

  command_t *cur = cmd;

  for (int i = 0; tokens[i]; i++)
  {
    char *t = tokens[i];

    if (t == NULL)
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
      cur->argv[argc] = NULL;
      cur->pipe_to = alloc_cmd();
      cur = cur->pipe_to;
      argc = 0;
    }
    else
    {
      cur->argv[argc++] = strdup(t);
    }
  }
  cur->argv[argc] = NULL;

  return cmd;
}

/**
 * set_exec
 *
 * Mark whether a command should be executed via exec (external) or is a
 * builtin that should be handled internally.
 *
 * Parameters:
 *   cmd - pointer to a command_t whose argv[0] names the command.
 *
 * Behavior:
 *   - Sets cmd->is_exec to 0 for builtins "cd" and "exit", otherwise sets
 *     cmd->is_exec to 1.
 */
static void set_exec(command_t *cmd)
{
  if (strcmp(cmd->argv[0], "cd") == 0 || strcmp(cmd->argv[0], "exit") == 0 || strcmp(cmd->argv[0], "history") == 0)
    cmd->is_exec = 0;
  else
    cmd->is_exec = 1;
}

/**
 * tokenize
 *
 * Split an input string into a NULL-terminated array of tokens suitable for
 * parse_tokens().
 *
 * Parameters:
 *   input - NULL terminated input command line string.
 *
 * Returns:
 *   A freshly allocated NULL-terminated array of heap-allocated strings.
 *   Each token is heap-allocated and the array itself is heap-allocated.
 *   The caller must free the result with free_tokens().
 */
static char **tokenize(const char *input)
{
  char **tokens = calloc(MAX_TOKENS, sizeof(char *));
  int t = 0;

  int i = 0, n = strlen(input);

  while (i < n)
  {
    while (isspace(input[i]))
      i++;

    if (i == n)
      break;

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
      {
        i++;
      }
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

/**
 * free_command
 *
 * Recursively free a command_t structure and all resources it owns.
 *
 * Parameters:
 *   cmd - pointer to the command_t to free (may be NULL).
 *
 * Behavior:
 *   - Frees argv strings, the argv array, input_redirect, output_redirect.
 *   - Recursively frees cmd->pipe_to (if non-NULL).
 *   - Finally frees the command_t itself.
 */
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

/**
 * parse_command
 *
 * High-level helper: tokenize an input string and parse it into a
 * command_t pipeline structure.
 *
 * Parameters:
 *   input - NULL terminated input command line string.
 *
 * Returns:
 *   A pointer to the parsed command_t structure (head of pipeline). The
 *   caller owns the returned pointer and must free it with free_command().
 */
command_t *parse_command(const char *input)
{
  char **tokens = tokenize(input);
  command_t *cmd = parse_tokens(tokens);

  command_t *cur = cmd;
  while (cur)
  {
    set_exec(cur);
    cur = cur->pipe_to;
  }

  free_tokens(tokens);
  return cmd;
}
