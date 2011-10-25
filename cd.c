/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

#include "main.h"

void cd(struct command command){ 
    int count = 0;
    struct argument * argument;

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

    if(count > 2){
        printf("warning> cd only has one argument\n");
    }
    if(chdir(command.arg[1]) == NULL){
        printf("error> directory \"%s\" does not exist\n", command.arg[1]);
    }
    return;
}

