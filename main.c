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
    boolean echoInputEnable = ECHO_INPUT;
	int i = 1;

	/* I/O */
	int fdInput = 0;
	int fdOutput = 0;
	int fdAppend = 0;

	/* process */
	pid_t pid;

	/* pipe */
	int fd[2];
    
    do{
        getTokens(&token);
        identifyTokens(&token);
        if(!checkSyntax(&token)) continue;
        if(ECHO_TOKENS) echoTokens(&token);
        if(echoInputEnable) echoInput(&token);
        while(getCommand(&token,&command)){
			fdInput = 0;
			fdOutput = 0;
			fdAppend = 0;
			pid = fork();
			if(pid < 0){ /* erro */
				printf("error> in %s at line %d\n", __FILE__, __LINE__);
				exit(1);
			}
			if(pid == 0){ /* filho */
				i = 2;
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
		            case commandExit: end = shellExit(command); break;
	        	    case commandQuit: end = shellQuit(command); break;
            	    case commandSystem: shellSystem(command); break;
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
				    switch(command.type){
				        case commandExit: end = true; break;
			    	    case commandQuit: end = true; break;
						case commandCd: cd(command); break;
				    }
				}
			}
        }
    }
    while(!end);
    
	return 0;
}

