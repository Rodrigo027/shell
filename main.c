/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

#include "main.h"

int main(void)
{
    t_token token = {0};
    t_command command = {0};
    t_boolean end = false;
    t_boolean echo_input = ECHO_INPUT;
    
    do
    {
        f_get_tokens(&token);
        f_identify_tokens(&token);
        if(!f_check_syntax(&token)) continue;
        if(ECHO_TOKENS) f_echo_tokens(&token);
        if(echo_input) f_echo_input(&token);
        while(f_get_command(&token,&command))
        {
            switch(command.type)
            {
                case exit: end = f_exit(command); break;
                case quit: end = f_quit(command); break;
                case echo: f_echo(command); break;
                case test: f_test(command); break;
                case system: f_system(command); break;
            }
        }
    }
    while(!end);
    
	return 0;
}
