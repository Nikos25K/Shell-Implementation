# Shell Implementation

## Introduction

This project was an assignment of the course "System Programming" in National and Kapodistrian University of Athens. <br>
In this project, we are tasked with creating a simple shell that uses various components to handle different aspects of shell functionality, providing a robust and flexible command-line interface for users. This shell supports:
- Command execution
- History management
- Alias management
- Wildcard expansion
- Input/Output Redirections
- Pipelines
- Background execution
- Ctrl-C and Ctrl-Z signal management



## Compilation and Execution

To compile the program, a Makefile is provided which automates the build process. The Makefile includes various targets to compile the source files, create necessary directories, and build the executable. To compile the program, simply run the following command in the terminal:

```sh
make
```

This will generate the executable file in the `./bin` directory and the object files in the `./build` directory. To run the shell, use the following command:

```sh
make run
```

Additionally, you can use `make valgrind` to run the program with Valgrind for memory leak detection. To clean up the compiled files and the executable, use:

```sh
make clean
```

For more information on available targets, you can use:

```sh
make help
```

## Components

- **Main Program (`main.c`)**: Initializes the shell, sets up signal handling, and enters a loop to read and execute commands.
- **History Management (`history.c`)**: Manages the history of executed commands.
- **Signal Handling (`signals.c`)**: Handles signals like SIGINT and SIGTSTP.
- **Utility Functions (`utils.c`)**: Provides utility functions like string duplication, memory allocation checks, and command parsing.
- **Alias Management (`aliases.c`)**: Manages command aliases.
- **Command Execution (`execute.c`)**: Executes commands, including custom commands and pipelines.
- **Simple Commands (`simple_commands.c`)**: Implements simple built-in commands like `cd` and `ver`.
- **Wildcard Expansion (`wildcards.c`)**: Expands wildcards in command arguments.
- **Redirection (`redirect.c`)**: Handles input and output redirection.

## Actions and How It Works

### Initialization

- The main function initializes signal handling and creates data structures for history and aliases.
- It then enters an infinite loop to read and execute commands.

### Reading Commands

- The `read_args` function reads a command from the user or an alias and splits it into arguments.
- It checks for wildcards and expands them if necessary.

### Handling Aliases

- The program checks if the first argument is an alias and replaces it with the corresponding command if found.

### Executing Commands

- The `execute_custom` function checks if the command is a built-in command (e.g., `cd`, `ver`, `createalias`, `destroyalias`, `aliases`, `history`) and executes it if so.
- If the command is not a built-in command, the program proceeds to execute it as a regular command or a pipeline of commands.

### Pipelines and Redirection

- The `execute_pipeline` function handles commands with pipes (`|`), setting up the necessary file descriptors and calling the helper function to execute each stage of the pipeline.
- The `redirection` function handles input and output redirection (`<`, `>`, `>>`).

### Signal Handling

- The `catchinterrupt` function handles signals like `SIGINT` and `SIGTSTP`, forwarding them to the currently running process.

### History Management

- The `add_history` function adds executed commands to the history.
- The `print_history` function prints the command history.
- The `manage_history` function allows executing commands from the history.

### Alias Management

- The `create_alias` function creates a new alias.
- The `destroy_alias` function removes an alias.
- The `print_aliases` function prints all aliases.


## Example Workflow

### User Input

The user types a command, e.g., `ls -l | grep myfile > output.txt`.

### Command Parsing

The command is read and split into arguments: `["ls", "-l", "|", "grep", "myfile", ">", "output.txt"]`.

### Alias Replacement

The program checks if `ls` is an alias and replaces it if necessary.

### Pipeline and Redirection Handling

- The program detects the pipe (`|`) and sets up the necessary file descriptors.
- It also detects the output redirection (`>`) and sets up the file descriptor for writing to `output.txt`.

### Command Execution

- The `ls -l` command is executed, and its output is piped to the `grep myfile` command.
- The output of `grep myfile` is redirected to `output.txt`.

### History Update

The command is added to the history.

### Signal Handling

If the user presses `Ctrl+C` or `Ctrl+Z`, the signal is caught and forwarded to the currently running process.
