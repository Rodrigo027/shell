/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

#include "main.h"

void f_test2(t_command command)
{
	int fd_input = 0;
	int fd_output = 0;
	int fd_append = 0;
	int status;
	pid_t pid;

	pid = fork();

	if(pid < 0) /* erro */
	{
		printf("error> in %s at line %d\n",__FILE__,__LINE__);
		exit(1);
	}

	if(pid == 0) /* filho */
	{
		if((command.a_input)[0])
		{
			fd_input = open(command.a_input, O_RDONLY, 0666);
			dup2(fd_input,0);
		}
		if((command.a_output)[0])
		{
			fd_output = open(command.a_input, O_RDWR | O_CREAT | O_TRUNK, 0666);
			dup2(fd_output,1);
		}
		if((command.fd_append)[0])
		{
			fd_append = open(command.a_input, O_RDWR | O_CREAT | O_APPEND, 0666);
			dup2(fd_append,1);
		}
		execvp("echo","echo");
	}

	if(pid > 0) /* pai */
	{
		if(command.background)
		{
			waitpid(-1,&status,WNOHANG);
		}
		else
		{
			wait(&status);
		}
	}

	if(fd_input) close(fd_input);
	if(fd_output) close(fd_input);
	if(fd_append) close(fd_append);
}

