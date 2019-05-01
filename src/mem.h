#ifndef MEM_H_H7CNZ6UF
#define MEM_H_H7CNZ6UF

#include "except.h"

// exported exceptions
extern const except_t MemFailed;

// exported functions
extern void *mem_alloc(long nbytes, const char *file, int line);
extern void *mem_calloc(long count, long nbytes, const char *file, int line);
extern void mem_free(void *ptr, const char *file, int line);
extern void *mem_resize(void *ptr, long nbytes, const char *file, int line);

// exported macros
#define alloc(nbytes) \
    mem_alloc((nbytes), __FILE__, __LINE__)
#define calloc(count, nbytes) \
    mem_calloc((count), (nbytes), __FILE__, __LINE__)
#define new(p) ((p) = alloc((long) sizeof(*(p))))
#define newo(p) ((p) = calloc(1, (long) sizeof(*(p))))
#define dealloc(p) ((void)(mem_free((p), __FILE__, __LINE__), (p) = 0))
#define resize(p, n) \
    ((p) = mem_resize((p), (n), __FILE__, __LINE__))

#endif /* end of include guard: MEM_H_H7CNZ6UF */
