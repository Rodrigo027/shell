/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

#include "main.h"

boolean shellExit(struct command command){
    if(command.argNumber != 1){
        printf("error> exit don't have arguments\n");
		return false;
    }
    return true;
}

