#ifndef TABLE_H_V1AAMISV
#define TABLE_H_V1AAMISV

#define T table_t
typedef struct T *T;

/* exported functions */
extern T table_new(int hint,
		int cmp(const void *x, const void *y),
		unsigned hash(const void *key));
extern void table_free(T *table);
extern int table_length(T table);
// key should be immutable
extern void *table_put(T table, const void *key, void *value);
extern void *table_get(T table, const void *key);
extern void *table_remove(T table ,const void *key);
extern void table_map(T table,
		void apply(const void *key, void **valeu, void *cl),
		void *cl);
extern void **table2array(T table, void *end);

#undef T
#endif /* end of include guard: TABLE_H_V1AAMISV */
