#include "executor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#include "parser.h"
#include "utility.h"
#include "builtins.h"

// Forward declaration
static void setup_redirection(command_t *cmd);

// -----------------------------------------------------------
// Pipeline (cmd1 | cmd2)
// -----------------------------------------------------------
static void execute_pipeline(command_t *cmd) {
    int prev_pipefd[2] = {-1, -1};
    int pipefd[2] = {-1, -1};
    pid_t pid;
    command_t *cur = cmd;
    
    // Track all children to wait for them later
    int children_count = 0;

    while (cur) {
        if (cur->pipe_to) {
            if (pipe(pipefd) < 0) {
                perror("pipe");
                return; // TODO: better error handling cleanup
            }
        }

        pid = fork();
        if (pid < 0) {
            perror("fork");
            return;
        }

        if (pid == 0) {
            signal(SIGINT, SIG_DFL);

            // If there is a previous pipe, read from it
            if (prev_pipefd[0] != -1) {
                dup2(prev_pipefd[0], STDIN_FILENO);
                close(prev_pipefd[0]);
                close(prev_pipefd[1]);
            }

            // If there is a next command, write to the current pipe
            if (cur->pipe_to) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            setup_redirection(cur);
            execvp(cur->argv[0], cur->argv);
            perror("execvp");
            exit(1);
        } else {
            // Parent process
            children_count++;

            // Close previous pipe ends as they are no longer needed by parent
            if (prev_pipefd[0] != -1) {
                close(prev_pipefd[0]);
                close(prev_pipefd[1]);
            }

            // If there is a next command, current pipe becomes previous pipe
            if (cur->pipe_to) {
                prev_pipefd[0] = pipefd[0];
                prev_pipefd[1] = pipefd[1];
            }
        }

        cur = cur->pipe_to;
    }

    // Wait for all children
    for (int i = 0; i < children_count; i++) {
        wait(NULL);
    }
}

// -----------------------------------------------------------
// Redirection setup
// -----------------------------------------------------------
static void setup_redirection(command_t *cmd) {
    if (cmd->input_redirect) {
        int fd = open(cmd->input_redirect, O_RDONLY);
        if (fd < 0) {
            perror("open");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (cmd->output_redirect) {
        int fd = open(cmd->output_redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror("open");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

// -----------------------------------------------------------
// Simple command execution (no pipeline)
// -----------------------------------------------------------
static void execute_simple(command_t *cmd) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        // Children restore default SIGINT behavior
        signal(SIGINT, SIG_DFL);

        setup_redirection(cmd);
        execvp(cmd->argv[0], cmd->argv);
        perror("execvp");
        exit(1);
    }

    if (cmd->background) {
        printf("[bg] started PID %d\n", pid);
        return;
    }

    waitpid(pid, NULL, 0);
}

int execute_command(command_t *cmd) {
    if (!cmd || !cmd->argv[0]) return 0;

    if (cmd->pipe_to) {
        if (cmd->background) {
            printf("Warning: pipeline background execution not supported.\n");
            return 0;
        }
        execute_pipeline(cmd);
        return 1;
    } else {
        execute_simple(cmd);
        return 1;
    }
    return 0;
}
