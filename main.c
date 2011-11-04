/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

#include "main.h"

int main(void){
    struct token token = {0};
    struct command command = {0};
    boolean end = false;
    
    do{
        getTokens(&token);
        identifyTokens(&token);
        if(!checkSyntax(&token)) continue;
        if(ECHO_TOKENS) echoTokens(&token);
        if(ECHO_INPUT) echoInput(&token);
        while(getCommand(&token, &command)){
			end = executeCommand(*(command.next));
        }
    }
    while(!end);

	clearToken(&token);
	//finishChildCommand(&command);
	clearCommand(&command);
    
	return 0;
}

boolean executeCommand(struct command command){
	boolean end = false;
	int fd[2];

	switch(command.type){
		case commandExit:
		case commandQuit:
		case commandCd:
			end = executeAsParent(command, fd);
			break;
		default:
			executeAsChild(command, fd);
	}

	return end;
}

boolean executeAsParent(struct command command, int * fd){
	int fdInput = 0;
	int fdOutput = 0;
	int fdAppend = 0;

	boolean end = false;

	if((command.input)[0]){
		fdInput = open(command.input, O_RDONLY, 0666);
		dup2(fdInput, 0);
	}
	if((command.output)[0]){
		fdOutput = open(command.output, O_RDWR | O_CREAT | O_TRUNC, 0666);
		dup2(fdOutput, 1);
	}
	if((command.append)[0]){
		fdAppend = open(command.append, O_RDWR | O_CREAT | O_APPEND, 0666);
		dup2(fdAppend, 1);
	}
	if(command.pipeInput){
		close(fd[1]);
		dup2(fd[0], 0);
	}
	if(command.pipeOutput){
		pipe(fd);
		close(fd[0]);
		dup2(fd[1], 1);
	}

	switch(command.type){
		case commandExit: end = shellExit(command); break;
		case commandQuit: end = shellQuit(command); break;
		case commandCd: cd(command); break;
	}

	if((command.input)[0]){
		dup2(0, fdInput);
		close(fdInput);
	}
	if((command.output)[0]){
		dup2(1, fdOutput);
		close(fdOutput);
	}
	if((command.append)[0]){;
		dup2(1, fdAppend);
		close(fdAppend);
	}
	if(command.pipeInput){
		dup2(0, fd[0]);
	}
	if(command.pipeOutput){
		dup2(1, fd[1]);
	}

	return end;
}

void executeAsChild(struct command command, int * fd){
	/* I/O */
	int fdInput = 0;
	int fdOutput = 0;
	int fdAppend = 0;

	/* process */
	pid_t pid;

	pid = fork();
	if(pid < 0){ /* erro */
		printf("error> in %s at line %d\n", __FILE__, __LINE__);
		exit(1);
	}
	if(pid == 0){ /* filho */
		if((command.input)[0]){
			fdInput = open(command.input, O_RDONLY, 0666);
			dup2(fdInput,0);
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

		switch(command.type){
			case commandSystem: shellSystem(command); break;
			case commandHistory: history(command); break;
			case commandPwd: pwd(command); break;
		}

		if(fdInput) close(fdInput);
		if(fdOutput) close(fdOutput);
		if(fdAppend) close(fdAppend);

		exit(0);
	}
	if(pid > 0){ /* pai */
		if(command.background){
			waitpid(-1,NULL,WNOHANG);
		}else{
			wait(NULL);
		}
	}
}

