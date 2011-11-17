/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

#include "main.h"

void shellKill(struct command headCommand){
	struct argument * argument;
	struct command * command;
	int id;

	if(headCommand.argNumber != 2 || (!sscanf(headCommand.argument.next->string,"%d",&id))){
		printf("error> expected \"kill process number\", but found: ");
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
			if(command->status == running || command->status == suspended){
				kill(command->id,SIGTERM);
				return;
			}
		}
	}
	printf("error> %d not found\n", id);
}
