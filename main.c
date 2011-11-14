/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239

    Modificação
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
<<<<<<< HEAD
			if(ECHO_COMMAND) echoCommand(*(command.next));
=======
>>>>>>> 9f59be43605538ee81fd9d29ac0eeb91a8841103
			end = executeCommand(*(command.next));
        }
    }
    while(!end);

	clearToken(&token);
<<<<<<< HEAD
=======
	//finishChildCommand(&command);
>>>>>>> 9f59be43605538ee81fd9d29ac0eeb91a8841103
	clearCommand(&command);
    
	return 0;
}

boolean executeCommand(struct command command){
	boolean end = false;
<<<<<<< HEAD
	static int nextPipe[2];
	static int previousPipe[2];
	int pipeInput;
	int pipeOutput;

	if(command.pipeOutput){
		pipe(nextPipe);
		pipeOutput = nextPipe[1];
	}

	if(command.pipeInput){
		pipeInput = previousPipe[0];
	}
=======
	int fd[2];
>>>>>>> 9f59be43605538ee81fd9d29ac0eeb91a8841103

	switch(command.type){
		case commandExit:
		case commandQuit:
		case commandCd:
<<<<<<< HEAD
			end = executeAsParent(command, pipeInput, pipeOutput);
			break;
		default:
			executeAsChild(command, pipeInput, pipeOutput);
	}

	if(command.pipeOutput){
		close ( nextPipe[1] );
	}

	if(command.pipeInput){
		close ( previousPipe[0] );
	}

	previousPipe[0] = nextPipe[0] ;
	previousPipe[1] = nextPipe[1] ;

	return end;
}

boolean executeAsParent(struct command command, int pipeInput, int pipeOutput){
=======
			end = executeAsParent(command, fd);
			break;
		default:
			executeAsChild(command, fd);
	}

	return end;
}

boolean executeAsParent(struct command command, int * fd){
>>>>>>> 9f59be43605538ee81fd9d29ac0eeb91a8841103
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
<<<<<<< HEAD
		dup2(pipeInput, 0);
	}
	if(command.pipeOutput){
		dup2(pipeOutput, 1);
=======
		close(fd[1]);
		dup2(fd[0], 0);
	}
	if(command.pipeOutput){
		pipe(fd);
		close(fd[0]);
		dup2(fd[1], 1);
>>>>>>> 9f59be43605538ee81fd9d29ac0eeb91a8841103
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
<<<<<<< HEAD
		dup2(0, pipeInput);
	}
	if(command.pipeOutput){
		dup2(1, pipeOutput);
=======
		dup2(0, fd[0]);
	}
	if(command.pipeOutput){
		dup2(1, fd[1]);
>>>>>>> 9f59be43605538ee81fd9d29ac0eeb91a8841103
	}

	return end;
}

<<<<<<< HEAD
void executeAsChild(struct command command, int pipeInput, int pipeOutput){
=======
void executeAsChild(struct command command, int * fd){
>>>>>>> 9f59be43605538ee81fd9d29ac0eeb91a8841103
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
<<<<<<< HEAD
			dup2(pipeInput,0);
		}
		if(command.pipeOutput){
			dup2(pipeOutput,1);
=======
			close(fd[1]);
			dup2(fd[0],0);
		}
		if(command.pipeOutput){
			pipe(fd);
			close(fd[0]);
			dup2(fd[1],1);
>>>>>>> 9f59be43605538ee81fd9d29ac0eeb91a8841103
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

