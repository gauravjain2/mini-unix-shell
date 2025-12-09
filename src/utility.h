#ifndef UTILITY_H
#define UTILITY_H

const char *get_home();
const char *get_pwd();
const char *get_cwd();
void set_pwd();
void add_cmd_history(const char *cmd);
char *get_cmd_history();

#endif