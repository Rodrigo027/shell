/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

#include "main.h"

void shellSystem(struct command command){
	static int fd[2];

	/* file descriptors */
	int fdInput = 0;
	int fdOutput = 0;
	int fdAppend = 0;

	/* command arguments array */
	char ** args;
	int count = 0;
	int i;
	struct argument * argument;

	/* process */
	pid_t pid;
	int status;

	/* count number of arguments */
	argument = &(command.argument);
	while(argument){
		++count;
		argument = argument->next;
	}
	args = (void *)calloc(count+1,sizeof(char *));
	for(i = 0 ; i < count ; ++i){
		args[i] = calloc(TOKEN_STRING_SIZE,sizeof(char));
	}

	/* copy arguments */
	count = 0;
	argument = &(command.argument);
	while(argument){
		strcpy(args[count],argument->string);
		argument = argument->next;
		++count;
	}

	pid = fork();

	if(pid < 0){ /* erro */
		printf("error> in %s at line %d\n",__FILE__,__LINE__);
		exit(1);
	}

	if(pid == 0){ /* filho */
		if((command.input)[0]){
			fdInput = open(command.input, O_RDONLY, 0666);
			dup2(fdInput,0);printf("hello\n");
		}
		if((command.output)[0]){
			fdOutput = open(command.output, O_RDWR | O_CREAT | O_TRUNC, 0666);
			dup2(fdOutput,1);
		}
		if((command.append)[0]){
			fdAppend = open(command.append, O_RDWR | O_CREAT | O_APPEND, 0666);
			dup2(fdAppend,1);
		}
		if(command.pipeInput){	
			close(fd[1]);
			dup2(fd[0],0);	
		}
		if(command.pipeOutput){
			pipe(fd);
			close(fd[0]);
			dup2(fd[1],1);
		}
		execvp(args[0],args);
	}

	if(pid > 0){ /* pai */
		if(command.background){
			waitpid(-1,&status,WNOHANG);
		}else{
			wait(&status);
		}
	}

	if(fdInput) close(fdInput);
	if(fdOutput) close(fdOutput);
	if(fdAppend) close(fdAppend);
}

