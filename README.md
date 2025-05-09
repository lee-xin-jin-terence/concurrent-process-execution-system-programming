> This repository is part of an academic coursework authored by Terence Lee.

# Multi-process Execution Using GNU C (System Programming)

This repository contains the solution for an academic coursework in system programming. The project demonstrates the management of multiple processes using C, focusing on forking and executing child processes concurrently.

## Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [Compilation Instructions](#compilation-instructions)
- [Usage Instructions](#usage-instructions)
- [License](#license)

## Overview

This program is a simple demonstration of system programming techniques in C. It simulates the creation of multiple child processes, each running a specific command in parallel. The program uses the `fork()` system call to create child processes and `exec()` to execute commands. It also includes a wait mechanism to synchronize the parent process with the completion of its children.

Key features:
- Creation of multiple child processes using `fork()`.
- Execution of commands using `exec()`.
- Parent process waits for child processes to finish.
- Error handling for invalid commands and process creation issues.

## Project Structure

```
├── main.c (originally named q2.c)
├── README.md
```

- **main.c**: The source code for the system programming project.
- **README.md**: This documentation file.

## Compilation Instructions

To compile the program, you need a C compiler such as `gcc`. Use the following command in the terminal:

```bash
gcc -o main main.c
```

After compiling the program, run it from the command line with the following command:

```bash
sudo ./main
```

The program expects a list of commands as arguments to be executed concurrently. Each command will be executed in a separate child process, and the program will wait for all processes to complete before exiting.

### Example:

```bash
sudo ./main ls pwd whoami
```

This will create three child processes, each running `ls`, `pwd`, and `whoami` concurrently.

## Code Explanation

The code consists of a simple C program that forks a new process for each command provided as arguments. The program waits for each child process to finish before exiting.

- **main.c**: The main file containing the logic for handling multiple child processes.
- **fork()**: The function used to create a child process.
- **execvp()**: The function used to execute the commands in the child process.
- **waitpid()**: The function used to wait for the child processes to complete before the parent exits.


## License

This project is for academic coursework purposes only and is not intended for commercial use.
