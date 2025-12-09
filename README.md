# Mini UNIX Shell

A lightweight, custom UNIX shell implementation in C.

## Features

- **Command Execution**: Execute standard external commands (e.g., `ls`, `grep`, `sleep`).
- **Built-in Commands**:
  - `cd [directory]`: Change the current working directory.
  - `exit`: Exit the shell.
- **I/O Redirection**:
  - Input redirection: `command < input_file`
  - Output redirection: `command > output_file`
- **Pipelines**: Support for multistage pipelines (e.g., `cmd1 | cmd2 | cmd3`).
- **Background Execution**: Run commands in the background using `&`.

## System Requirements

- **C Compiler**: GCC (GNU Compiler Collection) or Clang.
- **Build Tool**: GNU Make.
- **Operating System**: A POSIX-compliant operating system (Linux, macOS, or Windows via WSL).

## Installation & Running

### Linux

1.  **Install dependencies** (Ubuntu/Debian example):
    ```bash
    sudo apt update
    sudo apt install build-essential
    ```
2.  **Build**:
    ```bash
    make
    ```
3.  **Run**:
    ```bash
    ./shell
    ```

### macOS

1.  **Install Command Line Tools** (if not already installed):
    ```bash
    xcode-select --install
    ```
2.  **Build**:
    ```bash
    make
    ```
3.  **Run**:
    ```bash
    ./shell
    ```

### Windows

**Note**: This shell relies on POSIX system calls (like `fork()`, `pipe()`, `execvp()`) which are not natively available in the Windows kernel. You **must** use the Windows Subsystem for Linux (WSL).

1.  **Install WSL**:
    Open PowerShell as Administrator and run:
    ```powershell
    wsl --install
    ```
    Restart your computer if prompted.
2.  **Open WSL Terminal**:
    Launch "Ubuntu" (or your installed distribution) from the Start menu.
3.  **Install dependencies**:
    ```bash
    sudo apt update
    sudo apt install build-essential
    ```
4.  **Build and Run**:
    Navigate to the project directory and run:
    ```bash
    make
    ./shell
    ```

## Usage

Start the shell by running the executable:

```bash
./shell
```

You will see the shell prompt (e.g., `mini-shell> `). You can now type commands.

### Examples

**Simple Command:**
```bash
mini-shell> ls -l
```

**Change Directory:**
```bash
mini-shell> cd src
```

**Input/Output Redirection:**
```bash
mini-shell> ls > filelist.txt
mini-shell> sort < filelist.txt
```

**Pipelines:**
Combine multiple commands. The output of one command serves as the input to the next.
```bash
mini-shell> ls -l | grep ".c" | wc -l
```

**Background Execution:**
Run a command without blocking the shell.
```bash
mini-shell> sleep 10 &
```

## Implementation Details

The shell is built using a modular approach:
- **Parser**: Tokenizes input and builds a command structure.
- **Executor**: Handles process creation (`fork`), execution (`execvp`), pipes, and redirection.
- **Builtins**: Handles internal shell commands.
