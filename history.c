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
	struct command * lastCommand;

    if(headCommand.argNumber != 1){
        printf("warning> history don't have arguments\n");
    }

	lastCommand = &headCommand;
	while(lastCommand -> next){
		lastCommand = lastCommand -> next;
	}
	argument = &(lastCommand->argument);
	while(argument){
		printf("%s ",argument->string);
		argument = argument->next;
	}
	if(lastCommand -> pipeOutput){
		printf("| ");
	} else {
		printf("\n");
	}

	while(command = &headCommand, command != lastCommand){
		while(command -> next != lastCommand){
			command = command -> next;
		}
		lastCommand = command;
		argument = &(lastCommand->argument);
		while(argument){
			printf("%s ",argument->string);
			argument = argument->next;
		}
		if(lastCommand -> pipeOutput){
			printf("| ");
		} else {
			printf("\n");
		}
	}
}

