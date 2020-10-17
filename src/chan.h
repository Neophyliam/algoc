#ifndef CHAN_H_TQHSPIMC
#define CHAN_H_TQHSPIMC

#define T chan_t
typedef struct T *T;

extern T chan_new(void);
extern int chan_send(T c, const void *ptr, int size);
extern int chan_receive(T c, void *ptr, int size);

#undef T

#endif /* end of include guard: CHAN_H_TQHSPIMC */
