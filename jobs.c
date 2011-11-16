/*
	Nome: Rodrigo Fernandes da Costa
	n. USP: 6793073

	Nome: Lucas de Moraes Franco
	n. USP: 6793239
*/

#include "main.h"

void jobs(struct command headCommand){
	struct argument * argument;
	struct command * command;

	command = & headCommand;
    while(command = command -> next){
		if ( command -> status == running ) {
			printf("jobs> %s running as %d \n" ,  command -> argument . string , command -> id);
		}
		else if ( command -> status == suspended ) {
			printf("jobs> %s suspended as %d \n" ,  command -> argument . string , command -> id);
		}
	}
}

