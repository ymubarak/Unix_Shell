#include "environment.h"
#include "variables.h"
#include <stdio.h>
#include <stdlib.h>

void setup_environment( void )
{
	set_variable("PATH",getenv("PATH"));
	set_variable("HOME",getenv("HOME"));
}
