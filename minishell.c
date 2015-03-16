#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "internals.h"
#include "execute.h"
#include "jobs.h"
#include "minishell_input.h"

void checkExit(const char * buf){
	
	if(strcmp(buf,"exit") == 0){
		jobs_free_mem();
		exit(0);
	}

	return;
}

int main (int argc, char *argv[])
{
	char buf[BUFSIZ];
	
	while(1){

		print_prompt();
		
		read_command_line(buf);

			checkExit(buf);

			if(is_internal_command(buf)==1){

				execute_internal_command(buf);
			}
			else if(is_internal_command(buf)==0){

				execute_external_command(buf);
			}
			else printf("\n Unexpected ERROR \n");
		}

	return 0;
}
