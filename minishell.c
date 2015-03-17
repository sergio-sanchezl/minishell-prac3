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
	
	//if(strstr(buf,"exit") != NULL){
	/* with strcmp, typing multiple commands where any of them is "exit"
	wont close the minishell. For that, I could use strstr to search
	for the substring "exit", but that way if you type something like
	 "echo exit", the minishell would close, so I prefer to avoid that.*/
	if(strcmp(buf,"exit") == 0){
		jobs_free_mem();
		exit(0);
	}

	return;
}

int main (int argc, char *argv[])
{
	char buf[BUFSIZ];
	char *tokenbuf;	
	while(1){

		print_prompt();
		
		read_command_line(buf);
		tokenbuf=strtok(buf,";"); // gets substrings separated with token ';'

		while( tokenbuf != NULL ){ // if no substring is found (no more tokens)
			checkExit(tokenbuf);

			if(is_internal_command(tokenbuf)==1){

				execute_internal_command(tokenbuf);
			}
			else if(is_internal_command(tokenbuf)==0){

				execute_external_command(tokenbuf);
			}
			else printf("\n Unexpected ERROR \n");
			tokenbuf=strtok(NULL,";"); // next substring.
		}
		
	}
	return 0;
}
