# OSAssignment2
*Spawning Processes*

## Task

Create a C++ program, called OSShell, that will serve as a shell interface that accepts user commands, then executes each command in a separate process.

### *OSShell*

  -  Create a shell interface (program that gives the user a prompt for a command)
     - Search the directories in the PATH environment variable and find the first executable with the name of the entered command
       - If one is found, spawn a new process to run that executable, and wait for its completion
       - If not found, print the following error message:
        "<command>: Error running command"
     - Just hitting enter without typing a command should not print an error
     - After a command executes (or no command is found and error printed, or user simply hit enter), the shell interface should prompt the user for the next command
     - Also, on startup, your program should print a greeting along with instructions before providing the user with its first prompt
       - "Welcome to OSShell! Please enter your commands ('exit' to quit)."
 - Implement 2 special commands (do not search for executable if either command is typed - do the following instead)
     - The first command is "history",  which will show previous commands entered
       - Store up to the latest 128 commands
       - Print all recent commands (and their position)
     - The second command is "exit", which will terminate the shell
       - Your program should quit (no longer prompt the user for input)
       
### *Additional features*

 - If user input does not match a program in the PATH, check if it is a relative or full path to an executable file
     - If so, spawn a new process to run that executable, and wait for its completion
 - Allow previous commands stored for the "history"command to persist between runs of OSShell
 - Allow for an optional argument to the "history" command - an integer > 0 or the string 'clear'
     - If this parameter exists and is a positive integer, only print the most recent commands up to that number
       - Example: `history 2` would print the two most recent commands
     - If this parameter exists and is the string 'clear', clear the history (and do not log the current `history` command in the history)
       - Example: `history clear` would clear the history list
     - Any number <= 0 or any non-integer other than the string 'clear' should result in an error
       - "Error: history expects an integer > 0 (or 'clear')"

## Compile and Run

compile: make

run: ./bin/osshell
