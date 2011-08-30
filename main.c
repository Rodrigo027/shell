/* 
    Nome: Rodrigo Fernandes da Costa
    n. USP: 6793073
    
    Nome: Lucas de Moraes Franco
    n. USP: 6793239
*/

#include "main.h"

int main(void)
{
    char command_matrix[COMMAND_MATRIX_HEIGHT][COMMAND_MATRIX_WIDTH] = {0};
    char c;
    int i,j;
    int temp;

    do
    {
        /* clean commmad matrix */
        j = i = 0;
        memset(command_matrix,'\0',COMMAND_MATRIX_HEIGHT*COMMAND_MATRIX_WIDTH);
        
        printf("shell> ");
        
        /* get user input */
        while((c = getchar()) != '\n' && i < COMMAND_MATRIX_HEIGHT - 1)
        {
            switch (c)
            {
                case '\t':
                case ' ':
                    if(j != 0)
                    {
                        ++i;
                        j = 0;
                    }
                    break;
                default:
                    if(j < COMMAND_MATRIX_WIDTH - 1)
                    {
                        command_matrix[i][j++] = c;
                    }
            }
        }
        
        /* clean redirection commands */
        for(i = 0 ; i < COMMAND_MATRIX_HEIGHT ; ++i)
        {
            if
            (
                command_matrix[i][0] == '|'
                && command_matrix[i][1] == '\0'
            )
            {
                command_matrix[i][0] = '\0';
            }
            if
            (
                (
                    command_matrix[i][0] == '<'
                    && command_matrix[i][1] == '\0'
                ) || (
                    command_matrix[i][0] == '>'
                    && command_matrix[i][1] == '\0'
                ) || (
                    command_matrix[i][0] == '<'
                    && command_matrix[i][1] == '<'
                    && command_matrix[i][2] == '\0'
                ) || (
                    command_matrix[i][0] == '>'
                    && command_matrix[i][1] == '>'
                    && command_matrix[i][2] == '\0'
                )
            )
            {
                command_matrix[i][0] = '\0';
                command_matrix[i][1] = 'f';
            }
        }
        
        /* print user input without redirection commands */
        // for(printf("echo>") , i = 0 ; i < COMMAND_MATRIX_HEIGHT ; ++i)
        // {
            // if(command_matrix[i][0] != '\0')
            // {
                // printf(" %s",command_matrix[i]);
            // }
        // }
        // putchar('\n');
        // for(i = 0 ; i < COMMAND_MATRIX_HEIGHT ; ++i)
        // {
            // if (command_matrix[i][0] == '\0')
                // printf("NOTHING");
            // else
                // printf("%s",command_matrix[i]);
            // putchar('\n');
        // }
        
        /* look for command */
        
        i = 0;
        while(i < COMMAND_MATRIX_HEIGHT)
        {
            /* co command found */
            if(command_matrix[i][0] == '\0')
            {
                ++i;
                continue;
            }
        
            if(! strcmp("exit",command_matrix[i]) )
            {
                if(i+1 < COMMAND_MATRIX_HEIGHT && command_matrix[i+1][0] == '\0')
                {
                    printf("shell> quit\n");
                    return 0;
                }
                temp = 0;
                if
                (
                    i+1 < COMMAND_MATRIX_HEIGHT && 
                    (! sscanf(command_matrix[i+1],"%d",&temp))
                )
                {
                    printf("?> Command parameter not reconized.\n");
                    printf("?> type: exit return_value(optional)\n");
                }
                else
                {
                    printf("shell> quit\n");
                    return temp;
                }
            }
            
            else if(! strcmp("quit",command_matrix[0]))
            {
                if(i+1 < COMMAND_MATRIX_HEIGHT && command_matrix[i+1][0] == '\0')
                {
                    printf("shell> quit\n");
                    return 0;
                }
                temp = 0;
                if
                (
                    i+1 < COMMAND_MATRIX_HEIGHT && 
                    (! sscanf(command_matrix[i+1],"%d",&temp))
                )
                {
                    printf("?> Command parameter not reconized.\n");
                    printf("?> type: quit return_value(optional)\n");
                }
                else
                {
                    printf("shell> quit\n");
                    return temp;
                }
            }
            
            else
            {
                printf("?> command \"%s\" not found.\n",command_matrix[i]);
            }
            
            while(command_matrix[i][0] != '\0' || command_matrix[i][1] == 'f')
            {
                ++i;
            }
            ++i;
        }
    }while(1);
    
	return 0;
}
