#include <stdlib.h>
#include <string.h>
#include "assert.h"
#include "except.h"
#include "arena.h"
#define T arena_t

const except_t ArenaNewFailed = {"Arena Creation Failed"};
const except_t ArenaFailed = {"Arena Allocation Failed"};

/* macros */
#define THRESHOLD 10

/* types */
struct T {
    T prev;
    char *avail;
    char *limit;
};

union align {
    int i;
    long l;
    long *lp;
    void *p;
    void (*fp)(void);
    float f;
    double d;
    long double ld;
};

union header {
    struct T b;
    union align a;
};

/* data */
static T freechunks;
static int nfree;

/* functions */
T arena_new(void)
{
    T arena = malloc(sizeof(*arena));
    if (arena == NULL)
        RAISE(ArenaNewFailed);
    arena->prev = NULL;
    arena->limit = arena->avail = NULL;
    return arena;
}

void arena_dispose(T *ap)
{
    assert(ap && *ap);
    arena_free(*ap);
    free(*ap);
    *ap = NULL;
}

void *arena_alloc(T arena, long nbytes, const char *file, int line)
{
    assert(arena);
    assert(nbytes > 0);
    /* round nbytes up to an alignment boundary */
    nbytes = ((nbytes + sizeof(union align) - 1) /
              (sizeof(union align))) * (sizeof(union align));
    while (nbytes > arena->limit - arena->avail) {
        /* get a new chunk */
        T new_arena;
        char *limit;
        /* new_arena<-a new chunk */
        if ((new_arena = freechunks) != NULL) {
            freechunks = freechunks->prev;
            nfree--;
            limit = new_arena->limit;
        } else {
            long m = sizeof(union header) + nbytes + 10*1024;
            new_arena = malloc(m);
            if (new_arena == NULL) {
                if (file == NULL)
                    RAISE(ArenaFailed);
                else
                    except_raise(&ArenaFailed, file, line);
            }
        }
        *new_arena = *arena;
        arena->avail = (char *)((union header*)new_arena + 1);
        arena->limit = limit;
        arena->prev = new_arena;
    }
    arena->avail += nbytes;
    return arena->avail - nbytes;
}

void *arena_calloc(T arena, long count, long nbytes,
        const char *file, int line)
{
    void *ptr;
    assert(count > 0);
    ptr = arena_alloc(arena, count*nbytes, file, line);
    memset(ptr, '\0', count*nbytes);
    return ptr;
}

void arena_free(T arena)
{
    assert(arena);
    while (arena->prev) {
        struct T tmp = *arena->prev;
        /* free the chunk described by arena */
        if (nfree < THRESHOLD) {
            arena->prev->prev = freechunks;
            freechunks = arena->prev;
            nfree++;
            // the limit member in freechunks describe the chunk itself,
            // not the chunk the prev pointer points to.
            freechunks->limit = arena->limit;
            // the avail member in freechunks does not matter, so leave it
            // alone.
        } else {
            free(arena->prev);
        }
        *arena = tmp;
    }
    assert(arena->limit == NULL);
    assert(arena->avail == NULL);
}
