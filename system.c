/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

#include "main.h"

void shellSystem(struct command command){
		execvp(command.arg[0],command.arg);
		printf("error> command \"%s\" not found.\n",command.arg[0]);
		exit(1);
}

