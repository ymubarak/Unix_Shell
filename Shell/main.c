#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include"command_parser.h"
#include"commands.h"
#include"environment.h"
#include"execute_engine.h"
#include"file_processing.h"
#include"prompt_adjust.h"
#include"variables.h"

#define CMND_MAX_LENGTH 513

void start_shell(bool read_from_file);
void shell_loop(bool input_from_file);
void stopShell();
void closeFiles();

bool Runing;
char *batch_file_name;
FILE *bf,*hstry;


int main(int argc, char *argv[])
{

    setup_environment();
    open_history_file();
    open_log_file();
    hstry = get_history_file();
    Runing = true;

    if( argc > 1 )
    {
        batch_file_name = argv[1];
        open_commands_batch_file(batch_file_name);
        bf = get_commands_batch_file();
        start_shell(true);
    }
    else
    {
        start_shell(false);
    }

    return 0;
}

void start_shell(bool read_from_file)
{
    cd(lookup_variable("HOME")); // let shell starts from home
    if(read_from_file)
    {
        shell_loop(true);
    }
    else
    {
        shell_loop(false);
    }
}

void shell_loop(bool input_from_file)
{
    bool from_file = input_from_file;
    char command[CMND_MAX_LENGTH];
    while(Runing)
    {

        if(from_file)
        {
	    // stop if end of file
            if(feof(bf))
            {
                from_file = false;
                continue;
            }
            //read next instruction from file
            if(fgets(command,sizeof(command),bf)!=NULL)
                printf("%s",command);
        }
        else
        {
            //read next instruction from console
            print_prompt();
            fgets(command,sizeof(command),stdin);
        }
        // trim command
        char * trimmed_command = trimwhitespace(command);
        // write to history
        if (*trimmed_command!= 0)
        {
            fprintf(hstry,"%s\n",trimmed_command);
            char** tokens = parse_command(trimmed_command);
            if(*tokens[0]!='#')
                execute(wait_status(),tokens[0],tokens);
            free(tokens);
        }
    }
    closeFiles();
}


void stopShell()
{
    Runing = false;
}
void closeFiles()
{

    close_history_file();
    close_log_file();

}
