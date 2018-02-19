#include "commands.h"
#include "variables.h"
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

char cwd[1024];
char temp[1024];
const char *p;
void cd( const char* path )
{
    p = path;
    bool user_not_found = false;
    if(p==NULL)
        p=memcpy(temp,lookup_variable("HOME"),strlen(lookup_variable("HOME"))+1);
    else if(*p =='~')
    {
        memcpy(temp,lookup_variable("HOME"),strlen(lookup_variable("HOME"))+1);
        if(*(p+1) == 0)
            p = temp;
        else
        {
            bool found_slash = false;
            p++;
            while(*p=='/')
            {
                found_slash=true;
                p++;
            }
            if(found_slash)
                p = strcat(temp,p-1);
            else
            {
                *(temp+6)=0;
                p = strcat(temp,p);
                user_not_found = true;
            }
        }
    }
    if(chdir(p)==0)
    {
        getcwd(cwd, sizeof(cwd));
        if(strcmp(cwd,lookup_variable("HOME")) ==0)
        {
            cwd[0] = '~';
            cwd[1] = '\0';
        }
        set_variable("CURRENT_DIR",cwd);
    }
    else
    {
        if(user_not_found)
            printf("cd: %s: No such file or directory\n",path);
        else
            printf("cd: %s: No such file or directory\n",p);
    }
}


void echo( char *const args[] )
{
    int i=1;
    while(args[i] != NULL)
       printf("%s " ,args[i++]);
     printf("\n");

}
