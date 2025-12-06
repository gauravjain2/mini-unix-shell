#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "builtins.h"

int run_builtin(command_t *cmd)
{
  if (!cmd || cmd->is_exec || !cmd->argv[0])
    return 0;

  // exit
  if (strcmp(cmd->argv[0], "exit") == 0)
  {
    exit(0);
    return 1;
  }

  // cd
  if (strcmp(cmd->argv[0], "cd") == 0)
  {
    const char *path = cmd->argv[1] ? cmd->argv[1] : getenv("HOME");
    if (chdir(path) != 0)
      perror("cd");
    return 1;
  }

  return 0;
}
