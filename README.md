# simple-shell

A simple basic shell written in C to understand how it works , Supports limited functionality and commands

A shell does three main things in its lifetime.

- **Initialize**: In this step, a typical shell would read and execute its configuration files. These change aspects of the shell’s behavior.
- **Interpret**: Next, the shell reads commands from stdin (which could be interactive, or a file) and executes them.
- **Terminate**: After its commands are executed, the shell executes any shutdown commands, frees up any memory, and terminates.

## Compiling instructions

```shell
gcc src/main.c -o <DESIRED_OUTPUT_FILE_NAME>
```
The desired output file will be an executable  and the shell will open

# TODO

- [] add a Makefile to make lives easier