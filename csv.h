#ifndef __CSV_H__
#define __CSV_H__

#include <stdio.h>

/**
 * Read next input line.
 * @return Resulting string allocated on heap or NULL if run out of memory.
*/
char *csv_getline(FILE *f);

/**
 * @return n-th field in last string read by csv_getline().
 */
char *csv_get_field(int n);

/**
 * @return Number of fields in last string read by csv_getline().
 */
int csv_n_fields(void);

#endif
