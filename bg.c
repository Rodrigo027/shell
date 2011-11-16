/*
	Nome: Rodrigo Fernandes da Costa
	n. USP: 6793073

	Nome: Lucas de Moraes Franco
	n. USP: 6793239
*/

#include "main.h"

void bg(struct command headCommand){
	struct argument * argument;
	struct command * command;
	int id;

	if(headCommand.argNumber != 2 || (!sscanf(headCommand.argument.next->string,"%d",&id))){
		printf("error> expected \"bg processNumber\", but found: ");
		argument = & ( headCommand . argument ) ;
		while ( argument ) {
			printf("%s ",argument->string);
			argument = argument -> next ;
		}
		printf("\n");
		return;
	}

	command = & headCommand;
    while(command = command -> next){
		if(command->id == id){
			if(command->status == running){
				printf("error> %d is already running on background\n", id);
				return;
			}
			if(command->status != suspended){
				printf("error> %d is not suspended\n", id);
				return;
			}
			command->status = running;
			kill(command->id, SIGCONT);
			return;
		}
	}
	printf("error> %d not found\n", id);
}

