# Mini Shell in C

## Description
A command-line Mini Shell developed in C that simulates basic Linux shell functionality. It supports built-in commands, execution of external commands, piping, background processes, and job control using process management and signal handling.

## Features
- Execute built-in commands such as `cd`, `pwd`, `echo`, and `exit`.
- Execute external Linux commands using process creation and `execvp()`.
- Support pipelines using the `|` operator.
- Run commands in the background using `&`.
- Handle `Ctrl+C` and `Ctrl+Z` signals.
- Maintain a list of running and stopped jobs.
- Support job control using `jobs`, `fg`, and `bg`.
- Display the last command's exit status using `echo $?`.
- Display the shell process ID using `echo $$`.
- Customize the shell prompt using `PS1=`.

## Technologies Used
- **Programming Language:** C
- **Platform:** Linux
- **Compiler:** GCC
- **System Calls:** `fork()`, `execvp()`, `waitpid()`, `pipe()`, `dup2()`, `kill()`, `chdir()`
- **Concepts:** Process Management, Inter-Process Communication (IPC), Signals, File Descriptors, Linked Lists

## Project Structure
- `main.c` – Initializes the shell and loads external commands.
- `minishell.c` – Implements command classification, built-in commands, external command execution, pipelines, and job control.
- `minishell.h` – Contains macros, structures, and function declarations.
- `scan_input.c` – Reads user input, handles signals, and manages foreground and background processes.
- `external.txt` – Contains the list of supported external commands.

## Compilation
Run the following command from the `MiniShell` directory:

```bash
gcc main.c minishell.c scan_input.c -o minishell
```

## Execution

```bash
./minishell
```

## Sample Commands

```bash
pwd
cd ..
echo Hello World
ls -l
ls | wc -l
sleep 10 &
jobs
fg
bg
echo $$
echo $?
exit
```

## Concepts Learned
- Process creation and execution in Linux.
- Inter-process communication using pipes.
- Signal handling with `SIGINT`, `SIGTSTP`, and `SIGCHLD`.
- Foreground and background process management.
- Dynamic memory allocation and linked-list-based job tracking.
- Linux system calls and file descriptor redirection.

## Author
Soundharya Lahari Uppula
