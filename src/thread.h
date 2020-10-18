#ifndef THREAD_H_7JRJHADP
#define THREAD_H_7JRJHADP

#include "except.h"

#define T thread_t
typedef struct T *T;

extern const except_t thread_failed;
extern const except_t thread_alerted;

extern int thread_init(int preempt, ...);
extern T thread_new(int apply(void *),
        void *args, int nbytes, ...);
extern void thread_exit(int code);
extern void thread_alert(T t);
extern T thread_self(void);
extern int thread_join(T t);
extern void thread_pause(void);

#undef T

#endif /* end of include guard: THREAD_H_7JRJHADP */
