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
    
    do{
        getTokens(&token);
        identifyTokens(&token);
        if(!checkSyntax(&token)) continue;
        if(ECHO_TOKENS) echoTokens(&token);
        if(echoInputEnable) echoInput(&token);
        while(getCommand(&token,&command)){
            switch(command.type){
                case commandExit: end = shellExit(command); break;
                case commandQuit: end = shellQuit(command); break;
                case commandSystem: shellSystem(command); break;
		case commandPwd: pwd(command); break;
		case commandCd: cd(command); break;
            }
        }
    }
    while(!end);
    
	return 0;
}

