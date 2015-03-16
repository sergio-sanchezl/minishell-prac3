#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minishell_input.h"

/**
 * This function prints a simple string that will be used as the shell prompt.
 */
void print_prompt()
{
   printf("$ ");
   fflush(stdout);
}

/**
 * This function removes the '\n' at the end of the string, changing it for a
 * '\0' character. The CR+LF character is introduced by the function fgets().
 */
static void remove_crlf(char *str)
{
    int i, length;

    length = strlen(str);

    for(i = length - 1; i >= 0; i--)
    {
        if (str[i] == '\n')         
        {
            str[i] = 0;
            break;
        }
    }
}

/**
 * This function reads a character string from the standard input using the
 * function fgets(). When done, it removes the CR+LF at the end of the line
 * using the function remove_crlf().
 */
void read_command_line(char *buf)
{
    memset(buf, 0, sizeof(buf));

    fgets(buf, BUFSIZ-1, stdin);

    buf[BUFSIZ-1]=0;

    remove_crlf(buf);  

}
