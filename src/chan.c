#include <string.h>
#include "assert.h"
#include "mem.h"
#include "chan.h"
#include "sem.h"

#define T chan_t

struct T {
    const void *ptr;
    int *size;
    sem_t send, recv, sync;
};

T chan_new(void) {
    T c;
    new(c);
    sem_init(&c->send, 1);
    sem_init(&c->recv, 0);
    sem_init(&c->sync, 0);
    return c;
}

int chan_send(chan_t c, const void *ptr, int size) {
    assert(c);
    assert(ptr);
    assert(size >= 0);
    sem_wait(&c->send);
    c->ptr = ptr;
    c->size = &size;
    sem_signal(&c->recv);
    sem_wait(&c->sync);
    return size;
}

int chan_receive(chan_t c, void *ptr, int size) {
   int n;
   assert(c);
   assert(ptr);
   assert(size >= 0);
   sem_wait(&c->recv);
   n = *c->size;
   if (size < n)
       n = size;
   *c->size = n;
   if (n > 0)
       memcpy(ptr, c->ptr, n);
   sem_signal(&c->sync);
   sem_signal(&c->send);
   return n; 
}
