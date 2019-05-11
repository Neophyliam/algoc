#ifndef MINUNIT_H_HXJE4TUL
#define MINUNIT_H_HXJE4TUL

#define mu_assert(test, message) do { \
    if (!(test)) return message;} while (0)
#define mu_run_test(test) do { \
    char *message = test(); \
    tests_run++; \
    if (message) return message; \
    } while(0)
extern int tests_run;

#endif /* end of include guard: MINUNIT_H_HXJE4TUL */
