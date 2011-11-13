/*
	Nome: Rodrigo Fernandes da Costa
	n. USP: 6793073

	Nome: Lucas de Moraes Franco
	n. USP: 6793239
*/

#include "main.h"

void history(struct command headCommand){
	struct argument * argument;
	struct command * command;

	command = &headCommand;
    while(command){
		argument = &(command->argument);
		while(argument){
			printf("%s ",argument->string);
			argument = argument->next;
		}
		printf("\n");
		command = command -> next;
	}
}

