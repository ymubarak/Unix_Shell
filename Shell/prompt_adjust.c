#include <stdio.h>
#include <string.h>
#include "variables.h"
#define PROMPT_LABEL "MrJoe"
#define LABEL_COLOR KRED
#define DEFAULT_COLOR KWHT
#define BOLD "\033[1m"
#define UNBOLD "\033[0m"
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void print_prompt()
{
    /* Enter bold mode */
    printf(BOLD);
    /* print prompt:path */
    printf(LABEL_COLOR"%s",PROMPT_LABEL);
    printf(KWHT":");
    printf(KGRN"%s"KWHT"> ",lookup_variable("CURRENT_DIR"));
    /* Turn off bold mode! */
    printf(UNBOLD);;
    /* Turn off color mode! */
    printf(DEFAULT_COLOR);
}

