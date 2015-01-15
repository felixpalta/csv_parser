#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { MAX_FIELDS = 20 };

static char linebuf[200];
static char *fields[MAX_FIELDS];

/* returns number of fields parsed in line */
int csv_getline(FILE *fin)
{
    int nfields = 0;
    char *p = NULL;

    if (fgets(linebuf, sizeof(linebuf), fin) == NULL)
        return -1;

    p = strtok(linebuf, ",\n\r");
    while (p != NULL && nfields < MAX_FIELDS)
    {
        fields[nfields++] = p;
        p = strtok(NULL, ",\n\r");
    }

    return nfields;
}

void error(const char *msg, int code)
{
    fprintf(stderr, "%s", msg);
    exit(code);
}

int main(int argc, char *argv[])
{
    const char *filename = NULL;
    FILE *f = NULL;
    int i = 0, nf = 0;

    if (argc < 2)
        error("Input file required\n", 1);

    filename = argv[1];

    if ((f = fopen(filename, "r")) == NULL)
        error("Cannot open file for reading\n", 2);

    while ((nf = csv_getline(f)) != -1)
        for (i = 0; i < nf; ++i)
            printf("field[%d] = %s\n", i, fields[i]);

    return 0;
}

