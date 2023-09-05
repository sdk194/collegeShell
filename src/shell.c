#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "shell.h"

#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPERATORS " \t\n"

int main(int argc, char * argv[]) {     //main will handle the parsing and input
    if (argv[1]) {  //escapes out of main and goes to batchmode function which exits once finished
        batchMode(argv[1]);
    }
    char buffer[MAX_BUFFER];
    char * args[MAX_ARGS];
    char ** arg;
    int power = 1;
    initiateSHELL();    //changing SHELL environment variable

    while (power) {     //power's function is to keep the while loop going until it has been turned off (i.e. power = 0)
        char pwd[MAX_BUFFER + 1];
        getcwd(pwd, MAX_BUFFER + 1);
        fputs(strcat(pwd, "> "), stdout);   //Display the current directory with the prompt
        
        int length = 1; //assume theres at least 1 argument, mainly used to check for ampersand (to check for it, u do args[len-1])
        if (fgets(buffer, MAX_BUFFER, stdin)) {
            arg = args;
            *arg++ = strtok(buffer, SEPERATORS);

            while ((*arg++ = strtok(NULL, SEPERATORS))) {       //The parsing process was taken from Ian G Graham's sample shell, more information of acknowledgements in the readme
                length += 1;
            }
        }

        //printf("%ld\n", strlen(args[0]));     FOR TESTING PURPOSES

        if (!args[0]) {     //In case no arguments were passed, shell doesnt break
            continue;
        }

        else if (!strcmp(args[0], "quit")) {    //This will turn switch power to 0 to stop the shell
            power = 0;
        }
        else {
            selector(args, length);     //args and the length of the args are passed to the selector function
        }
    }
    return 0;
}

/*
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
*/