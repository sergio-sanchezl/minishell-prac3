/***********************************************
 * Author: Sergio Sanchez Lopez.               *
 * Group: 1ºE Grado en Ingenieria Informatica. *
 ***********************************************/
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "parser.h"
#include "execute.h"
#include "free_args.h"
#include "jobs.h"

//Signal handler (which sigaction act will use):
void handler(int sig, siginfo_t *siginfo, void *context) {
	// Reap zombie processes:
	while(waitpid(siginfo->si_pid,0,WNOHANG) > 0) {}

	// Then, change state of job to finished using jobs_finished:
	jobs_finished(siginfo->si_pid);
	//printf("-Job %d has finished-\n",siginfo->si_pid);
}

void execute_external_command(const char *command)
{	
	char **args;
	int backgr;
	int status;
	pid_t pid;
	
	struct sigaction act;
	act.sa_sigaction = handler; 		
	act.sa_flags = SA_SIGINFO | SA_RESTART;
	sigaction(SIGCHLD,&act,NULL); 	// Set SIGCHLD handler routine

	if ((args=parser_command(command,&backgr)) == NULL)
	{
		return;
	}

	pid = fork();

	if(pid==0){  // Child
		if (execvp(*args,args)<0){ // Change child's code.
			printf("ERROR: execvp failed. \n");
			exit(1);
			return;
		}
		pid = getpid(); // Get pid of exec'd child process
	}
	else if (pid>0){ // Parent, which will wait for child w/ or w/o hang.
			if(backgr==0){ // Foreground: will wait with hang.
				jobs_new(pid,args[0]);
				waitpid(pid,&status,0);
				}
			else{ // Will wait without hang so other commands can be run.
				jobs_new(pid,args[0]);
				waitpid(pid,&status,WNOHANG);
				}
		}
	else if ( pid < 0 ){
			printf("ERROR: fork failed"); // Will likely not happen.
			return;
		}

	parser_free_args(args);

	return;

}
