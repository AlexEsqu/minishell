# Ongoing project

## Parsing
- Lexing input into typed tokens
- Parse input into data structures

## Execution
- Prompts user
- Executes commands
- Redirects streams
- Maintain history of commands

## Interactivity
- Ctrl + C interrupts command
- Ctrl + D exits
- Ctrl + / nothing

# Example
> `echo -n "hello and goodbye" > test.txt | ls -a`

### 1) Lexing
- identifies `echo` `-n` `test.txt` `ls` and `-a` as `t_word`
- identifier `"` as `t_delimiter`
- identifies `"hello and goodbye"` as `t_string`
- identifies `>` as `t_operator`
- identifies `|` as `t_pipe`
- identifies the end of the command line as `t_eof`


### 2) Parsing
- identifies first `t_word` of command as `t_cmd`
  >`echo` = command
- identifies `t_word` following a command and begining with `-` as `t_option`
  >`-n` = option of command `echo`
- identifies `t_word` following a command OR its options without `-` as `t_arg`
  >`"hello"` = argument of command `echo`
- identifies `>` and the following `t_word` as `t_redirection`
  >`>` = redirection of command
  
  >`test.txt` = outfilepath of command `echo`
  
Starts again after a pipe:
- identifies first `t_word` after a `t_pipe` as `t_cmd`
  >`ls` = command
- identifies `t_word` following a command and begining with `-` as `t_option`
  >`-a` = option of command `ls`
  
Loads everything into a data structure to send to execution



