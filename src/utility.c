#include "utility.h"

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

static char cwd[256] = "";
static char home[PATH_MAX] = "";
static char pwd[PATH_MAX] = "";
static char history_path[PATH_MAX] = "";

/**
 * get_cwd
 *
 * Return the basename of current working directory name.
 *
 * Returns:
 *   const char * - pointer to a NULL-terminated string containing the basename of
 *                  the current working directory.
 */
const char *get_cwd()
{
  if (cwd[0] == '\0')
  {
    set_pwd();
  }
  return cwd;
}

/**
 * get_home
 *
 * Return the HOME directory string for the current user.
 *
 * Returns:
 *   const char * - pointer to a NULL-terminated string containing the HOME
 *                  directory, or NULL if the HOME environment variable is not set.
 */
const char *get_home()
{
  if (home[0] == '\0')
  {
    char *env_home = getenv("HOME");
    strncpy(home, env_home, sizeof(home) - 1);
    home[sizeof(home) - 1] = '\0';
  }
  return home;
}

/**
 * get_pwd
 *
 * Return the absolute pathname of the current working directory.
 *
 * Returns:
 *   const char * - pointer to a NULL-terminated string containing the absolute
 *                  pathname of the current working directory, or NULL if the
 *                  value cannot be determined and no fallback is available.
 */
const char *get_pwd()
{
  if (pwd[0] == '\0')
  {
    set_pwd();
  }
  return pwd;
}

/**
 * set_pwd
 *
 * Populate the module's internal pathname buffers for the current working
 * directory.
 *
 * Behavior:
 *   - Attempts to obtain the absolute pathname via getcwd() into the internal
 *     pwd buffer.
 *   - If getcwd() fails, falls back to the user's HOME directory (via
 *     get_home()).
 *   - Updates the cwd to point to the basename of the resolved pwd.
 */
void set_pwd()
{
  if (getcwd(pwd, sizeof(pwd)) == NULL)
  {
    strncpy(pwd, get_home(), sizeof(pwd) - 1);
    pwd[sizeof(pwd) - 1] = '\0';
  }

  strncpy(cwd, basename(pwd), sizeof(cwd) - 1);
  cwd[sizeof(cwd) - 1] = '\0';
}

/**
 * @brief Adds a command to the command history.
 *
 * @param cmd A pointer to a null-terminated string containing the command to add.
 *
 * @return void
 */
void add_cmd_history(const char *cmd)
{
  if (history_path[0] == '\0')
  {
    const char *home = get_home();
    snprintf(history_path, sizeof(history_path), "%s/.shell_history", home);
  }

  FILE *history_file = fopen(history_path, "a");
  if (history_file)
  {
    fprintf(history_file, "%s", cmd);
    fclose(history_file);
  }
}

char *get_cmd_history()
{
  if (history_path[0] == '\0')
  {
    const char *home = get_home();
    snprintf(history_path, sizeof(history_path), "%s/.shell_history", home);
  }
  FILE *fptr = fopen(history_path, "r");
  if (!fptr)
  {
    return NULL;
  }

  char *lines[10] = {NULL};
  int line_count = 0;
  int current_idx = 0;
  char *line_buf = NULL;
  size_t line_len = 0;
  ssize_t read_bytes;

  while ((read_bytes = getline(&line_buf, &line_len, fptr)) != -1)
  {
    if (lines[current_idx] != NULL)
    {
      free(lines[current_idx]);
    }
    lines[current_idx] = strdup(line_buf);
    if (lines[current_idx] == NULL)
    {
      // Handle allocation error
      for (int i = 0; i < 10; ++i)
      {
        if (lines[i] != NULL)
        {
          free(lines[i]);
        }
      }
      free(line_buf);
      fclose(fptr);
      return NULL;
    }
    current_idx = (current_idx + 1) % 10;
    if (line_count < 10)
    {
      line_count++;
    }
  }

  free(line_buf);
  fclose(fptr);

  // Calculate total length for the result string
  size_t total_length = 0;
  int start_idx = (line_count < 10) ? 0 : current_idx;
  for (int i = 0; i < line_count; ++i)
  {
    total_length += strlen(lines[(start_idx + i) % 10]);
  }

  char *result = malloc(total_length + 1); // +1 for null terminator
  if (result == NULL)
  {
    for (int i = 0; i < 10; ++i)
    {
      if (lines[i] != NULL)
      {
        free(lines[i]);
      }
    }
    return NULL;
  }
  result[0] = '\0'; // Initialize as empty string

  for (int i = 0; i < line_count; ++i)
  {
    strcat(result, lines[(start_idx + i) % 10]);
    free(lines[(start_idx + i) % 10]); // Free memory after copying
  }

  return result;
}