#include "builtins.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * run_builtin - Execute a shell builtin command if applicable.
 *
 * @cmd: Pointer to a command_t describing the parsed command.
 *
 * Return: 1 if a builtin was recognized and handled (or attempted to be
 *         handled), 0 if no builtin was handled or if input is invalid.
 *
 * Description:
 * This function checks whether the provided command corresponds to a shell
 * builtin and, if so, performs the builtin action in the current process.
 */
int run_builtin(command_t *cmd)
{
  if (!cmd || cmd->is_exec || !cmd->argv[0])
    return 0;

  // exit
  if (strcmp(cmd->argv[0], "exit") == 0)
  {
    exit(0);
  }

  // cd
  if (strcmp(cmd->argv[0], "cd") == 0)
  {
    const char *path = cmd->argv[1] ? cmd->argv[1] : get_home();
    if (chdir(path) != 0)
    {
      fprintf(stderr, "cd: No such file or directory: %s\n", path);
    }
    set_pwd();
    return 1;
  }

  return 0;
}
