#include "minishell.h"

char prompt[] = "msh>";
extern char *external_commands[155];
int main()
{
    /* load external commands list */
    extract_external_commands(external_commands);

    system("clear");

    char input_string[1024];
    scan_input(prompt, input_string);

    return 0;
}

/*SCAN INPUT*/
/*
    1. print prompt[]
    2. read input_string using selective scanf
    3. check input_string has "PS1="
    true->prompt = input_string
    false-> 
    4. char *cmd = get_command(input_string);
    5. int ret = check_command_type(cmd);
    6. check ret == 1
    {
       //getcwd(),chdir(),getenv()
       execute_internal_command(input_string);
    }
    check ret == 2
    {
        execute_external_command(input_string);
    }
    ret == 0
    printf("Invalid command");
    7.Go to step 1
*/

/*CHECK COMMAND TYPE*/
/*
    1. logic to compare cmd with builtin array
        matching -> return 1
    2. logic to compare cmd with external array
        matching -> return 2
        return 0 -> invalid
*/
/*EXECUTE INTERNAL COMMANDS*/
/*
    1. check input is "pwd"
    yes -> getcwd();
    2. check input is "cd"
    yes -> chdir();
    3. check input is "exit"
    yes -> exit(1)
    4. check input is "echo"
    yes -> check $SHELL
            yes -> getenv(SHELL);
           check $$
            yes -> getpid();
           check $?
            yes -> last process execution is done or not
    If it is any other builtin cmd entered then print "Internal Command"
*/