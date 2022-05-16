# 3600-Major-2-The-Shell-and-System-Call

Major Assignment 2: The Shell and System Calls

Group Number 10
Group Members: Anika Chowdhury, Christopher Collins, Dajanek Davis, Joshua Thomas

BONUS: We implemented the bonus for this project. We implemented the customized prompt.

Project Responsibilites:

Individual

Built in commands:
Cd (Christopher Collins)
My history (Joshua Thomas)
Path (Anika Chowdhury)
Exit (Dajanek Davis)
Redirection, Pipelining, signal control, alias support
Pipelining (Christopher Collins)
Alias command (Joshua Thomas)
Signal control (Anika Chowdhury)
Redirection (Dajanek Davis)

Group

Shell
Interactive user input (Dajanek Davis and Joshua Thomas)
File of commands (batch mode) (Joshua Thomas)
major2.c (Joshua Thomas)
execution function (Joshua Thomas)

Organization of Project:
The work was organized and managed in the group portion by dividing the interactive and batch mode amongst teammembers. 
Chris and Joshua were responsible for batch mode. Specifically, Chris was responsible for the defensive programming of batch
mode. Dajanek was responsible for interactive mode. Christopher was responsible for the 'cd' command and pipeling, Joshua 
was responsible for 'myhistory' and the 'alias' command, Anika was responsible for the path command and signal control, and Dajanek
was responsible for the 'exit' command and redirection.   


Defensive programming
major2.c (Joshua Thomas)
Interactive (Joshua Thomas)
Batch (Christopher Collins and Joshua Thomas)

Misc.

Readme (Christopher Collins, Dajanek Davis)
Organization of the project (Dajanek Davis)
Design overview (Christopher Collins)
Complete Specification (Christopher Collins)
Make file (Joshua Thomas)

Known Bugs/Problems:

-The pipe can't accept 2 commands or not have a space between the command and pipe.
ex- ls | sort | cat

-Path hasn't been implemented yet. The commented out code is in shell.c.

Complete Specification:

We managed to deal with the ambiguity of the specifications by adding a parsing system that 
would detect spaces, semicolons, etc. We would analyze what inputs could be made from a broad scale
and use that information to determine what information we would parse from. Earlier in the project
we also experimented with errorhandling. One of the biggere obstacles was dealing with git.
Git made planning and pushing commits harder due to a lack of experience and merges potentially 
messing with the repository.Initially we set the programto respond to the number of commands 
entered but this changed due to future specifications by our professor. We individualized parts 
and even group parts of the project inorder to make its more efficent for us to colaborate.

Design overview:

The basic flow of the program is us asking for the prompt or the user to be given the oppritunity 
to alter the prompt to their liking. Based on the mode that user choses we go into either interactive 
or batch modes. From there the user is allowed to chose what mode they want to use. It executes from 
a commandline of batch file or interactive mode. From there the user is given several commands they can use
such as cd, Path, my history and exit. Then they are given several modes for functions that alter
the shell itself such as Redirection, Pipelining, Signal Control, and Alias Support. Each of these 
funtions are called when the user enters their commands a certain way. For instance pipe is segmented via | operators.
We parse what is in between and execute the command. The others work similarly


Usage Instructions:
- Use the command 'make' to compile the program.
- Enter ./major2 or 'make run' to run the program.
- When program begins, choose whether you want to customize your prompt.
    - If so, follow the directions in the program
- When the prompt appears type either 'newshell' or 'newshell <filename>'
    - 'newshell' will activate the shell in interactive mode
    - 'newshell filename' will activate the shell in batch mode

Other Makefile Commands:
- Use the command 'make rebuild' to recompile all program files (both modified and unmodified).
- Use the command 'make clean' to delete the compiled program and the .o program files (Source code files are unaffected).



