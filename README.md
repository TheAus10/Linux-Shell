# Linux-Shell

### Overview
A program that can take in and execute a linux command. It can be ran in an interactive mode to run one command at a time, or in batch mode to read a list of commands from a file and execute them all sequentially. The application also supports a few custom commands and features. Those being, directory change, command history, environment path, signal catching, pipelining, and redirection. The program uses system calls such as `fork()` and `execvp()` to handle executing the commands on different processes. Application was developed in a Linux environment.

### To Run
To compile the program, use command `gcc main.c processor.c -o [executable name]`. This will generate an executable of your desired name that can be used to run the program. 
When running, there are two different modes, batch and interactive. 
##### Batch Mode
To enter batch mode, add a file as a command line argument after the executable name. 
##### Interactive Mode
To run in interactive mode, simply run the executable and type commands as you go. 

### To Use
The system can execute any Linux system command. 
