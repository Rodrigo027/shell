/*
   Nome: Rodrigo Fernandes da Costa
   n. USP: 6793073

   Nome: Lucas de Moraes Franco
   n. USP: 6793239
*/

#include "main.h"

int main(void){

   struct token token = {0};
   struct command command = {0};
   boolean end = false;
   pid_t shell_pgid;
   int terminal;
   void *handle;

   terminal = STDIN_FILENO;

   while(tcgetpgrp(terminal) != (shell_pgid = getpgrp())){
       kill(- shell_pgid,SIGTTIN);
   }

   shell_pgid = getpid();
   if(setpgid(shell_pgid,shell_pgid) < 0){
       printf("Erro");
       exit(1);
   }

   tcsetpgrp(terminal,shell_pgid);

   terminateCommand ( & command );
   signal( SIGCHLD , terminateHandler );

   stopCommand( & command );
   signal ( SIGTSTP , stopHandler );

   handle = dlopen("./libparser.so", RTLD_LAZY);

   do{
       ((void(*)(struct token*))(dlsym(handle,"getTokens")))(&token);
       ((void(*)(struct token*))(dlsym(handle,"identifyTokens")))(&token);
       if(!((boolean(*)(struct token*))(dlsym(handle,"checkSyntax")))(&token)) continue;
       if(ECHO_TOKENS) ((void(*)(struct token*))(dlsym(handle,"echoTokens")))(&token);
       if(ECHO_INPUT) ((void(*)(struct token*))(dlsym(handle,"echoInput")))(&token);
       while(((boolean(*)(struct token*,struct command*))(dlsym(handle,"getCommand")))(&token, &command)){
           if(ECHO_COMMAND) ((void(*)(struct command))(dlsym(handle,"echoCommand")))(*(command.next));
           end = executeCommand(command.next);
       }
   }
   while(!end);

   ((void(*)(struct command*))(dlsym(handle,"clearCommand")))(&command);
   ((void(*)(struct token*))(dlsym(handle,"clearToken")))(&token);

   dlclose(handle);

   return 0;
}

boolean executeCommand(struct command * command){
   boolean end = false;
   static int nextPipe[2];
   static int previousPipe[2];
   int pipeInput;
   int pipeOutput;
   pid_t pipeGroup;
   boolean pipes;

   if(command->pipeOutput){
       pipe(nextPipe);
       pipeOutput = nextPipe[1];
   }

   if(command->pipeInput){
       pipeInput = previousPipe[0];
   }

   switch(command->type){
       case commandExit:
       case commandQuit:
       case commandCd:
       case commandFg:
           end = executeAsParent(command, pipeInput, pipeOutput);
           break;
       default:
           executeAsChild(command, pipeInput, pipeOutput);
   }

   if(command->pipeOutput){
       close ( nextPipe[1] );
   }

   if(command->pipeInput){
       close ( previousPipe[0] );
   }

   if(command->pipeInput || command->pipeOutput){
       if(pipes){
           setpgid(command->id,pipeGroup);
       } else {
           setpgid(command->id,command->id);
           pipeGroup = command->id;
           pipes = true;
       }
   } else {
       pipes = false;
   }

   previousPipe[0] = nextPipe[0] ;
   previousPipe[1] = nextPipe[1] ;

   return end;
}

boolean executeAsParent(struct command * command, int pipeInput, int pipeOutput){
   void *handle;
   int fdInput = 0;
   int fdOutput = 0;
   int fdAppend = 0;
   int stdoutSave;
   int stdinSave;

   boolean end = false;

   if((command->input)[0]){
       stdinSave = dup(0);
       fdInput = open(command->input, O_RDONLY, 0666);
       dup2(fdInput, 0);
   }
   if((command->output)[0]){
       stdoutSave = dup(1);
       fdOutput = open(command->output, O_RDWR | O_CREAT | O_TRUNC, 0666);
       dup2(fdOutput, 1);
   }
   if((command->append)[0]){
       stdoutSave = dup(1);
       fdAppend = open(command->append, O_RDWR | O_CREAT | O_APPEND, 0666);
       dup2(fdAppend, 1);
   }
   if(command->pipeInput){
       stdinSave = dup(0);
       dup2(pipeInput, 0);
   }
   if(command->pipeOutput){
       stdoutSave = dup(1);
       dup2(pipeOutput, 1);
   }

   switch(command->type){

       case commandExit:
           handle = dlopen("./libexit.so", RTLD_LAZY);
           end = ((boolean(*)(struct command))(dlsym(handle,"shellExit")))(*command);
           dlclose(handle);
           break;

       case commandQuit:
           handle = dlopen("./libquit.so", RTLD_LAZY);
           end = ((boolean(*)(struct command))(dlsym(handle,"shellQuit")))(*command);
           dlclose(handle);
           break;

       case commandCd:
           handle = dlopen("./libcd.so", RTLD_LAZY);
           ((void(*)(struct command))(dlsym(handle,"cd")))(*command);
           dlclose(handle);
           break;

       case commandFg:
           handle = dlopen("./libfg.so", RTLD_LAZY);
           ((void(*)(struct command))(dlsym(handle,"fg")))(*command);
           dlclose(handle);
           break;
   }

   if((command->input)[0]){
       dup2(stdinSave,0);
       close(fdInput);
   }
   if((command->output)[0]){
       dup2(stdoutSave,1);
       close(fdOutput);
   }
   if((command->append)[0]){;
       dup2(stdoutSave,1);
       close(fdAppend);
   }
   if(command->pipeInput){
       dup2(stdinSave,0);
   }
   if(command->pipeOutput){
       dup2(stdoutSave,1);
   }

   return end;
}

void executeAsChild(struct command * command, int pipeInput, int pipeOutput){
   /* lib */
   void * handle;

   /* I/O */
   int fdInput = 0;
   int fdOutput = 0;
   int fdAppend = 0;

   /* process */
   pid_t pid;

   command->id = pid = fork();

   if(pid < 0){ /* erro */
       printf("error> in %s at line %d\n", __FILE__, __LINE__);
       exit(1);
   }
   if(pid == 0){ /* filho */
       if((command->input)[0]){
           fdInput = open(command->input, O_RDONLY, 0666);
           dup2(fdInput,0);
       }
       if((command->output)[0]){
           fdOutput = open(command->output, O_RDWR | O_CREAT | O_TRUNC, 0666);
           dup2(fdOutput,1);
       }
       if((command->append)[0]){
           fdAppend = open(command->append, O_RDWR | O_CREAT | O_APPEND, 0666);
           dup2(fdAppend,1);
       }
       if(command->pipeInput){
           dup2(pipeInput,0);
       }
       if(command->pipeOutput){
           dup2(pipeOutput,1);
       }

       switch(command->type){

           case commandSystem:
               handle = dlopen("./libsystem.so", RTLD_LAZY);
               ((void(*)(struct command))(dlsym(handle,"shellSystem")))(*command);
               dlclose(handle);
               break;

           case commandPwd:
               handle = dlopen("./libpwd.so", RTLD_LAZY);
               ((void(*)(struct command))(dlsym(handle,"pwd")))(*command);
               dlclose(handle);
               break;

           case commandJobs:
               handle = dlopen("./libjobs.so", RTLD_LAZY);
               ((void(*)(struct command))(dlsym(handle,"jobs")))(*command);
               dlclose(handle);
               break;

           case commandBg:
               handle = dlopen("./libbg.so", RTLD_LAZY);
               ((void(*)(struct command))(dlsym(handle,"bg")))(*command);
               dlclose(handle);
               break;

           case commandHistory:
               handle = dlopen("./libhistory.so", RTLD_LAZY);
               ((void(*)(struct command))(dlsym(handle,"history")))(*command);
               dlclose(handle);
               break;

           case commandKill:
               handle = dlopen("./libkill.so", RTLD_LAZY);
               ((void(*)(struct command))(dlsym(handle,"shellKill")))(*command);
               dlclose(handle);
               break;
       }

       if(fdInput) close(fdInput);
       if(fdOutput) close(fdOutput);
       if(fdAppend) close(fdAppend);

       exit(0);
   }
   if(pid > 0){ /* pai */
       if(command->background){
           printf("jobs> %s running as %d \n" ,  command -> argument . string , command -> id);
           waitpid(-1,NULL,WNOHANG|WUNTRACED);
       }else{
           command -> status = foreground;
           waitpid(command->id,NULL,WUNTRACED);
           if(command -> status != suspended){
               command -> status = finish;
           }
           fflush(stdin);
       }
   }
}

void stopCommand(struct command * argCommand){
   static boolean initialized = false;
   static struct command * headCommand;
   struct command * command;
   int status;

   if(!initialized){
       headCommand = argCommand;
       initialized = true;
       return;
   }

   command = headCommand ;
   while(command = command -> next){
       if(command->status == foreground){
           command->status = suspended;
           printf("jobs> %s suspended as %d \n" ,  command -> argument . string , command -> id);
           kill(command->id, SIGSTOP);
       }
   }
}

void stopHandler(int signalNumber){
   stopCommand(NULL);
}

void terminateCommand(struct command * argCommand){
   static boolean initialized = false;
   static struct command * headCommand;
   struct command * command;
   int status;

   if(!initialized){
       headCommand = argCommand;
       initialized = true;
       return;
   }

   command = headCommand ;
   while(command = command -> next){
       if(command->status == running || command->status == suspended){
           if(waitpid(command->id,&status,WNOHANG|WUNTRACED) && ( WIFEXITED(status) || WIFSIGNALED(status))){
               command->status = finish;
           }
       }
   }
}

void terminateHandler(int signalNumber){
   terminateCommand(NULL);
}
