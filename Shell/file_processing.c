#include "file_processing.h"
#include<stdio.h>
#include<stdlib.h>
# define HISTORY_FILE "history.txt"
# define LOG_FILE "log.txt"

/*
	history file section
*/
FILE *history,*logFile,*batch;

void open_history_file()
{
    history = fopen(HISTORY_FILE,"a+");
}

FILE* get_history_file()
{
    return history;
}

void close_history_file()
{
    fclose(history);
}


/*
	log file section
*/
void open_log_file()
{
    logFile = fopen(LOG_FILE,"w");
}

FILE* get_log_file()
{
    return logFile;
}

void close_log_file()
{
    fclose(logFile);
}


/*
	CommandsBatch file section
*/
void open_commands_batch_file(const char* batch_file_name)
{
    if((batch=fopen(batch_file_name,"r")) == NULL){
        printf("Bad file Or %s not found!\n",batch_file_name);
	exit(EXIT_FAILURE);
   }
}

FILE* get_commands_batch_file()
{
    return batch;
}

void close_commands_batch_file()
{
    fclose(batch);
}
