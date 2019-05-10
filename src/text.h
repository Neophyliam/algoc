#ifndef TEXT_H_6PH3TEXJ
#define TEXT_H_6PH3TEXJ

#include <stdarg.h>

#define T text_t

typedef struct T {
    int len;
    const char *str;
} T;

extern const T text_cset;
extern const T text_ascii;
extern const T text_ucase;
extern const T text_lcase;
extern const T text_digits;
extern const T text_null;

extern T text_put(const char *str);
extern char *text_get(char *str, int size, T s);
extern T text_box(const char *str, int len);
extern T text_sub(T s, int i, int j);
extern int text_pos(T s, int i);
extern T text_cat(T s1, T s2);
extern T text_dup(T s, int n);
extern T text_reverse(T s);
extern T text_map(T s, const T *from, const T *to);
extern int text_cmp(T s1, T s2);
extern int text_chr(T s, int i, int j, int c);
extern int text_rchr(T s, int i, int j, int c);
extern int text_upto(T s, int i, int j, T set);
extern int text_rupto(T s, int i, int j, T set);
extern int text_any(T s, int i, T set);
extern int text_many(T s, int i, int j, T set);
extern int text_rmany(T s, int i, int j, T set);
extern int text_find(T s, int i, int j, T str);
extern int text_rfind(T s, int i, int j, T str);
extern int text_match(T s, int i, int j, T str);
extern int text_rmatch(T s, int i, int j, T str);

typedef struct text_save_t *text_save_t;

extern text_save_t text_save(void);
extern void text_restore(text_save_t *save);

#undef T

#endif /* end of include guard: TEXT_H_6PH3TEXJ */
