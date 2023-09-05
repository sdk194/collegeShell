#ifndef SHELL_H
#define SHELL_H

void initiateSHELL(void);        //This function sets the SHELL environment variable to the current shell
void batchMode(char * argv);    //this function will be called from main if an additional argument when running the shell is spotted
void sighandler(int signum);    //this is to handle the child process if running in the background, once the signal tells this function that the child is done, it will terminate the child properly preventing a zombie
void cd(char ** args);          //This function is for changing directories
void clr(void);                 //This function is for clearing the screen
void dir(char ** args);         //This function is for listing contents from a valid directory
void env(char ** args);         //This function is for listing all environment variables
void echo(char ** args);        //This function is for echoing args to standard output or to a file
void help(char ** args);                //This function will display the readme in the 'more' command style
void paused(void);              //This function will pause the shell until the enter key is pressed
void selector(char ** args, int len);   //This function determines which functions are called out for execution of commands
void external(char ** args, int len);   //This function handles any other invocation that isnt an internal command using fork and exec
extern char ** environ;                 //grabbing the environment variables

#endif

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