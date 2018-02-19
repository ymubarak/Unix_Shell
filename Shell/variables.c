#include "variables.h"
#include<stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include<string.h>
# define MAX_CAPACITY 100
typedef struct{
    const char *key;
    const char *value;
} entry;

entry table[MAX_CAPACITY];
int pivot = 0;

const char* lookup_variable( const char* key )
{
	int i;
	for(i=0;i<pivot;i++){
       	    if(strcmp(key,table[i].key) == 0)
              return table[i].value;
	}
	if(getenv(key))
	  return getenv(key);
	return "";
}

void set_variable( const char* key , const char* value )
{
	bool found = false;
	int i;
	for(i=0;i<pivot;i++){
       	    if(strcmp(key,table[i].key) == 0){
            	table[i].value = value;
		found = true;
		break;
	     }
	}
	if(!found && pivot < MAX_CAPACITY){
		entry new_variable ={key,value};
		table[pivot] = new_variable;
		pivot++;
	}
}

void print_all_variables( void )
{
    int i;
	for(i=0;i<pivot;i++){
        printf("%s = %s\n",table[i].key,table[i].value);
	}
}
