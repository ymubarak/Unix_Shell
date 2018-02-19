#include"variables.h"
#include"file_processing.h"
#include"commands.h"
#include"command_parser.h"
#include"main.h"
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include <ctype.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<unistd.h>
#define CMND_LINE_LENGTH 513

void print_history();
bool external_command(const char *command,char *const args[]);
void child_process(const char *command, char *const args[]);
bool check_expresion(const char *command,bool isExported);
void finish();

FILE *lg;
void execute(bool waitting,const char *command, char *const args[])
{
    lg = get_log_file();
    if(!external_command(command,args))
    {
        int status;
        pid_t child_id, end_id;
        signal(SIGCHLD,finish);
        child_id = fork();
	if(child_id == -1){
	  printf("Fork failed! \n");
        }else if(child_id == 0)
        {
            child_process(command,args);
        }
        else if(waitting)
        {
            // printf("foreground command , parent should wait..\n");
            //wait(NULL);
            int status;
            do
            {
                waitpid(child_id, &status, WUNTRACED);
            }
            while( !WIFEXITED(status) && !WIFSIGNALED(status) );
        }
    }

}

bool with_path(const char* command){
	while(*command){
	    if(*(command++) == '/')
		return true;
	}
	return false;
}

void child_process(const char *command, char *const args[])
{

    if(strcmp(command,"history") == 0)
        print_history();
    else if(strcmp(command,"echo") == 0)
        echo(args);
    else
    {

        bool valid_command = false;
	if(with_path(command)){
	   if(execv(command,args)==0)
                valid_command = true;
	}else{
        char ** paths = tokenize(lookup_variable("PATH"),':');
        int i =0;
        while(*(paths+i))
        {
            if(execv(concat(*(paths+i),concat("/",command)),args)==0)
            {
                valid_command = true;
                break;
            }
            i++;
        }
	}
        if(!valid_command){
            printf("%s: command not found\n",command);
            exit(EXIT_FAILURE);
          }
    }
    exit(EXIT_SUCCESS);
}


void print_history()
{
    FILE *h = get_history_file();
    fseek(h,0L,SEEK_SET);
    char command[CMND_LINE_LENGTH];
    while(!feof(h))
    {

        if(fgets(command,sizeof(command),h)==NULL)
            break;
        printf("%s",command);
    }
}

bool external_command(const char *command,char *const args[])
{
    if(strcmp(command,"exit") == 0)
    {
        stopShell();
        return true;
    }
    if(strcmp(command,"cd") == 0)
    {
        cd(args[1]);
        return true;
    }
    if(check_expresion(command,false))
        return true;
    if(strcmp(command,"export") == 0)
        return check_expresion(args[1],true);
    return false;
}

bool check_expresion(const char *command,bool isExported)
{
    if(isdigit((unsigned char)*command) || *command == '='){
	if(isExported)
	   printf("export:%s: invalid expression\n",command);
	else
           printf("%s: command not found\n",command);
	return true;
    
    }else
    {
        char* var = malloc(strlen(command));
        char* end = var;
        bool valid_exp = true;
        bool has_value = false;
        while(*command)
        {
            if(*command=='=')
            {
               has_value=true;
                command++;
                break;
            }
            if(isalnum((unsigned char)*command))
            {
                *end=*command;
                end++;
                command++;
            }
            else{
		 valid_exp = false;
                break;
		}
        }
        if(valid_exp)
        {
	    if(has_value){
		    *end =0;
		    char* value = malloc(strlen(command)+1);
		    end = value;
		    while(*command )
		    {
		        if(*command !='\"')
		        {
		            *end=*command;
		            end++;
		        }
		        command++;
		    }
		    set_variable(var,value);
	    }else if(isExported)
		    set_variable(var,"");
	    else 
		valid_exp = false;
        }else if(isExported){
	   printf("export:%s: invalid expression\n",command);
	   return true;
	}
        return valid_exp;
    }
}

void finish()
{ 
    fprintf(lg,"Child process terminated\n");
}
