#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char *argv[]){

	printf("Fork Process Begin\n");
	/* fork a child process */
	
	int stat;
	pid_t identifier;
	identifier = fork();
	/* execute test program */ 

	if(identifier != -1){
		if(identifier == 0){
			int n;
			char *arg[argc];

			n = 0;
			while(n<argc-1){
				arg[n] = argv[n+1];
				n++;
			}

			arg[argc-1] = NULL;
			printf("This is child process, the PID = %d\n", getpid());
			printf("Child start to execute test program:\n");
			execve(arg[0], arg, NULL);
		}

		else{
			printf("This is parent process, the PID = %d\n", getpid());
			waitpid(identifier, &stat, WUNTRACED);
			/* wait for child process terminates */
			printf("Receiving SIGCHLD Signal For Parent Process");

			if(WIFEXITED(stat)){
				printf("Normal termination with EXIT status = %d\n", WEXITSTATUS(stat));
			}
			else if(WIFSIGNALED(stat)){
				if(WTERMSIG(stat) == 1){
					printf("Child Process Receive SIGHUP Signal \n");
					printf("Child Process Is Hung Up \n");
				}

				else if(WTERMSIG(stat) == 2){
					printf("Child Process Receive SIGINT Signal \n");
					printf("Child Process Got Interrupted \n");
				}

				else if(WTERMSIG(stat) == 3){
					printf("Child Process Receive SIGQUIT Signal \n");
					printf("Child Process Quited \n");
				}

				else if(WTERMSIG(stat) == 4){
					printf("Child Process Receive SIGILL Signal \n");
					printf("Child Process Got Illegal Instruction \n");
				}

				else if(WTERMSIG(stat) == 5){
					printf("Child Process Receive SIGTRAP Signal \n");
					printf("Child Process Is Trapped \n");
				}

				else if(WTERMSIG(stat) == 6){
					printf("Child Process Receive SIGABRT Signal \n");
					printf("Child Process Is Aborted \n");
				}

				else if(WTERMSIG(stat) == 7){
					printf("Child Process Receive SIGBUS Signal \n");
					printf("Child Process Is Blocked \n");
				}

				else if(WTERMSIG(stat) == 8){
					printf("Child Process Receive SIGFPE Signal \n");
					printf("Child Process Is Floated \n");
				}

				else if(WTERMSIG(stat) == 9){
					printf("Child Process Receive SIGKILL Signal \n");
					printf("Child Process Is Killed \n");
				}

				else if(WTERMSIG(stat) == 11){
					printf("Child Process Receive SIGSEGV Signal \n");
					printf("Child Process Got Segment Fault \n");
				}
				else if(WTERMSIG(stat) == 13){
					printf("Child Process Receive SIGPIPE Signal \n");
					printf("Child Process Got Piped \n");
				}

				else if(WTERMSIG(stat) == 14){
					printf("Child Process Receive SIGALRM Signal \n");
					printf("Child Process Is Alarmed \n");
				}

				else if(WTERMSIG(stat) == 15){
					printf("Child Process Receive SIGTERM Signal \n");
					printf("Child Process Is Terminated \n");
				}

				printf("Execution Failed \n");
			}

			else if(WIFSTOPPED(stat)){
				printf("Child Process Receive SIGSTOP Signal\n ");
				printf("Child Process Stopped \n");
			}
			
			else{
				printf("Child Process Continue \n");
			}
			exit(0);
		}
	}
	else if(identifier == 0){
		perror("fork");
		exit(1);
	}

	
	/* check child process'  termination status */
	
	
}
