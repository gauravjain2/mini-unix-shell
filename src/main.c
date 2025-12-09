#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>

#include "parser.h"
#include "builtins.h"
#include "utility.h"
#include "executor.h"

/**
 * sigchld_handler - Signal handler for SIGCHLD
 * @sig: Signal number (unused)
 *
 * Description:
 * Reaps all terminated child processes to prevent them from becoming
 * zombies. Uses WNOHANG flag to non-blocking wait for any child process.
 *
 * Return: void
 */
void sigchld_handler(int sig)
{
  (void)sig;
  // Reap all dead children
  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;
}

/**
 * main - Main entry point for the mini Unix shell
 *
 * Description:
 * Initializes the shell by setting up signal handlers to ignore SIGINT
 * (Ctrl-C) and handle SIGCHLD for reaping zombie processes. Enters an
 * infinite loop to continuously read and process user commands. Maintains
 * the current working directory and displays it in the shell prompt.
 *
 * Return: 0 on successful execution, non-zero on error
 */
int main()
{
  // Shell ignores Ctrl-C
  signal(SIGINT, SIG_IGN);

  // Reap zombies
  signal(SIGCHLD, sigchld_handler);

  char line[1024];

  while (1)
  {
    printf("shell %s > ", get_cwd());
    fflush(stdout);

    if (!fgets(line, sizeof(line), stdin))
      break;

    if (line[0] == '\n')
      continue;

    command_t *cmd = parse_command(line);

    if (cmd->is_exec)
    {
      int status = execute_command(cmd);
      if (status == 0)
      {
        printf("Error occurred while executing the command\n");
      }
    }
    else
    {
      run_builtin(cmd);
    }

    free_command(cmd);
    add_cmd_history(line);
  }

  return 0;
}
