#include "csv.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef enum
{
    FALSE,
    TRUE
} Bool;

// Get size in bytes of the array element type.
#define ELEM_SZ(arr) (sizeof(arr[0]))

enum { NOMEM = -2 };

static char *_line = NULL;   // Input line buffer.

static char *_sline = NULL; // Stripped copy of _line with delimiters replaced by null characters.

static int _line_sz = 0;  // Size of _line and _sline buffers in bytes.

static char **_fields = NULL;   // Array of pointers to fields in _sline, i.e. fields[i] is an i-th field in _sline.
static int _fields_n = 0;    // Number of fields in found by _split() in _sline.
static int _fields_len = 0;    // Length of _fields in memory, i.e. capacity of the array.
static char _field_sep[] = ",";

/**
 * Set static variables back to starting values.
 */
static void _reset(void);

/**
 * Split _line into fields.
 * @return Number of fields found in line.
 */
static int _split(void);

/**
* Check for and consume end of line character(s) such as \r, \r\n or \n
* */
static Bool _end_of_line(FILE *f, int c);

char *csv_getline(FILE *f)
{
    int i, c;

    assert(_line_sz >= 0);
    assert(_fields_len >= 0);

    if (_line == NULL)
    {
        _line_sz = 1;
        _fields_len = 1;
        _line = (char *) malloc((size_t) _line_sz);
        _sline = (char *) malloc((size_t) _line_sz);
        _fields = (char **) malloc((size_t) _fields_len * ELEM_SZ(_fields));
        if (_line == NULL || _sline == NULL || _fields == NULL)
        {
            _reset();
            return NULL;
        }
    }
    for (i = 0; (c = getc(f)) != EOF && !_end_of_line(f, c); ++i)
    {
        if (i >= _line_sz - 1)
        {
            char *new_line, *new_sline;

            assert(_line_sz > 0);

            _line_sz *= 2;
            new_line = (char *) realloc(_line, (size_t) _line_sz);
            new_sline = (char *) realloc(_sline, (size_t) _line_sz);
            if (new_line == NULL || new_sline == NULL)
            {
                _reset();
                return NULL;
            }
            _line = new_line;
            _sline = new_sline;
        }
        assert(c != EOF);
        _line[i] = (char) c;
    }
    _line[i] = '\0';
    if (_split() == NOMEM)
    {
        _reset();
        return NULL;
    }
    return (c == EOF && i == 0) ? NULL : _line;
}

static void _reset(void)
{
    free(_line);
    _line = NULL;

    free(_sline);
    _sline = NULL;

    free(_fields);
    _fields = NULL;

    _line_sz = 0;
    _fields_len = 0;
    _fields_n = 0;

}

static Bool _end_of_line(FILE *f, int c)
{
    if (c == '\n')
        return TRUE;
    if (c == '\r')  // check whether \r or \r\n found
    {
        c = getc(f);
        if (c != '\n' && c != EOF)
            ungetc(c, f);
        return TRUE;
    }
    return FALSE;
}

/**
 * Process quoted field.
 * @return Pointer to next separator.
 */
static char *_advquoted(char *p)
{
    size_t i, j;
    for (i = j = 0; p[j] != '\0'; ++i, ++j)
    {
        if (p[j] == '"' && p [++j] != '"')
        {
            size_t k = strcspn(p + j, _field_sep);
            memmove(p +i, p + j, k);
            i += k;
            j += k;
            break;
        }
        p[i] = p[j];
    }
    p[i] = '\0';
    return p + j;
}

static int _split(void)
{
    char *p;
    char *sep_p; // pointer to a temporary separator character
    char sep_c; // temporary separator character

    _fields_n = 0;
    if (_line[0] == '\0')
        return 0;
    strcpy(_sline, _line);
    p = _sline;

    do
    {
        if (_fields_n >= _fields_len)
        {
            char **new_fields;

            assert(_fields_len > 0);

            _fields_len *= 2;
            new_fields = (char **) realloc(_fields, (size_t) _fields_len * ELEM_SZ(_fields));
            if (new_fields == NULL)
                return NOMEM;
            _fields = new_fields;
        }
        if (*p == '"')
            sep_p = _advquoted(++p);  // skip initial quote
        else
            sep_p = p + strcspn(p, _field_sep);
        sep_c = sep_p[0];
        sep_p[0] = '\0'; // terminate field
        _fields[_fields_n++] = p;
        p = sep_p + 1;
    }
    while (sep_c == ',');

    return _fields_n;
}

char *csv_get_field(int n)
{
    if (n < 0 || n >= _fields_n)
        return NULL;
    return _fields[n];
}

int csv_n_fields()
{
    return _fields_n;
}
