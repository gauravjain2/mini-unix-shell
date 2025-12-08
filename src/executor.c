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
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return;
    }

    pid_t left = fork();
    if (left == 0) {
        signal(SIGINT, SIG_DFL);

        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        setup_redirection(cmd);
        execvp(cmd->argv[0], cmd->argv);
        perror("execvp");
        exit(1);
    }

    pid_t right = fork();
    if (right == 0) {
        signal(SIGINT, SIG_DFL);

        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        setup_redirection(cmd->pipe_to);
        execvp(cmd->pipe_to->argv[0], cmd->pipe_to->argv);
        perror("execvp");
        exit(1);
    }

    // parent closes
    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(left, NULL, 0);
    waitpid(right, NULL, 0);
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
