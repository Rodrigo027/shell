/*
	Nome: Rodrigo Fernandes da Costa
	n. USP: 6793073

	Nome: Lucas de Moraes Franco
	n. USP: 6793239
*/

#include "main.h"

void fg(struct command headCommand){
	struct argument * argument;
	struct command * command;
	int id;

	if(headCommand.argNumber != 2 || (!sscanf(headCommand.argument.next->string,"%d",&id))){
		printf("error> expected \"fg processNumber\", but found: ");
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
				command->status = foreground;
				waitpid(command->id,NULL,WUNTRACED);
				if(command -> status != suspended){
					command -> status = finish;
				}
				fflush(stdin);
				return;
			}
			if(command->status == suspended){
				command->status = foreground;
				kill(command->id, SIGCONT);
				waitpid(command->id,NULL,0);
				if(command -> status != suspended){
					command -> status = finish;
				}
				fflush(stdin);
				return;
			}
			printf("error> %d is not suspended or running\n", id);
		}
	}
	printf("error> %d not found\n", id);
}

