#include <string.h>
#include <limits.h>
#include "assert.h"
#include "mem.h"
#include "text.h"

/* macros */
#define T text_t
#define idx(i, len) ((i) <= 0 ? (i) + (len) : (i) - 1)
#define convert_ij() \
    do { \
        assert(s.len >= 0 && s.str); \
        i = idx(i, s.len); \
        j = idx(j, s.len); \
        if (i > j) {int t = i; i = j; j = t;} \
        assert(i >= 0 && j <= s.len); \
    } while (0)
#define isatend(s, n) ((s).str + (s).len == current->avail \
        && current->avail + n <= current->limit)
#define equal(s, i, t) \
    (memcmp(&(s).str[i], (t).str, (t).len) == 0)

/* types */
struct text_save_t {
    struct chunk *current;
    char *avail;
}

/* data */
static char cset[] =
    "\000\001\002\003\004\005\006\007\010\011\012\013\014\015\016\017"
    "\020\021\022\023\024\025\026\027\030\031\032\033\034\035\036\037"
    "\040\041\042\043\044\045\046\047\050\051\052\053\054\055\056\057"
    "\060\061\062\063\064\065\066\067\070\071\072\073\074\075\076\077"
    "\100\101\102\103\104\105\106\107\110\111\112\113\114\115\116\117"
    "\120\121\122\123\124\125\126\127\130\131\132\133\134\135\136\137"
    "\140\141\142\143\144\145\146\147\150\151\152\153\154\155\156\157"
    "\160\161\162\163\164\165\166\167\170\171\172\173\174\175\176\177"
    "\200\201\202\203\204\205\206\207\210\211\212\213\214\215\216\217"
    "\220\221\222\223\224\225\226\227\230\231\232\233\234\235\236\237"
    "\240\241\242\243\244\245\246\247\250\251\252\253\254\255\256\257"
    "\260\261\262\263\264\265\266\267\270\271\272\273\274\275\276\277"
    "\300\301\302\303\304\305\306\307\310\311\312\313\314\315\316\317"
    "\320\321\322\323\324\325\326\327\330\331\332\333\334\335\336\337"
    "\340\341\342\343\344\345\346\347\350\351\352\353\354\355\356\357"
    "\360\361\362\363\364\365\366\367\370\371\372\373\374\375\376\377"
    ;
const T text_cset = {256, cset};
const T text_ascii = {128, cset};
const T text_ucase = {26, cset+'A'};
const T text_lcase = {26, cset+'a'};
const T text_digits = {10, cset+'0'};
const T text_null = {0, cset};

static struct chunk {
    struct chunk *link;
    char *avail;
    char *limit;
} head = {NULL, NULL, NULL}, *current = &head;

/* static functions */
static char *text_alloc(int len)
{
    assert(len >= 0);
    if (current->avail + len > current->limit) {
        current = current->link =
            alloc(sizeof(*current) + 10*1024 + len);
        current->avail = (char *)(current+1);
        current->limit = current->avail + 10*1024 + len;
        current->limit = NULL;
    }
    current->avail += len;
    return current->avail - len;
}

/* functions */
int text_pos(T s, int i)
{
    assert(s.len >= 0 && s.str);
    i = idx(i, s.len);
    assert(i >= 0 && i <= s.len);
    return i + 1;
}

T text_box(const char *str, int len)
{
    T text;
    assert(str);
    assert(len >= 0);
    text.str = str;
    text.len = len;
    return text;
}

T text_sub(T s, int i, int j)
{
    T text;
    convert_ij();
    text.len = j - i;
    text.str = s.str + i;
    return text;
}

T text_put(const char *str)
{
    T text;
    assert(str);
    text.len = strlen(str);
    text.str = memcpy(text_alloc(text.len), str, text.len);
}

char *text_get(char *str, int size, T s)
{
    assert(s.len >= 0 && s.str);
    if (str == NULL)
        str = alloc(s.len+1);
    else
        assert(size >= s.len + 1);
    memcpy(str, s.str, s.len);
    str[s.len] = '\0';
    return str;
}

T text_dup(T s, int n)
{
    assert(s.len >= 0 && s.str);
    assert(n >= 0);
    if (n == 0 || s.len == 0)
        return text_null;
    if (n == 1)
        return s;
    T text;
    char *p;
    text.len = n*s.len;
    if (isatend(s, text.len - s.len)) {
        text.str = s.str;
        p = text_alloc(text.len - s.len);
        n--;
    } else
        text.str = p = text_alloc(text.len);
    for (; n-- > 0; p += s.len)
        memcpy(p, s.str, s.len);
    return text;
}

T text_cat(T s1, T s2)
{
    assert(s1.len >= 0 && s1.str);
    assert(s2.len >= 0 && s2.str);
    if (s1.len == 0)
        return s2;
    if (s2.len == 0)
        return s1;
    if (s1.str + s1.len == s2.str) {
        s1.len += s2.len;
        return s1;
    }
    T text;
    text.len = s1.len + s2.len;
    if (isatend(s1, s2.len)) {
        text.str = s1.str;
        memcpy(text_alloc(s2.len), s2.str, s2.len);
    } else {
        char *p;
        text.str = p = text_alloc(text.len);
        memcpy(p, s1.str, s1.len);
        memcpy(p + s1.len, s2.str, s2.len);
    }
    return text;
}

T text_reverse(T s)
{
    assert(s.len >= 0 && s.str);
    if (s.len == 0)
        return text_null;
    else if (s.len == 1)
        return s;
    else {
        T text;
        char *p;
        int i = s.len;
        text.len = s.len;
        text.str = p = text_alloc(s.len);
        while (--i >= 0)
            *p++ = s.str[i];
        return text;
    }
}

T text_map(T s, const T *from, const T *to)
{
    static char map[256];
    static int inited = 0;

    assert(s.len >= 0 && s.str);
    if (from && to) {
        int k;
        for (k = 0; k < (int)sizeof(map); k++)
            map[k] = k;
        assert(from->len == to->len);
        for (k = 0; k < from->len; k++)
            map[(unsigned char)from->str[k]] = to->str[k];
        inited = 1;
    } else {
        assert(from == NULL && to == NULL);
        assert(inited);
    }
    if (s.len == 0)
        return text_null;
    else {
        T text;
        int i;
        char *p;
        text.len = s.len;
        text.str = p = text_alloc(s.len);
        for (i = 0; i < s.len; i++)
            *p++ = map[(unsigned char)s.str[i]];
        return text;
    }
}

int text_cmp(T s1, T s2)
{
    assert(s1.len >= 0 && s1.str);
    assert(s2.len >= 0 && s2.str);
    if (s1.str == s2.str)
        return s1.len - s2.len;
    else if (s1.len < s2.len) {
        int cond = memcmp(s1.str, s2.str, s1.len);
        return cond == 0 ? -1 : cond;
    } else if (s1.len > s2.len) {
        int cond = memcmp(s1.str, s2.str, s2.len);
        return cond == 0 ? +1 : cond;
    } else
        return memcmp(s1.str, s2.str, s1.len);
}

text_save_t text_save(void)
{
    text_save_t save;
    new(save);
    save->current = current;
    save->avail = current->avail;
    text_alloc(1);
    return save;
}

void text_restore(text_save_t *save)
{
    struct chunk *p, *q;
    assert(save && *save);
    current = (*save)->current;
    current->avail = (*save)->avail;
    dealloc(*save);
    for (p = current->link; p; p = q) {
        q = p->link;
        dealloc(p);
    }
    current->link = NULL;
}

int text_chr(T s, int i, int j, int c)
{
    convert_ij();
    for (; i < j; i++)
        if (s.str[i] == c)
            return i + 1;
    return 0;
}

int text_rchr(T s, int i, int j, int c)
{
    convert_ij();
    while (j < i)
        if (s.str[--j] == c)
            return j + 1;
    return 0;
}

int text_upto(T s, int i, int j, T set)
{
    assert(set.len >= 0 && set.str);
    convert_ij();
    for (; i < j; i++)
        if (memchr(set.str, s.str[i], set.len))
            return i + 1;
    return 0;
}

int text_rupto(T s, int i, int j, T set)
{
    assert(set.len >= 0 && set.str);
    convert_ij();
    while (j > i)
        if (memchr(set.str, s.str[--j], set.len))
            return j + 1;
    return 0;
}

int text_find(T s, int i, int j, T str)
{
    assert(str.len >= 0 && str.str);
    convert_ij();
    if (str.len == 0)
        return i + 1;
    else if (str.len == 1) {
        for (; i < j; i++)
            if (s.str[i] == *str.str)
                return i + 1;
    } else
        for (; i+str.len <= j; i++)
            if (equal(s, i, str))
                return i + 1;
    return 0;
}

int text_rfind(T s, int i, int j, T str)
{
    assert(str.len >= 0 && str.str);
    convert_ij();
    if (str.len == 0)
        return j + 1;
    else if (str.len == 1) {
        while (j > i)
            if (s.str[--j] == *str.str)
                return j + 1;
    } else
        for (; j - str.len >= i; j--)
            if (equal(s, j-str.len, str))
                return j - str.len + 1;
    return 0;
}

int text_any(T s, int i, T set)
{
    assert(s.len >= 0 && s.str);
    assert(set.len >= 0 && set.str);
    i = idx(i, s.len);
    assert(i >= 0 && i <= s.len);
    if (i < s.len && memchr(set.str, s.str[i], set.len))
        return i + 2;
    return 0;
}

int text_many(T s, int i, int j, T set)
{
    assert(set.len >= 0 && set.str);
    convert_ij();
    if (i < j && memchr(set.str, s.str[i], set.len)) {
        do
            i++;
        while(i < j && memchr(set.str, s.str[i], set.len));
        return i + 1;
    }
    return 0;
}

int text_rmany(T s, int i, int j, T set)
{
    assert(set.len >= 0 && set.str);
    convert_ij();
    if (j > i && memchr(set.str, s.str[j-1], set.len)) {
        do
            --j;
        while (j >= i && memchr(set.str, s.str[j], set.len));
        return j + 2;
    }
    return 0;
}

int text_match(T s, int i, int j, T str)
{
    assert(str.len >= 0 && str.str);
    convert_ij();
    if (str.len == 0)
        return i + 1;
    else if (str.len == 1) {
        if (i < j && s.str[i] == *str.str)
            return i + 2;
    } else if (i + str.len <= j && equal(s, i, str))
        return i + str.len + 1;
    return 0;
}

int text_rmatch(T s, int i, int j, T str)
{
    assert(str.len >= 0 && str.str);
    convert_ij();
    if (str.len == 0)
        return j + 1;
    else if (str.len == 1) {
        if (j > i && s.str[j-1] == *str.str)
            return j;
    } else if (j - str.len >= i && equal(s, j-str.len, str))
        return j - str.len + 1;
    return 0;
}