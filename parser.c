/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

#include "main.h"

void getTokens(struct token * head){
    char c;
    int i;
    boolean overflowWarning = false;
    boolean overflowTokenWarning = false;
    boolean reading = false;
    boolean quotation = false;
    
    printf("shell> ");
    
    while((c = getchar()) != '\n'){
        switch (c){
            case '"':
                quotation = !quotation; 
                break;
            case '\t':
            case ' ':
                if(!quotation){
                    if(reading){
                        overflowTokenWarning = reading = false;
                    }
                    break;
                }
            default:
                if(reading){
                    if(i < TOKEN_STRING_SIZE){
                        head->string[i++] = c;
                    }else if(!overflowWarning){
                        printf(
                            "warning> words can't have more them %d characters\n"
                            "warning> %s%c... will be reduced to %s\n",
                            TOKEN_STRING_SIZE,
                            head->string,c,head->string
                        );
                        overflowWarning = true;
                        overflowTokenWarning = true;
                    }
                    else if(!overflowTokenWarning){
                        printf(
                            "warning> %s%c... will be reduced to %s\n",
                            head->string,c,head->string
                        );
                        overflowTokenWarning = true;
                    }
                }else{
                    i = 0;
                    head->next = (void *)calloc(1,sizeof(struct token));
                    head = head->next;
                    head->string[i++] = c;
                    reading = true;
                }
        }
    }
    if(quotation){
        printf("warning> closing \" not found, using end of command instead\n");
    }
}

void identifyTokens(struct token * head){
    while(head = head->next){
        if(!strcmp(head->string,"|")) head->type = tokenPipe;
        else if(!strcmp(head->string,"<")) head->type = tokenInput;
        else if(!strcmp(head->string,">")) head->type = tokenOutput;
        else if(!strcmp(head->string,">>")) head->type = tokenAppend;
        else if(!strcmp(head->string,"&")) head->type = tokenBackground;
        else head->type = tokenWord;
    }
}

boolean checkSyntax(struct token * head){
	tokenType expectation = tokenCommand;
    boolean error = false;
    boolean haveInput = false;
    boolean haveOutput = false;
    boolean haveAppend = false;
    boolean haveBackground = false;
    boolean anyCommand = false;
    boolean backgroundWarning = false;
	boolean havePipe = false;
    
    while(head = head->next){
        switch(expectation){
            case tokenCommand:
                switch(head->type){
                    case tokenWord:
                        head->type = tokenCommand;
                        expectation = tokenArgument;
                        anyCommand = true;
                        break;
                    default:
                        printf(
                            "error> expected a command, but found %s\n",
                            head->string
                        );
                        error = true;
                }
                break;
            case tokenArgument:
                switch(head->type){
                    case tokenWord:
                        head->type = tokenArgument;
                        break;
                    case tokenPipe:
						if(!haveOutput){
		                    expectation = tokenCommand;
		                    haveInput = false;
		                    haveOutput = false;
		                    haveAppend = false;
		                    haveBackground = false;
							havePipe = true;
						}
						else{
                            printf("error> can't redirect output twice\n");
                            error = true;
						}
                        break;
                    case tokenInput:
                        if(haveInput || havePipe){
                            printf("error> can't redirect input twice\n");
                            error = true;
                            break;
                        }
                        haveInput = true;
                        expectation = tokenFile;
                        break;
                    case tokenOutput:
                        if(haveOutput || haveAppend){
                            printf("error> can't redirect output twice\n");
                            error = true;
                            break;
                        }
                        haveOutput = true;
                        expectation = tokenFile;
                        break;
                    case tokenAppend:
                        if(haveOutput || haveAppend){
                            printf("error> can't redirect output twice\n");
                            error = true;
                            break;
                        }
                        haveAppend = true;
                        expectation = tokenFile;
                        break;
                    case tokenBackground:
                        if(haveBackground && !backgroundWarning){
                            printf("warning> & used more them one time\n");
                            backgroundWarning = true;
                        }
                        haveBackground = true;
                        break;
                }
                break;
            case tokenFile:
                switch(head->type){
                    case tokenWord:
                        head->type = tokenFile;
                        expectation = tokenArgument;
                        break;
                    default:
                        printf(
                            "error> expected a file name, but found %s\n",
                            head->string
                        );
                        error = true;
                }
                break;
        }
        if(error) break;
    }
    if(expectation != tokenArgument){
        error = true;
        switch(expectation){
            case tokenCommand:
                printf("error> command missing\n");
                break;
            case tokenFile:
                printf("error> file missing\n");
                break;
        }
    }
    return !error;
}

boolean getCommand(struct token * head, struct command * command)
{
    struct token * token = head->next;
    struct token * nextToken;
    struct argument * argument;
    struct argument * nextArgument;
    
    /* clear command */
    argument = command->argument.next;
    while(argument)
    {
        nextArgument = argument->next;
        free(argument);
        argument = nextArgument;
    }
    command->argument.next = NULL;
    (command->input)[0] = '\0';
    (command->output)[0] = '\0';
    (command->append)[0] = '\0';
    command->background = false;
    command->pipeInput = false;
    command->pipeOutput = false;
    
    if(!token) return false;
    
    if(token->type == tokenPipe){
        command->pipeInput = true;
        /* remove current token and go to the next */
        nextToken = token->next;
        free(token);
        token = nextToken;
    }
    
    /* get the command name */
    argument = &(command->argument);
    strcpy(argument->string,token->string);
    if(!strcmp(argument->string,"quit")) command->type = commandQuit;
    else if(!strcmp(argument->string,"exit")) command->type = commandExit;
    else command->type = commandSystem;
    
    /* remove current token and go to the next */
    nextToken = token->next;
    free(token);
    token = nextToken;
    
    while(token){
        switch(token->type){
            case tokenArgument:
                argument->next = (void *)calloc(1,sizeof(struct argument));
                argument = argument->next;
                
                strcpy(argument->string,token->string);
                
                /* remove current token and go to the next */
                nextToken = token->next;
                free(token);
                token = nextToken;
                
                break;
            case tokenInput:
                /* remove current token and go to the next */
                nextToken = token->next;
                free(token);
                token = nextToken;
                
                strcpy(command->input, token->string);
                
                /* remove current token and go to the next */
                nextToken = token->next;
                free(token);
                token = nextToken;
                
                break;
            case tokenOutput:
                /* remove current token and go to the next */
                nextToken = token->next;
                free(token);
                token = nextToken;
                
                strcpy(command->output, token->string);
                
                /* remove current token and go to the next */
                nextToken = token->next;
                free(token);
                token = nextToken;
                
                break;
            case tokenAppend:
                /* remove current token and go to the next */
                nextToken = token->next;
                free(token);
                token = nextToken;
                
                strcpy(command->append, token->string);
                
                /* remove current token and go to the next */
                nextToken = token->next;
                free(token);
                token = nextToken;
                
                break;
            case tokenBackground:
                command->background = true;
            
                /* remove current token and go to the next */
                nextToken = token->next;
                free(token);
                token = nextToken;
                
                break;
            case tokenPipe:
                /* leave this token for the next command */
                head->next = token;
                command->pipeOutput = true;
                return true;
        }
    }
    head->next = token;
    return true;
}

void echoTokens(struct token * head){
    while(head = head->next){
        printf("parser> %s",head->string);
        switch(head->type){
            case tokenWord: printf("(word)"); break;
            case tokenCommand: printf("(command)"); break;
            case tokenArgument: printf("(argument)"); break;
            case tokenFile: printf("(file)"); break;
            case tokenInput: printf("(input)"); break;
            case tokenOutput: printf("(output)"); break;
            case tokenAppend: printf("(append)"); break;
            case tokenPipe: printf("(pipe)"); break;
            case tokenBackground: printf("(background)"); break;
        }
        putchar('\n');
    }
}

void echoInput(struct token * head)
{
    if(head->next){
        printf("echo>");
        while(head = head->next){
            printf(" %s", head->string);
        }
        putchar('\n');
    }
}

