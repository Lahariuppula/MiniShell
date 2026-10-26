#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 
/* command types */
#define BUILTIN		    1
#define EXTERNAL	    2
#define NO_COMMAND      3
#define RUNNING_PROCESS 4
#define STOPPED_PROCESS 5

/* colours */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* job structure */
typedef struct jobs
{
    pid_t pid;
    int job_num;
    int status;
    char command[100];
    struct jobs *link;
} jobs_t;

/* globals */
extern jobs_t *head;
extern pid_t pid;
extern int last_status;
extern char prompt[];
extern char *external_commands[155];
extern int job_counter;

/* scan */
void scan_input(char *prompt, char *input_string);

/* job helpers */
int insert_at_first(jobs_t **head, pid_t pid, char *cmd, int status);
int delete_first(jobs_t **head);

/* command helpers */
char *get_command(char *input_string);
void extract_external_commands(char **external_commands);
int check_command_type(char *command);
int execute_internal_commands(char *input_string);
void execute_external_commands(char *input_string);

/* signal */
void my_handler(int signum);

#endif