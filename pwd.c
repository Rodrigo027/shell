/*
	Nome: Rodrigo Fernandes da Costa
	n. USP: 6793073

	Nome: Lucas de Moraes Franco
	n. USP: 6793239
*/

#include "main.h"

void pwd(struct command command){
    char path[100];

    if(getcwd(path,sizeof(path)) == NULL)
		printf("error> pwd execution unsuccessful.\n");
    
    printf("%s\n",path);
    return;
}

