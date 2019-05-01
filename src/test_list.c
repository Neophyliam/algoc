#include <stdio.h>
#include <string.h>
#include "atom.h"
#include "list.h"
#include "assert.h"

void test_list_list()
{
	list_t p1, p2;
	void *x;
	p1 = list_list(NULL);
	p2 = list_list("Atom", "Mem", "Arena", "List", NULL);
	assert(list_length(p1) == 0);
	assert(list_length(p2) == 4);
	p2 = list_pop(p2, &x);
	assert(strcmp(x, "Atom") == 0);
	p2 = list_pop(p2, &x);
	assert(strcmp(x, "Mem") == 0);
	p2 = list_pop(p2, &x);
	assert(strcmp(x, "Arena") == 0);
	p2 = list_pop(p2, &x);
	assert(strcmp(x, "List") == 0);
	assert(list_length(p2) == 0);
	list_free(&p1);
	list_free(&p2);
	p2 = list_list(list_list("Atom", NULL),
			(void *)list_list("Mem", NULL),
			(void *)list_list("Arena", NULL),
			(void *)list_list("List", NULL), NULL);
	assert(list_length(p2) == 4);
	p2 = list_pop(p2, &x);
	assert(list_length(x) == 1);
	list_free((list_t *)&x);
	p2 = list_pop(p2, &x);
	assert(list_length(x) == 1);
	list_free((list_t *)&x);
	p2 = list_pop(p2, &x);
	assert(list_length(x) == 1);
	list_free((list_t *)&x);
	p2 = list_pop(p2, &x);
	assert(list_length(x) == 1);
	list_free((list_t *)&x);
	list_free(&p2);
	printf("test succeed: %s()\n", __FUNCTION__);
}

void test_list_push()
{
	list_t p1, p2;
	list_t x1, x2;
	p1 = list_push(NULL, "List");
	p1 = list_push(p1, "Arena");
	p1 = list_push(p1, "Mem");
	p1 = list_push(p1, "Atom");
	p2 = list_list("Atom", "Mem", "Arena", "List", NULL);
	for (x1 = p1, x2 = p2; x1 && x2; x1 = x1->rest, x2 = x2->rest) {
		assert(strcmp(x1->first, x2->first) == 0);
	}
	list_free(&p1);
	list_free(&p2);
	printf("test succeed: %s()\n", __FUNCTION__);
}

void test_list_append()
{
	list_t p1, p2, p3;
	list_t x1, x2;
	p1 = list_list("Atom", "Mem", "Arena", "List", NULL);
	p2 = list_list("Atom", "Mem", NULL);
	p3 = list_list("Arena", "List", NULL);
	p2 = list_append(p2, p3);
	for (x1 = p1, x2 = p2; x1 && x2; x1 = x1->rest, x2 = x2->rest) {
		assert(strcmp(x1->first, x2->first) == 0);
	}
	list_free(&p1);
	list_free(&p2);
	printf("test succeed: %s()\n", __FUNCTION__);
}

void test_reverse_copy()
{
	list_t p1, p2, p3;
	list_t x1, x2;
	p1 = list_list("Atom", "Mem", "Arena", "List", NULL);
	p2 = list_list("List", "Arena", "Mem", "Atom", NULL);
	p2 = list_reverse(p2);
	for (x1 = p1, x2 = p2; x1 && x2; x1 = x1->rest, x2 = x2->rest) {
		assert(strcmp(x1->first, x2->first) == 0);
	}
	p2 = list_reverse(p2);
	p3 = list_reverse(list_copy(p2));
	for (x1 = p1, x2 = p3; x1 && x2; x1 = x1->rest, x2 = x2->rest) {
		assert(strcmp(x1->first, x2->first) == 0);
	}
	assert(p2 != p3);
	list_free(&p1);
	list_free(&p2);
	list_free(&p3);
	printf("test succeed: %s()\n", __FUNCTION__);
}

void test_ring()
{
	list_t p1, p2, p3;
	p1 = list_list("Atom", "Mem", "Arena", "List", NULL);
	p2 = list_list("Atom", "Mem", NULL);
	p3 = list_list("Arena", "List", NULL);
	// form a ring
	p2 = list_append(p2, p3);
	p3 = list_append(p3, p2);
	list_free(&p1);
	list_free(&p2);
	printf("test succeed: %s()\n", __FUNCTION__);
}

void mkatom(void **x, void *cl)
{
	char **str = (char **)x;
	FILE *fp = cl;
	*str = atom_string(*str);
	fprintf(fp, "%s\n", *str);
}

void test_list_map()
{
	list_t p1;
	p1 = list_list("Atom", "Mem", "Arena", "List", NULL);
	list_map(p1, mkatom, stderr);
	printf("test succeed: %s()\n", __FUNCTION__);
	list_free(&p1);
}

int main(int argc, char *argv[])
{
	test_list_list();
	test_list_push();
	test_list_append();
	test_reverse_copy();
	test_list_map();
	/* test_ring(); */
	return 0;
}
