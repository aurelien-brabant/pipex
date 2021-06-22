# Pipex

The `pipex` project is an introduction to UNIX pipes, that are a way to
to communicate between distinct processes.

It is usually done to prepare a much bigger 42 project which is [minishell](https://github.com/aurelien-brabant/minishell).

## Features

The `pipex` program should be used using the following syntax:
```bash
./pipex [here_doc] <in_arg> <cmd1> ... <cmdn> <out_arg>
```

### Normal mode

In normal mode, the optional `here_doc` parameter should NOT be present.

The `<in_arg>` argument is the name of the file which content will be used
as input for the FIRST command.

The `<out_arg>` argument is the name of the file that will hold the output of
the LAST command. This file will be created if it doesn't exist.

Between these two special arguments, an unlimited list of commands can be
provided. Each command's output will be used as input by the next command,
using the pipe mechanism. You can think of it like if every command was
separated from the next by a `|` (pipe) symbol.
