#include <stdlib.h>

#include "free_args.h"

void parser_free_args(char **args)
{
    int i = 0;

    while(args[i])
        free(args[i++]);

    free(args);
}
