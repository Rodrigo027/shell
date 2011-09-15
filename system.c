/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

#include "main.h"

void f_system(t_command command)
{
	// int fd[2];
	// pipe(fd);

	/* file descriptors */
	int fd_input = 0;
	int fd_output = 0;
	int fd_append = 0;

	/* command arguments array */
	char ** aa_args;
	int count = 0;
	int i;
	t_argument * a_argument;

	/* process */
	pid_t pid;
	int status;

	/* count number of arguments */
	a_argument = &(command.argument);
	while(a_argument)
	{
		++count;
		a_argument = a_argument->a_next;
	}
	aa_args = (void *)calloc(count+1,sizeof(char *));
	for(i = 0 ; i < count ; ++i)
	{
		aa_args[i] = calloc(TOKEN_STRING_SIZE,sizeof(char));
	}

	/* copy arguments */
	count = 0;
	a_argument = &(command.argument);
	while(a_argument)
	{
		strcpy(aa_args[count],a_argument->a_string);
		a_argument = a_argument->a_next;
		++count;
	}

	pid = fork();

	if(pid < 0) /* erro */
	{
		printf("error> in %s at line %d\n",__FILE__,__LINE__);
		exit(1);
	}

	if(pid == 0) /* filho */
	{
		// close(fd[0]);
		// dup2(fd[1],1);
		if((command.a_input)[0])
		{
			fd_input = open(command.a_input, O_RDONLY, 0666);
			dup2(fd_input,0);printf("hello\n");
		}
		if((command.a_output)[0])
		{
			fd_output = open(command.a_output, O_RDWR | O_CREAT | O_TRUNC, 0666);
			dup2(fd_output,1);
		}
		if((command.a_append)[0])
		{
			fd_append = open(command.a_append, O_RDWR | O_CREAT | O_APPEND, 0666);
			dup2(fd_append,1);
		}
		if(command.pipe_input)
		{
			
		}
		if(command.pipe_output)
		{
			
		}
		execvp(aa_args[0],aa_args);
	}

	if(pid > 0) /* pai */
	{
		// close(fd[1]);
		// dup2(fd[0],0);
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
