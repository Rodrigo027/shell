/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

#include "main.h"

void f_get_tokens(t_token * a_head)
{
    char c;
    int i;
    t_boolean overflow_warning = false;
    t_boolean overflow_token_warning = false;
    t_boolean reading = false;
    t_boolean quotation = false;
    
    printf("shell> ");
    
    while((c = getchar()) != '\n')
    {
        switch (c)
        {
            case '"':
                quotation = !quotation; 
                break;
            case '\t':
            case ' ':
                if(!quotation)
                {
                    if(reading)
                    {
                        overflow_token_warning = reading = false;
                    }
                    break;
                }
            default:
                if(reading)
                {
                    if(i < TOKEN_STRING_SIZE)
                    {
                        a_head->a_string[i++] = c;
                    }
                    else if(!overflow_warning)
                    {
                        printf
                        (
                            "warning> words can't have more them %d characters\n"
                            "warning> %s%c... will be reduced to %s\n",
                            TOKEN_STRING_SIZE,
                            a_head->a_string,c,a_head->a_string
                        );
                        overflow_warning = true;
                        overflow_token_warning = true;
                    }
                    else if(!overflow_token_warning)
                    {
                        printf
                        (
                            "warning> %s%c... will be reduced to %s\n",
                            a_head->a_string,c,a_head->a_string
                        );
                        overflow_token_warning = true;
                    }
                }
                else
                {
                    i = 0;
                    a_head->a_next = (void *)calloc(1,sizeof(t_token));
                    a_head = a_head->a_next;
                    a_head->a_string[i++] = c;
                    reading = true;
                }
        }
    }
    if(quotation)
    {
        printf("warning> closing \" not found, using end of command instead\n");
    }
}

void f_identify_tokens(t_token * a_head)
{
    while(a_head = a_head->a_next)
    {
        if(!strcmp(a_head->a_string,"|")) a_head->type = e_pipe;
        else if(!strcmp(a_head->a_string,"<")) a_head->type = e_input;
        else if(!strcmp(a_head->a_string,">")) a_head->type = e_output;
        else if(!strcmp(a_head->a_string,">>")) a_head->type = e_append;
        else if(!strcmp(a_head->a_string,"&")) a_head->type = e_background;
        else a_head->type = e_word;
    }
}

t_boolean f_check_syntax(t_token * a_head)
{
    t_boolean error = false;
    token_type expectation = e_command;
    t_boolean have_input = false;
    t_boolean have_output = false;
    t_boolean have_append = false;
    t_boolean have_background = false;
    t_boolean any_command = false;
    t_boolean background_warning = false;
	t_boolean have_pipe = false;
    
    while(a_head = a_head->a_next)
    {
        switch(expectation)
        {
            case e_command:
                switch(a_head->type)
                {
                    case e_word:
                        a_head->type = e_command;
                        expectation = e_argument;
                        any_command = true;
                        break;
                    default:
                        printf
                        (
                            "error> expected a command, but found %s\n",
                            a_head->a_string
                        );
                        error = true;
                }
                break;
            case e_argument:
                switch(a_head->type)
                {
                    case e_word:
                        a_head->type = e_argument;
                        break;
                    case e_pipe:
						if(!have_output)
						{
		                    expectation = e_command;
		                    have_input = false;
		                    have_output = false;
		                    have_append = false;
		                    have_background = false;
							have_pipe = true;
						}
						else
						{
                            printf("error> can't redirect output twice\n");
                            error = true;
						}
                        break;
                    case e_input:
                        if(have_input || have_pipe)
                        {
                            printf("error> can't redirect input twice\n");
                            error = true;
                            break;
                        }
                        have_input = true;
                        expectation = e_file;
                        break;
                    case e_output:
                        if(have_output || have_append)
                        {
                            printf("error> can't redirect output twice\n");
                            error = true;
                            break;
                        }
                        have_output = true;
                        expectation = e_file;
                        break;
                    case e_append:
                        if(have_output || have_append)
                        {
                            printf("error> can't redirect output twice\n");
                            error = true;
                            break;
                        }
                        have_append = true;
                        expectation = e_file;
                        break;
                    case e_background:
                        if(have_background && !background_warning)
                        {
                            printf("warning> & used more them one time\n");
                            background_warning = true;
                        }
                        have_background = true;
                        break;
                }
                break;
            case e_file:
                switch(a_head->type)
                {
                    case e_word:
                        a_head->type = e_file;
                        expectation = e_argument;
                        break;
                    default:
                        printf
                        (
                            "error> expected a file name, but found %s\n",
                            a_head->a_string
                        );
                        error = true;
                }
                break;
        }
        if(error) break;
    }
    if(expectation != e_argument)
    {
        error = true;
        switch(expectation)
        {
            case e_command:
                printf("error> command missing\n");
                break;
            case e_file:
                printf("error> file missing\n");
                break;
        }
    }
    return !error;
}

t_boolean f_get_command(t_token * a_head,t_command * a_command)
{
    t_token * a_token = a_head->a_next;
    t_token * a_next_token;
    t_argument * a_argument;
    t_argument * a_next_argument;
    
    /* clear command */
    a_argument = a_command->argument.a_next;
    while(a_argument)
    {
        a_next_argument = a_argument->a_next;
        free(a_argument);
        a_argument = a_next_argument;
    }
    a_command->argument.a_next = NULL;
    (a_command->a_input)[0] = '\0';
    (a_command->a_output)[0] = '\0';
    (a_command->a_append)[0] = '\0';
    a_command->background = false;
    a_command->pipe_input = false;
    a_command->pipe_output = false;
    
    if(!a_token) return false;
    
    if(a_token->type == e_pipe)
    {

        a_command->pipe_input = true;
        
        /* remove current token and go to the next */
        a_next_token = a_token->a_next;
        free(a_token);
        a_token = a_next_token;
    }
    
    /* get the command name */
    a_argument = &(a_command->argument);
    strcpy(a_argument->a_string,a_token->a_string);
    if(!strcmp(a_argument->a_string,"quit")) a_command->type = e_quit;
    else if(!strcmp(a_argument->a_string,"exit")) a_command->type = e_exit;
    else if(!strcmp(a_argument->a_string,"test")) a_command->type = e_test;
    else a_command->type = e_system;
    
    /* remove current token and go to the next */
    a_next_token = a_token->a_next;
    free(a_token);
    a_token = a_next_token;
    
    while(a_token)
    {
        switch(a_token->type)
        {
            case e_argument:
                a_argument->a_next = (void *)calloc(1,sizeof(t_argument));
                a_argument = a_argument->a_next;
                
                strcpy(a_argument->a_string,a_token->a_string);
                
                /* remove current token and go to the next */
                a_next_token = a_token->a_next;
                free(a_token);
                a_token = a_next_token;
                
                break;
            case e_input:
                /* remove current token and go to the next */
                a_next_token = a_token->a_next;
                free(a_token);
                a_token = a_next_token;
                
                strcpy(a_command->a_input,a_token->a_string);
                
                /* remove current token and go to the next */
                a_next_token = a_token->a_next;
                free(a_token);
                a_token = a_next_token;
                
                break;
            case e_output:
                /* remove current token and go to the next */
                a_next_token = a_token->a_next;
                free(a_token);
                a_token = a_next_token;
                
                strcpy(a_command->a_output,a_token->a_string);
                
                /* remove current token and go to the next */
                a_next_token = a_token->a_next;
                free(a_token);
                a_token = a_next_token;
                
                break;
            case e_append:
                /* remove current token and go to the next */
                a_next_token = a_token->a_next;
                free(a_token);
                a_token = a_next_token;
                
                strcpy(a_command->a_append,a_token->a_string);
                
                /* remove current token and go to the next */
                a_next_token = a_token->a_next;
                free(a_token);
                a_token = a_next_token;
                
                break;
            case e_background:
                a_command->background = true;
            
                /* remove current token and go to the next */
                a_next_token = a_token->a_next;
                free(a_token);
                a_token = a_next_token;
                
                break;
            case e_pipe:
                /* leave this token for the next command */
                a_head->a_next = a_token;
                a_command->pipe_output = true;
                return true;
        }
    }
    a_head->a_next = a_token;
    return true;
}

void f_echo_tokens(t_token * a_head)
{
    while(a_head = a_head->a_next)
    {
        printf("parser> %s",a_head->a_string);
        switch(a_head->type)
        {
            case e_word: printf("(word)"); break;
            case e_command: printf("(command)"); break;
            case e_argument: printf("(argument)"); break;
            case e_file: printf("(file)"); break;
            case e_input: printf("(input)"); break;
            case e_output: printf("(output)"); break;
            case e_append: printf("(append)"); break;
            case e_pipe: printf("(pipe)"); break;
            case e_background: printf("(background)"); break;
        }
        putchar('\n');
    }
}

void f_echo_input(t_token * a_head)
{
    if(a_head->a_next)
    {
        printf("echo>");
        while(a_head = a_head->a_next)
        {
            printf(" %s",a_head->a_string);
        }
        putchar('\n');
    }
}
