#ifndef SEM_H_8HHGJOWN
#define SEM_H_8HHGJOWN

#define T sem_t
typedef struct T {
    int count;
    void *queue;
} T;

#define LOCK(mutex) do {sem_t *_yymutex = &(mutex); \
    sem_wait(__yymutex);
#define END_LOCK sem_signal(_yymutex); } while (0)

extern void sem_init(T *s, int count);
extern T *sem_new(int count);
extern void sem_wait(T *s);
extern void sem_signal(T *s);

#undef T

#endif /* end of include guard: SEM_H_8HHGJOWN */
