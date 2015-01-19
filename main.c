#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv.h"

void error(const char *msg, int code)
{
    fprintf(stderr, "%s", msg);
    exit(code);
}

int main(int argc, char *argv[])
{
    const char *filename = NULL;
    FILE *f = NULL;
    int i = 0;
    char *line;

    if (argc < 2)
        error("Input file required\n", 1);

    filename = argv[1];

    if ((f = fopen(filename, "r")) == NULL)
        error("Cannot open file for reading\n", 2);

    while ((line = csv_getline(f)) != NULL)
    {
        printf("Line: %s\n", line);
        for (i = 0; i < csv_n_fields(); ++i)
        {
            const char *s = csv_get_field(i);
            if (s == NULL)
                error("NULL as field string pointer\n", 3);
            printf("field[%d] = '%s'\n", i, s);
        }
    }

    return 0;
}

