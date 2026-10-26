#include "minishell.h"

extern int pid;
extern jobs_t *head;
int last_status = 0;
extern char prompt[];

//GLOBAL VARIABLE
// Stores the last command executed so we can save it when Ctrl+Z is pressed
char last_input[1024];  

void scan_input(char *prompt, char *input_string)
{
    /* register signal handlers */
    signal(SIGINT, my_handler);
    signal(SIGTSTP, my_handler);
    signal(SIGCHLD, my_handler);

    //welcome message
    printf(ANSI_COLOR_CYAN"--------------WELCOME TO MINISHELL----------------\n"ANSI_COLOR_RESET);

    while (1)
    {
        //printing the prompt
        printf(ANSI_COLOR_GREEN"%s"ANSI_COLOR_RESET, prompt);
        fflush(stdout);

        if (fgets(input_string, 1024, stdin) == NULL)
        {
            if (feof(stdin))
            {
                printf("\n");
                exit(0);
            }
            clearerr(stdin);
            continue;
        }

        // remove trailing newline
        input_string[strcspn(input_string, "\n")] = 0;

        // if user pressed only Enter
        if (input_string[0] == '\0')
        {
            continue;
        }

        // store the command in global variable
        strcpy(last_input, input_string);

        /* change shell prompt */
        if (strncmp(input_string, "PS1=", 4) == 0)
        {
            if (input_string[4] == '\0')
            {
                printf("invalid prompt\n");
                continue;
            }
            strcpy(prompt, input_string + 4);
            continue;
        }
        else
        {
            char *cmd = get_command(input_string);
            int type = check_command_type(cmd);

            if (type == BUILTIN)
            {
                int ret = execute_internal_commands(input_string);

                if (strcmp(input_string, "echo $?") != 0 &&
                    strcmp(input_string, "echo $$") != 0 &&
                    strcmp(input_string, "echo $SHELL") != 0 &&
                    strcmp(input_string, "echo $") != 0)
                {
                    last_status = ret;
                }
            }
            else if (type == EXTERNAL)
            {
                int background = 0;
                if (input_string[strlen(input_string) - 1] == '&')
                {
                    background = 1;
                    input_string[strlen(input_string) - 1] = '\0';
                }              
                pid = fork();
                if (pid < 0)
                {
                    perror("fork failed");
                }
                else if (pid == 0)
                {
                    signal(SIGINT, SIG_DFL);
                    signal(SIGTSTP, SIG_DFL);
                    execute_external_commands(input_string);
                    exit(0);
                }
                else
                {
                    if (background)
                    {
                        insert_at_first(&head, pid, input_string, RUNNING_PROCESS);
                        printf("[%d] %d %s &\n", head->job_num, pid, input_string);
                        pid = 0;
                    }
                    else
                    {
                        int status;
                        waitpid(pid, &status, WUNTRACED);
                        if (WIFEXITED(status))
                            last_status = WEXITSTATUS(status);
                        else if (WIFSIGNALED(status))
                            last_status = 128 + WTERMSIG(status);
                    }
                }
            }
            else
            {
                printf("%s: command not found\n", cmd);
                last_status = 127;
            }
        }
        pid=0;
    }
}

void my_handler(int signum)
{
    int status;

    if (signum == SIGINT)
    {
        if(pid==0)
        {
        // Ctrl+C pressed
        printf("\n");
        fflush(stdout);
        }
    }
    else if (signum == SIGTSTP)
    {
        if (pid == 0)
        {
            // Ctrl+Z pressed in shell (no foreground process)
            printf("\n");
            fflush(stdout);
        }
        else
        {
            // Stop the foreground process
            kill(pid, SIGSTOP);

            // Save stopped process in job list
            insert_at_first(&head, pid, last_input, STOPPED_PROCESS);

            printf("\n");
            //pid = 0;  // reset foreground pid
        }
    }
    else if (signum == SIGCHLD)
    {
        int status;
        pid_t child_pid;
        while ((child_pid = waitpid(-1, &status, WNOHANG)) > 0)
        {
            jobs_t *temp = head;
            jobs_t *prev = NULL;

            while (temp != NULL)
            {
                if (temp->pid == child_pid)
                {
                    if (WIFEXITED(status))
                    {
                        printf("\n[%d] Done %s\n",temp->job_num, temp->command);
                        last_status = WEXITSTATUS(status);
                    }
                    else if (WIFSIGNALED(status))
                    {
                        printf("\n[%d] Terminated %s\n",temp->job_num, temp->command);
                        last_status = 128 + WTERMSIG(status);
                    }

                // Remove this completed job
                if (prev == NULL)
                    head = temp->link;
                else
                    prev->link = temp->link;

                free(temp);
                break;
            }
            prev = temp;
            temp = temp->link;
        }
    }
    }
}

int insert_at_first(jobs_t **head, pid_t pid, char *cmd,int status)
{
    jobs_t *new = malloc(sizeof(jobs_t));

    if (new == NULL)
        return -1;

    new->pid = pid;
    new->job_num = ++job_counter;
    new->status = status;

    if (cmd != NULL)
        strncpy(new->command, cmd, sizeof(new->command) - 1);

    new->command[sizeof(new->command) - 1] = '\0';

    new->link = *head;
    *head = new;

    return 0;
}

int delete_first(jobs_t **head)
{
    if (*head == NULL)
    {
        return -1;
    }
    else
    {
        jobs_t *temp = *head;
        *head = temp->link;
        free(temp);
        return 0;
    }
}
