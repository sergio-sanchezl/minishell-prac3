#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "parser.h"
#include "execute.h"
#include "free_args.h"
#include "jobs.h"

void execute_external_command(const char *command)
{
	char **args;
	int backgr = 0;
	int status;
	pid_t pid, ppid;
	
	if ((args=parser_command(command,&backgr)) == NULL)
	{
		return;
	}
	/*if ((pid = fork())<0) {
		printf("Error: fork process failed. \n");
		exit(1);
	}
	else if (pid == 0) {
		jobs_new(pid,args[0]);
		if (execvp(*args,args) < 0) {
			printf("Error: execvp failed. \n");
			exit(1);
		}
		else {
			
			if(backgr == 0)
			while(wait(NULL)!=pid);
		}
	}
	else if (pid > 0){
		ppid = wait(&status);
	}
	*/
	pid = fork();

	if(pid==0){
		ppid=getpid();
		if (execvp(*args,args)<0){
			printf("ERROR: execvp failed. \n");
			exit(1);
		}
	}else if (pid>0){
				ppid = wait(&status);
				if (WIFEXITED(status))
				printf("Done!\n");
				}
	


	jobs_finished(pid);

	fflush(stdout);

	parser_free_args(args);
	return;

}



