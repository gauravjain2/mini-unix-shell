#include "utility.h"

#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

static char cwd[256] = "";
static char home[PATH_MAX] = "";
static char pwd[PATH_MAX] = "";

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
