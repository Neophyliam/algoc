#ifndef ARENA_H_2KPNTUDB
#define ARENA_H_2KPNTUDB

#include "except.h"

#define T arena_t
typedef struct T *T;

extern const except_t ArenaNewFailed;
extern const except_t ArenaFailed;

// exported functions
extern T arena_new(void);
extern void arena_dispose(T *ap);
extern void *arena_alloc(T arena, long nbytes, const char *file, int line);
extern void *arena_calloc(T arena, long count, long nbytes,
		const char *file, int line);
extern void arena_free(T arena);

#undef T

#endif /* end of include guard: ARENA_H_2KPNTUDB */
