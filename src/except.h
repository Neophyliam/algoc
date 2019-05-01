#ifndef EXCEPT_H_94TG0MCY
#define EXCEPT_H_94TG0MCY

#include <setjmp.h>

#define T except_t
typedef struct T {
    const char* reason;
} T;

/* exported types */
typedef struct except_frame except_frame;
struct except_frame {
    except_frame *prev;
    jmp_buf env;
    const char *file;
    int line;
    const T *exception;
};

enum {except_entered=0, except_raised,
      except_handled,   except_finalized};

/* exported varaibles */
extern except_frame *except_stack;

extern const T AssertException;

/* exported functions */
void except_raise(const T *e, const char *file, int line);

/* exported macros */
#define RAISE(e) except_raise(&(e), __FILE__, __LINE__)
#define RERAISE except_raise(except_f.exception, except_f.file, \
        except_f.line)
#define RETURN switch (except_stack = except_stack->prev, 0) default: return 

#define TRY do { \
    volatile int except_flag; \
    except_frame except_f; \
    /* push */ \
    except_f.prev = except_stack; \
    except_stack = &except_f; \
    except_flag = setjmp(except_f.env); \
    if (except_flag == except_entered) {

#define EXCEPT(e) \
        if (except_flag == except_entered) except_stack = except_stack->prev; \
    } else if (except_f.exception == &(e)) { \
        except_flag = except_handled;

#define ELSE \
        if (except_flag == except_entered) except_stack = except_stack->prev; \
    } else { \
        except_flag = except_handled;

#define FINALLY \
        if (except_flag == except_entered) except_stack = except_stack->prev; \
    } { \
        if (except_flag == except_entered) \
            except_flag = except_finalized;

#define ENDTRY \
        if (except_flag == except_entered) except_stack = except_stack->prev; \
    } if (except_flag == except_raised) RERAISE; \
} while (0)

#undef T

#endif /* end of include guard: EXCEPT_H_94TG0MCY */
