/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TOKEN_STRING_SIZE 100
#define ECHO_TOKENS false
#define ECHO_INPUT false

typedef enum boolean {false,true} t_boolean;

typedef char t_token_string[TOKEN_STRING_SIZE];

/* word can be a command, argument or file, it is mark for further process */
typedef enum
{
    e_command,e_argument,e_file,e_word,e_input,e_output,e_append,e_pipe,e_background
}
token_type;

/* linked list of tokens, the first token is always invalid (head) */
struct token
{
    token_type type;
    t_token_string a_string;
    struct token * a_next;
};

typedef struct token t_token;

/* linked list of arguments, the first argument is always valid */
struct argument
{
    t_token_string a_string;
    struct argument * a_next;
};

typedef struct argument t_argument;
    
struct command
{
    enum {e_quit,e_exit,e_test,e_system} type; /* system for not built in commands */
    t_argument argument; /* first argument is the name of the command */
    t_token_string a_input; /* empty string when not redirected */
    t_token_string a_output; /* empty string when not redirected */
    t_token_string a_append; /* empty string when not redirected */
    t_boolean pipe_input;
    t_boolean pipe_output;
    t_boolean background;
};


typedef struct command t_command;

/* parser */
void f_get_tokens(t_token * a_head);
void f_identify_tokens(t_token * a_head);
t_boolean f_check_syntax(t_token * a_head);
t_boolean f_get_command(t_token * a_head,t_command * a_command); /* returns true when there are one or more commands */
void f_echo_tokens(t_token * a_head);
void f_echo_input(t_token * a_head);

/* commands */
t_boolean f_exit(t_command command);
t_boolean f_quit(t_command command);
void f_echo(t_command command);
void f_system(t_command command);
void f_test(t_command command);
void f_test2(t_command command);
