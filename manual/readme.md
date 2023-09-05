MYSHELL

1. ACKNOWLEDGEMENTS:
    1.1 ARGS PARSER:
        - The arguments parser found within my shell code was taken from an author of a simple shell, Ian G Graham from the School of Information and Technology,
          Griffith University, Gold Coast
        - email: ian.graham@griffith.edu.au
        - sample shell created date: December 2003
    
    1.2 CLEAR SCREEN:
        - The regex used for clearing the screen was taken from the geeksforgeeks website
        - link for clear screen: https://www.geeksforgeeks.org/clear-console-c-language/
        - Website link content last update: 11 November 2022

    1.3 DIRECTORY LISTING:
        - Code for listing a directory was taken from a StackOverFlow question thread
        - link for thread: https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
        - Last accessed on: 19 March 2023


2. LIST OF INTERNAL COMMANDS:
    2.1 CD:
        - Changes the working directory and prints out the new working directory. If no other argument is given, it will print out the current working directory.
    
    2.2 CLR:
        - Clears the screen
    
    2.3 DIR:
        - Lists out all the contents of the current working directory. If given a directory as an argument, it will list out the current within that directory.

    2.4 ENVIRON:
        - Lists out all environment variables.

        2.4.1 ENVIRONMENT VARIABLES:
            - The shell is able to grab all of the environment variables of the system. It can also update the environment variables when necessary. For example,
              the environment variable for the current working directory can change when the internal command cd is called.

    2.5 ECHO:
        - Outputs a line of text with a newline character

    2.6 HELP:
        - Displays the user manual. Press the return key to continue every 10 lines.

    2.8 PAUSE:
        - Pauses the shell until the return key is pressed.
    
    2.9 QUIT:
        - Exits out of the shell


3. ADDITIONAL FEATURES:
    3.1 EXTERNAL COMMANDS:
        - This shell can run external commands via fork() and exec().
        - It can be done with 2 modes, either as a foreground execution or a background execution

        3.1.1 FORKING AND EXECUTING:
            - Calling a fork duplicates a process, creating a child process and a parent process
            - This shell, when running an external command, calls a fork and makes the child process run the external command.
            - The parent will then either wait, or if it was inputted with an "&" at the end of the argument, the child process will run in the background.

        3.1.2 FOREGROUND PROCESS:
            - If the "&" wasn't called within the argument, the child is executed as a foreground process, meaning the parent has to wait for the child to finish.
              This would mean the user will not have control until the child is finished executing.
            - Once the child is finished, the parent terminates the child to prevent a zombie process.

        3.1.3 BACKGROUND PROCESS:
            - If the "&" was called within the argument, the child is executed as a background process.This means the user can have control of the shell while the
              child is running in the background. 
            - A signal is implemented as a means to know if the child is done executing. Once the child is done executing, a signal
              will be sent to a signal handler, which will terminate the child properly to prevent a zombie process.


    3.2 I/O REDIRECTIONS:
        - This shell can handle input redirection of external commands and some internal commands.

        3.2.1 INPUT REDIRECTION:
            - The symbol for input redirections is "<".
            - What this means is any content within a file to the right is read, and is then sent as input to the program to the left.

        3.2.2 OUTPUT REDIRECTION:
            - The symbol for output redirections are ">" and ">>".
            - What this means is any execution of programs will be outputted to a file.
            - ">" Creates the file if it doesn't exist and then sends the output to it. If it does exist, it replaces the contents within the file.
            - ">>" Creates the file if it doesn't exist and then sends the output to it. If it does exist, it appends the new content to the file.


4. ACADEMIC INTEGRITY:
    Student Name: Sean Dagohoy
    Student Number: 21392656

    I understand that the University regards breaches of academic integrity and plagiarism as grave
    and serious.
    I have read and understood the DCU Academic Integrity and Plagiarism Policy. I accept the
    penalties that may be imposed should I engage in practice or practices that breach this policy.
    I have identified and included the source of all facts, ideas, opinions and viewpoints of others in
    the assignment references. Direct quotations, paraphrasing, discussion of ideas from books,
    journal articles, internet sources, module text, or any other source whatsoever are acknowledged
    and the sources cited are identified in the assignment references.
    I declare that this material, which I now submit for assessment, is entirely my own work and has
    not been taken from the work of others save and to the extent that such work has been cited and
    acknowledged within the text of my work.
    I have used the DCU library referencing guidelines (available at
    https://www4.dcu.ie/library/classes_and_tutorials/citingreferencing.shtml and/or the appropriate
    referencing system recommended in the assignment guidelines and/or programme documentation.
    By signing this form or by submitting material online I confirm that this assignment, or any part
    of it, has not been previously submitted by me or any other person for assessment on this or any
    other course of study.
    By signing this form or by submitting material for assessment online I confirm that I have read
    and understood the DCU Academic Integrity and Plagiarism Policy (available at
    http://www.dcu.ie/registry/examinations/index.shtml).
