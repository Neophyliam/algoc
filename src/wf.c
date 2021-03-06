#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "table.h"
#include "getword.h"
#include "atom.h"
#include "mem.h"

int first(int c)
{
    return isalpha(c);
}

int rest(int c)
{
    return isalpha(c) || c == '_';
}

int compare(const void *x, const void *y)
{
    return strcmp(*(char **)x, *(char **)y);
}

void vfree(const void *key, void **count, void *cl)
{
    dealloc(*count);
}

void wf(char *name, FILE *fp)
{
    table_t table = table_new(0, NULL, NULL);
    char buf[128];

    while (getword(fp, buf, sizeof(buf), first, rest)) {
        const char *word;
        int i, *count;
        for (i = 0; buf[i] != '\0'; i++)
            buf[i] = tolower(buf[i]);
        word = atom_string(buf);
        count = table_get(table, word);
        if (count)
            (*count)++;
        else {
            new(count);
            *count = 1;
            table_put(table, word, count);
        }
    }
    if (name)
        printf("%s:\n", name);
    int i;
    void **array = table2array(table, NULL);
    qsort(array, table_length(table), 2*sizeof(*array), compare);
    for (i = 0; array[i]; i += 2)
        printf("%d\t%s\n", *(int *)array[i+1], (char *)array[i]);
    dealloc(array);
    table_map(table, vfree, NULL);
    table_free(&table);
}

int main(int argc, char *argv[])
{
    int i;
    for (i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "%s: can't open '%s' (%s)\n",
                    argv[0], argv[i], strerror(errno));
            return EXIT_FAILURE;
        } else {
            wf(argv[i], fp);
            fclose(fp);
        }
    }
    if (argc == 1) wf(NULL, stdin);
    return EXIT_SUCCESS;
}
