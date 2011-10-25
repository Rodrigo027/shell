/*
	Nome: Rodrigo Fernandes da Costa
	n. USP: 6793073

	Nome: Lucas de Moraes Franco
	n. USP: 6793239
*/

#include "main.h"

void cd(struct command command){
    if(command.argNumber == 1){
        printf("error> missing argument\n");
		return;
    }
    if(command.argNumber > 2){
        printf("warning> cd only has one argument\n");
    }
    if(chdir(command.arg[1]) == -1){
        printf("error> directory \"%s\" does not exist\n", command.arg[1]);
    }
    return;
}
