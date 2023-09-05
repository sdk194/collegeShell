#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_BUFFER 1024
#define MAX_ARGS 64
#define SEPERATORS " \t\n"

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

void initiateSHELL(void) {
    char pwd[MAX_BUFFER + 1];
    getcwd(pwd, MAX_BUFFER + 1);    //getting the current directory where shell was launched from
    setenv("SHELL", pwd, 1);        //setting the environment variable to the directory where shell was launched from
}

void selector(char ** args, int len) {  //selector function looks through every comparison until it matches, otherwise it goes to the external function
    if (!strcmp(args[0], "cd")) {
        cd(args);
    }
    else if (!strcmp(args[0], "clr")) {
        clr();
    }
    else if (!strcmp(args[0], "dir")) {
        dir(args);
    }
    else if (!strcmp(args[0], "environ")) {
        env(args);
    }
    else if (!strcmp(args[0], "echo")) {
        echo(args);
    }
    else if (!strcmp(args[0], "pause")) {
        paused();
    }
    else if (!strcmp(args[0], "help")) {
        help(args);
    }
    else {
        external(args, len);
    }
}

void sighandler(int signum) {   //will kill the child process in the background
    wait(NULL);
}

void cd(char ** args) {
    char pwd[MAX_BUFFER];
    int i = 1;
    char query[MAX_BUFFER] = "";
    while (args[i]) {   //This while loop is for if there is more than one argument (as a directory could have spaces)
        strcat(query, args[i]);
        if (args[i + 1]) {
            strcat(query, " ");
        }
        i += 1;
    }
    if (args[1]) {
		char oldpwd[MAX_BUFFER];
		getcwd(oldpwd, MAX_BUFFER + 1);
		setenv("OLDPWD", oldpwd, 1);
        if (!chdir(query)) {  //If directory exists
            printf("Now in: %s\n", getcwd(pwd, MAX_BUFFER));    //prints out the new current directory path
            setenv("PWD", pwd, 1);  //changing pwd environment variable to new current pwd variable
        }
        else {  //if it doesnt exist
            printf("No such directory exists\n");
        }
    }
    else {
        printf("Currently in: %s\n", getcwd(pwd, MAX_BUFFER));  //prints out the current directory path
    }
}

void clr(void) {
    printf("\e[1;1H\e[2J"); //Regex taken from GeeksforGeeks, more information of acknowledgement in readme
}

void dir(char ** args) {    //directory listing taken from StackOverFlow, more info of acknowledgement in readme
    FILE * f;
    int bool = 0;   //bool is for if an io redirection has been found
    int i = 0;
    while (args[i]) {   //checking for io redirection symbols
        if (!strcmp(args[i], ">")) {
            if (!args[i + 1]) {
                return;
            }
            f = fopen(args[i + 1], "w");
            if (!f) {
                printf("Error occurred\n");
                free(f);
                exit(1);
            }
            bool = 1;
        }                                           //Only need ">" and ">>" as "<" cannot be a valid io redirection for dir
        else if (!strcmp(args[i], ">>")) {
            if (!args[i + 1]) {
                return;
            }
            f = fopen(args[i + 1], "a");
            if (!f) {
                printf("Error occurred\n");
                free(f);
                exit(1);
            }
            bool = 1;
        }
        i += 1;
    }
    DIR * d;
    struct dirent * dir;
    if (!args[1] || (!strcmp(args[1], ">") || !strcmp(args[1], ">>"))) {    //ensures dir lists current directory in cases where its 'dir > outputfile' or if its just 'dir'
        d = opendir(".");
    }
    else if (args[1]) {
        d = opendir(args[1]);
    }
    if (d) {
        if (bool) { //If bool is switched to 1, redirects output to a file
            while ((dir = readdir(d)) != NULL) {
                fprintf(f, "%s\n", dir->d_name);
            }
            fclose(f);
        }
        else {  //otherwise, just prints to standard output
            while ((dir = readdir(d)) != NULL) {
                printf("%s ", dir->d_name);
            }
            printf("\n");
        }
    }
    else {
        printf("No such directory\n");
    }
    closedir(d);
    //fclose(f);
}

void env(char ** args) {
    int bool = 0;   //bool is for if an io redirection has been found
    int i = 0;
    FILE * f;
    while (args[i]) {   //checking for io redirection symbols
        if (!strcmp(args[i], ">")) {
            if (!args[i + 1]) {
                return;
            }
            f = fopen(args[i + 1], "w");
            if (!f) {
                printf("Error occurred\n");
                free(f);
                exit(1);
            }
            bool = 1;
        }
        else if (!strcmp(args[i], ">>")) {
            f = fopen(args[i + 1], "a");
            if (!f) {
                printf("Error occurred\n");
                free(f);
                exit(1);
            }
            bool = 1;
        }
        i += 1;
    }
    char ** envs = environ;
    i = 0;
    if (bool) { //if bool has been switched to 1, will redirect output to file
        while (envs[i]) {
            fprintf(f, "%s\n", envs[i]);
            i += 1;
        }
        fclose(f);
    }
    else {  //otherwise, just prints to standard output
        while (envs[i]) {
            printf("%s\n", envs[i]);
            i += 1;
        }
    }
}

void echo(char ** args) {
    FILE * f;
    int bool = 0;   //for if an io redirection has been found
    int i = 1;
    while (args[i]) {   //checking for io redirections
        if (!strcmp(args[i], ">")) {
            if (!args[i + 1]) {
                return;
            }
            f = fopen(args[i + 1], "w");
            if (!f) {
                printf("Error occurred\n");
                free(f);
                exit(1);
            }
            bool = 1;
        }
        else if (!strcmp(args[i], ">>")) {
            if (!args[i + 1]) {
                return;
            }
            f = fopen(args[i + 1], "a");
            if (!f) {
                printf("Error occurred\n");
                free(f);
                exit(1);
            }
            bool = 1;
        }
        i += 1;
    }
    i = 1;
    if (bool) { //will redirect output to file if bool is 1
        while (strcmp(args[i], ">") && strcmp(args[i], ">>")) {
            fprintf(f, "%s ", args[i]);
            i += 1;
        }
        fprintf(f, "\n");
        fclose(f);
    }
    else {  //otherwise, prints to standard output
        while (args[i]) {
            printf("%s ", args[i]);
            i += 1;
        }
        printf("\n");
    }
}

void paused(void) {
    getchar();  //will wait for user to press enter key
}

void external(char ** args, int len) {
    pid_t p = fork();   //creates a duplicate of process
    if (p < 0) {    //incase the fork fails
        printf("Fork failed\n");
        exit(1);
    }
    else if (p == 0) {  //child process
        int i = 0;
        int f;  //initialising variable for possible io redirection
        while (args[i]) {   //checking for io redirection
            if (!strcmp(args[i], ">")) {
                if (!args[i + 1]) {
                    exit(1);
                }
                f = open(args[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 644);   //allows for creating, writing and replacing contents with a permission of 644
                if (f == -1) {
                    exit(1);
                }
                dup2(f, STDOUT_FILENO);
                close(f);
            }

            else if (!strcmp(args[i], ">>")) {
                if (!args[i + 1]) {
                    exit(1);
                }
                f = open(args[i + 1], O_CREAT | O_WRONLY | O_APPEND, 644);  //allows for creating, writing and appending to content with a permission of 644
                if (f == -1) {
                    exit(1);
                }
                dup2(f, STDOUT_FILENO);
                close(f);
            }

            else if (!strcmp(args[i], "<")) {
                if (!args[i + 1]) {
                    exit(1);
                }
                f = open(args[i + 1], O_CREAT | O_RDONLY);  //allows for creating and reading
                if (f == -1) {
                    exit(1);
                }
                dup2(f, STDIN_FILENO);
                close(f);
            }
            i += 1;
        }
        if (execvp(args[0], args) == -1) {  //executes external command as well as it checks if it fails, and if it does, child exits with a status of 1
            exit(1);
        }
        else {  //otherwise, child exits normally
            exit(0);
        }
    }
    else {  //parent process
        if (args[len - 1][0] == '&') {  //checks if final string is an &, and if it is, it runs the child in the background
            signal(SIGCHLD, sighandler);    //while the parent does its own thing, the signal waits for a child to finish and then the sighandler will terminate the child properly, preventing zombies
        }
        else {  //This is for the case of no &, the parent shell waits until the child is finished and terminates it properly, preventing zombies
           wait(NULL); 
        }
    }
}

void help(char ** args) {
    char location[MAX_BUFFER + 1] = "";
    strcat(location, getenv("SHELL"));
    strcat(location, "/../manual/readme.md");   //getting readme.md location by using SHELL location as starting destination
    FILE * manual = fopen(location, "r");   //opens the readme file for reading
	if (!manual) {
		printf("Error occurred, possibly not in the directory where manual is?\n");
		return;
	}
    FILE * f;
    int bool = 0;
    int j = 0;
    while (args[j]) {   //checking for io redirection symbols
        if (!strcmp(args[j], ">")) {
            if (!args[j + 1]) {
                return;
            }
            f = fopen(args[j + 1], "w");
            if (!f) {
                printf("Error occurred\n");
                free(f);
                exit(1);
            }
            bool = 1;
        }
        else if (!strcmp(args[j], ">>")) {
            if (!args[j + 1]) {
                return;
            }
            f = fopen(args[j + 1], "a");
            if (!f) {
                printf("Error occurred\n");
                free(f);
                exit(1);
            }
            bool = 1;
        }
        j += 1;
    }
    char c;
    int maxLines = 10;
    int i = 0;
    c = fgetc(manual);
    if (bool) { //wii redirect output to file if bool is 1
        while (c != EOF) {
            fprintf(f, "%c", c);
            c = fgetc(manual);
        }
        fclose(f);
        return; //returns to main after to prevent further commands of function from running
    }
    while (c != EOF) {  //otherwise, prints to standard output
        if (i == maxLines) {
            getchar();
            i = 0;
        }
        printf("%c", c);
        if (c == '\n') {
            i += 1;
        }
        c = fgetc(manual);
    }
    printf("\n");
    fclose(manual);
}

void batchMode(char * argv) {   //batchmode is similar to main, except it reads from file instead of input
    char buffer[MAX_BUFFER];
    char * args[MAX_ARGS];
    char ** arg;
    initiateSHELL();    //setting SHELL environment variable
    FILE * batch = fopen(argv, "r");

    while (fgets(buffer, MAX_BUFFER, batch)) {
        int i = 0;
        while (buffer[i]) {
            if (buffer[i] == '\n') {    //Must modify buffer to get rid of newline character to prevent parser from breaking as reading from file works a slight bit different to reading from standard output
                buffer[i] = 0;
            }
            i += 1;
        }
        int length = 1; //assume theres at least 1 argument, used to check for ampersand (to check for it, u do args[len-1])
        arg = args;
        *arg++ = strtok(buffer, SEPERATORS);

        while ((*arg++ = strtok(NULL, SEPERATORS))) {
           length += 1;
        }

        //printf("%ld\n", strlen(args[0]));

        if (!args[0]) {
            continue;
        }

        else if (!strcmp(args[0], "quit")) {
            exit(0);
        }
        else {
            selector(args, length);
        }
    }
    fclose(batch);
    exit(0);    //shell exits when everything is done
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
