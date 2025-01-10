# Ongoing project

## Parsing
- Lexing input into typed tokens
- Parse typed tokens into data structures
- Expand variables
- Implement Here_docs

## Execution
- Executes commands
- Redirects streams
- Maintain history of commands

## Interactivity
- Prompts user
- Ctrl + C interrupts command
- Ctrl + D exits
- Ctrl + / nothing

# Example
> `echo -n "hello and goodbye" > test.txt | ls -a`

### 1 - Lexing
- identifies `echo` `-n` `test.txt` `ls` and `-a` as `t_word`
- identifier `"` as `t_delimiter`
- identifies `"hello and goodbye"` as `t_string`
- identifies `>` and `|` as `t_operator`
- identifies the end of the command line as `t_eof`


### 2 - Parsing
- identifies first `t_word` of command as `t_cmd`
  >`echo` = command
- identifies `t_word` following a command as `t_arg`
  >`-n` = argument of command `echo`

  >`"hello"` = argument of command `echo`
- identifies `>` and the following `t_word` as `t_redirection`
  >`>` = redirection of command

  >`test.txt` = target filepath of the output of command `echo`

Starts again after a pipe:
- identifies first `t_word` after a `|` `t_operator` as `t_cmd`
  >`ls` = command
- identifies `t_word` following a command as `t_arg`
  >`-a` = argument of command `ls`

Loads everything into a data structure to send to execution

### 3 - Execution




### 4 - Signals

