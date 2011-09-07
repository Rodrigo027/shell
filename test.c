/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

#include "main.h"

void f_test(t_command command)
{
    t_argument * a_argument = &(command.argument);
    while(a_argument)
    {
        printf("argument> %s\n",a_argument->a_string);
        a_argument = a_argument->a_next;
    }
    printf("input> %s\n",command.a_input);
    printf("ouput> %s\n",command.a_output);
    printf("append> %s\n",command.a_append);
    printf("brackground> %d\n",command.background);
    printf("pipe input> %d\n",command.pipe_input);
    printf("pipe output> %d\n",command.pipe_output);
}
