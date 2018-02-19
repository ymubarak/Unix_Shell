#include "command_parser.h"
#include"variables.h"
#include<string.h>
#include<stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#define MAX_NUM_OF_ARGS 10
#define CMND_MAX_LENGTH 513
char* concat(const char *s1,const char *s2);
char* expand_variables(const char* command);
char ** tokenize(const char* parsed_command,char delimiter);
bool run_in_foreground;

char** parse_command( const char* command )
{
    run_in_foreground = true;
    char* expanded_command = expand_variables(command);
    return tokenize(expanded_command,' ');
}

char ** tokenize(const char * str,char delimiter)
{
    char** tokens = malloc((MAX_NUM_OF_ARGS+1)*sizeof(char*));
    int i=0;
    char* word = malloc(strlen(str)+1);
    char* end = word;
    bool completed_word= false;
    bool within_qoutes = false;
    while(*str)
    {
        if(*str == '\"')
            within_qoutes =!within_qoutes;
        if(*str != delimiter || within_qoutes)
        {
            completed_word = true;
            *end=*str;
            end++;
        }
        else if(completed_word)
        {
            completed_word = false;
            *end = 0;
            end++;
            tokens[i]=word;
            word = end;
            i++;
        }
        str++;
    }
    if(*(end-1) == '&')
    {
        run_in_foreground = false;
        *(end-1)=0;
    }
    else
        *end = 0;
    if(*word)
    {
        tokens[i]=word;
        tokens[i+1]=NULL;
    }
    else
        tokens[i]=NULL;
    return tokens;
}

char* concat(const char *s1,const char *s2)
{
    char *result;
    result = malloc(strlen(s1)+strlen(s2)+1);
    if(result == NULL)
    {
        printf("Error: malloc failed in concat\n");
        exit(EXIT_FAILURE);
    }
    strcpy(result,s1);
    strcat(result,s2);
    return result;
}
char* expand_variables(const char* str)
{
    char *parsed_command = malloc(CMND_MAX_LENGTH);
    if(parsed_command == NULL)
    {
        printf("Error: malloc failed in expand_variables\n");
        exit(EXIT_FAILURE);
    }
    char* end = parsed_command;
    while(*str)
    {
        if(*str == '$' && *(str+1)!=' ' && *(str+1)!='\0')
        {
            str++;
            if(*str && isdigit((unsigned char)*str))// STOP IF FOLLWED BY A DIGIT
            {
                char digit[2]= {'9','\0'};
                strcat(end,lookup_variable(digit));
                end+= strlen(lookup_variable(digit));
                str++;
            }
            else
            {
                // GET THE VAR NAME
                char p[CMND_MAX_LENGTH/2];
                int i =0;
                while(isalnum((unsigned char)*str))
                {
                    p[i]=*str;
                    i++;
                    str++;
                }
                p[i]='\0';
                *end=0;
                strcat(end,lookup_variable(p));
                end+= strlen(lookup_variable(p));
            }
        }
        if(*str =='~'){
         *str++;
         *end=0;
         strcat(end,lookup_variable("HOME"));
         end+= strlen(lookup_variable("HOME"));
	}
        if(*str)
        {
            *end =*str;
            end++;
            str++;
        }
    }
    // Write new null terminator
    *end =0;
    return parsed_command;
}

char *trimwhitespace(char *str)
{
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end+1) = '\0';

    return str;
}

bool wait_status()
{
    return run_in_foreground;
}
