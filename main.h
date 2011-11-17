/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

/* Standard C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* process */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* I/O */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* signal */
#include <signal.h>

/* lib */
#include <dlfcn.h>

#define TOKEN_STRING_SIZE 100
#define ECHO_TOKENS false
#define ECHO_INPUT false
#define ECHO_COMMAND false

typedef enum boolean {false,true} boolean;

/* word can be a command, argument or file, it is mark for further process */
typedef enum{
    tokenCommand, tokenArgument, tokenFile, tokenWord, tokenInput, tokenOutput,
	tokenAppend, tokenPipe, tokenBackground
}tokenType;

/* linked list of tokens, the first token is always invalid (head) */
struct token{
    tokenType type;
    char string[TOKEN_STRING_SIZE];
    struct token * next;
};

/* linked list of arguments, the first argument is always valid */
struct argument{
    char string[TOKEN_STRING_SIZE];
    struct argument * next;
};

/* linked list of commands, the first command is always invalid (head) */
struct command{

    enum {
		commandQuit,commandExit,commandTest,commandSystem,commandPwd,commandCd,
		commandHistory,commandJobs,commandBg,commandFg,commandKill
	} type; /* system for not built in commands */

    struct argument argument; /* first argument is the name of the command */
	char ** arg;
	int argNumber;

	int id;
	enum { finish , suspended , running , foreground } status ;

	char input[TOKEN_STRING_SIZE]; /* empty string when not redirected */
	char output[TOKEN_STRING_SIZE]; /* empty string when not redirected */
	char append[TOKEN_STRING_SIZE]; /* empty string when not redirected */
    boolean pipeInput;
    boolean pipeOutput;
    boolean background;

	struct command * next;
};

/* core */
boolean executeCommand(struct command * command);
boolean executeAsParent(struct command * command, int pipeInput, int pipeOutput);
void executeAsChild(struct command * command, int pipeInput, int pipeOutput);

/* sinais */
void terminateHandler(int signalNumber);
void terminateCommand(struct command * argCommand);
void stopHandler(int signalNumber);
void stopCommand(struct command * argCommand);

/* parser */
void getTokens(struct token * head);
void identifyTokens(struct token * head);
boolean checkSyntax(struct token * head);
boolean getCommand(struct token * head, struct command * command); /* returns true when there are one or more commands left */
void echoTokens(struct token * head);
void echoInput(struct token * head);
void clearCommand(struct command * command);
void clearToken(struct token * token);

/* commands */
boolean shellExit(struct command command);
boolean shellQuit(struct command command);
void pwd(struct command command);
void cd(struct command command);
void jobs(struct command command);
void shellSystem(struct command command);
void bg(struct command headCommand);
void fg(struct command headCommand);
void shellKill(struct command command);

