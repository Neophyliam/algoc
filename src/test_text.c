#include <stdio.h>
#include <string.h>
#include "minunit.h"
#include "text.h"

int tests_run = 0;

char *test_put()
{
    text_t text;
    text = text_put("aaaaa");
    mu_assert(text.len == 5, "text.len != 5");
    mu_assert(memcmp(text.str, "aaaaa", 5) == 0,
            "text.str != \"aaaaa\"");
    return NULL;
}

char *test_get()
{
    char buf[48];
    char *str;
    text_t text = text_put("aaaaa");
    // provide a local buffer
    text_get(buf, 48, text);
    mu_assert(strlen(buf) == 5, "strlen(buf) != 5");
    mu_assert(strcmp(buf, "aaaaa") == 0,
            "strcmp(buf, \"aaaaa\") != 0");
    // no local buffer
    str = text_get(NULL, 0, text);
    mu_assert(strlen(str) == 5, "strlen(str) != 5");
    mu_assert(strcmp(str, "aaaaa") == 0,
            "strcmp(str, \"aaaaa\") != 0");
    return NULL;
}

char *test_box()
{
    text_t text;
    text = text_box("aaaaaaaaaa", 5);
    mu_assert(text.len == 5, "text.len != 5");
    mu_assert(memcmp(text.str, "aaaaa", 5) == 0,
            "text.str != \"aaaaa\"");
    return NULL;
}

char *test_sub()
{
    text_t text = text_put("aaaaa");
    text_t sub_text = text_sub(text, 1, 0);
    mu_assert(sub_text.len == 5, "sub_text.len != 5");
    mu_assert(sub_text.str == text.str, "sub_text.str != text.str");
    sub_text = text_sub(text, 1, 2);
    mu_assert(sub_text.len == 1, "sub_text.len != 1");
    mu_assert(sub_text.str == text.str, "sub_text.str != text.str");
    sub_text = text_sub(text, 2, 4);
    mu_assert(sub_text.len == 2, "sub_text.len != 2");
    mu_assert(sub_text.str == &text.str[1], "sub_text.str != &text.str[1]");
    return NULL;
}

char *test_pos()
{
    text_t text = text_put("bbbbb");
    mu_assert(text_pos(text, 1) == 1 &&
            text_pos(text, 2) == 2 &&
            text_pos(text, 5) == 5 &&
            text_pos(text, 6) == 6,
            "wrong positive position");
    mu_assert(text_pos(text, 0) == 6 &&
            text_pos(text, -1) == 5 &&
            text_pos(text, -2) == 4 &&
            text_pos(text, -5) == 1,
            "wrong negative position");
    return NULL;
}

char *test_cat()
{
    text_t s1, s2, s3;
    s1 = text_put("aaaaa");
    s2 = text_put("bbbbb");
    s3 = text_cat(s1, s2);
    mu_assert(s3.len == 10, "wrong s3.len");
    mu_assert(memcmp(s3.str, "aaaaabbbbb", 10) == 0,
            "wrong text_cat result");
    mu_assert(s3.str == s1.str,
            "not the third case");
    s3 = text_cat(s1, s3);
    mu_assert(s3.len == 15, "wrong s3.len");
    mu_assert(memcmp(s3.str, "aaaaaaaaaabbbbb", 15) == 0,
            "wrong text_cat result");
    mu_assert(s3.str != s1.str,
            "not the fifth case");
    s1 = s3;
    s3 = text_cat(s3, s2);
    mu_assert(s3.len == 20, "wrong s3.len");
    mu_assert(memcmp(s3.str, "aaaaaaaaaabbbbbbbbbb", 20) == 0,
            "wrong text_cat result");
    mu_assert(s3.str == s1.str,
            "not the fourth case");
    s3 = text_cat(text_null, s2);
    mu_assert(s3.str == s2.str && s3.len == s2.len, "not the first case");
    s3 = text_cat(s2, text_null);
    mu_assert(s3.str == s2.str && s3.len == s2.len, "not the second case");
    return NULL;
}

char *test_dup()
{
    text_t s1, s2;
    s1 = text_put("aaaaa");
    s2 = text_dup(s1, 0);
    mu_assert(s2.str == text_null.str && s2.len == text_null.len,
            "s2 is not text_null");
    s2 = text_dup(s1, 1);
    mu_assert(s2.str == s1.str && s2.len == s1.len,
            "s2 is not s1");
    s2 = text_dup(s1, 4);
    mu_assert(s2.len == 20 && s2.str == s1.str,
            "s2 does not start with s1, or s2 has a wrong len");
    return NULL;
}

char *test_reverse()
{
    text_t s1, s2;
    s1 = text_put("");
    s2 = text_reverse(s1);
    mu_assert(s2.str == text_null.str && s2.len == 0,
            "s2 is not text_null");
    s1 = text_put("a");
    s2 = text_reverse(s1);
    mu_assert(s2.str == s1.str && s2.len == s1.len,
            "s2 is not s1");
    s1 = text_put("ab");
    s2 = text_reverse(s1);
    mu_assert(s2.len == s1.len, "s2 has a wrong len");
    mu_assert(memcmp(s2.str, "ba", 2) == 0,
            "s2 is not \"ba\"");
    return NULL;
}

char *test_map()
{
    text_t s1, s2;
    s1 = text_put("aaaaa");
    s2 = text_map(s1, &text_lcase, &text_ucase);
    mu_assert(s2.len == s1.len, "s2 has a wrong len");
    mu_assert(memcmp(s2.str, "AAAAA", 5) == 0,
            "s2 != \"AAAAA\"");
    s1 = text_put("bbbbb");
    s2 = text_map(s1, NULL, NULL);
    mu_assert(s2.len == s1.len, "s2 has a wrong len");
    mu_assert(memcmp(s2.str, "BBBBB", 5) == 0,
            "s2 != \"BBBBB\"");
    return NULL;
}

char *test_cmp()
{
    text_t s1, s2;
    s1 = text_put("aaaaa");
    s2 = text_put("bbbbb");
    mu_assert(text_cmp(s1, s2) < 0, "s1 >= s2");
    s2 = text_put("aaaaaa");
    mu_assert(text_cmp(s1, s2) < 0, "s1 >= s2");
    s2 = text_put("a");
    mu_assert(text_cmp(s1, s2) > 0, "s1 <= s2");
    s2 = text_put("aaaaa");
    mu_assert(text_cmp(s1, s2) == 0, "s1 != s2");
    return NULL;
}

char *test_chr()
{
    text_t s = text_put("abcde");
    mu_assert(text_chr(s, 1, 0, 'a') == 1, "s.index('a') != 1");
    mu_assert(text_chr(s, 1, 0, 'e') == 5, "s.index('e') != 5");
    mu_assert(text_chr(s, 2, 0, 'e') == 5, "s.index('e') != 5");
    mu_assert(text_chr(s, 1, -1, 'b') == 2, "s.index('b') != 2");
    s = text_put("aabbc");
    mu_assert(text_rchr(s, 1, 0, 'b') == 4, "s.rindex('b') != 4");
    mu_assert(text_rchr(s, 1, -2, 'b') == 3, "s.rindex('b') != 3");
    return NULL;
}

char *test_upto()
{
    text_t s;
    s = text_put("abcde012345");
    mu_assert(text_upto(s, 1, 0, text_digits) == 6,
            "the first digit not in position 6");
    mu_assert(text_upto(s, 1, 0, text_lcase) == 1,
            "the first character not in position 1");
    mu_assert(text_upto(s, 2, 6, text_lcase) == 2,
            "the first character not in position 2");
    mu_assert(text_rupto(s, 1, 0, text_digits) == 11,
            "the last digit not in position 11");
    mu_assert(text_rupto(s, 1, 0, text_lcase) == 5,
            "the last character not in position 5");
    return NULL;
}

char *test_any()
{
    text_t s;
    s = text_put("abcde012345");
    mu_assert(text_any(s, 1, text_digits) == 0,
            "s[0] not in text_digits");
    mu_assert(text_any(s, 1, text_lcase) == 2,
            "s[0] in text_lcase");
    mu_assert(text_any(s, 6, text_digits) == 7,
            "s[5] in text_digits");
    mu_assert(text_any(s, 6, text_lcase) == 0,
            "s[5] not in text_lcase");
    return NULL;
}

char *test_many()
{
    text_t s;
    s = text_put("abcde012345");
    mu_assert(text_many(s, 1, 0, text_lcase) == 6,
            "the position to the right of the last lower case character "
            "is not 6");
    mu_assert(text_many(s, 1, 0, text_digits) == 0,
            "there are digits in the first of s");
    mu_assert(text_rmany(s, 1, 0, text_digits) == 6,
            "the position to the right of the first digit is not 7");
    mu_assert(text_rmany(s, 1, 0, text_lcase) == 0,
            "there are lower case characters at the end of s");
    return NULL;
}

char *test_find()
{
    text_t s = text_put("in the unix environment");
    text_t f = text_put("the");
    mu_assert(text_find(s, 1, 0, f) == 4,
            "the first occurrence of `the` is not in position 4");
    mu_assert(text_rfind(s, 1, 0, f) == 4,
            "the first occurrence of `the` is not in position 4");
    return NULL;
}

char *test_match()
{
    text_t s = text_put("in the unix environment");
    text_t f = text_put("in");
    mu_assert(text_match(s, 1, 0, f) == 3,
            "str s does not match str f");
    f = text_put("the");
    mu_assert(text_match(s, 1, 0, f) == 0,
            "str s matches str f");
    mu_assert(text_rmatch(s, 1, 0, f) == 0,
            "str s matches str f from right");
    f = text_put("environment");
    mu_assert(text_rmatch(s, 1, 0, f) == 13,
            "str s does not match str f from right");
    return NULL;
}

char *run_tests()
{
    text_save_t save = text_save();
    mu_run_test(test_put);
    mu_run_test(test_get);
    mu_run_test(test_box);
    mu_run_test(test_sub);
    mu_run_test(test_pos);
    mu_run_test(test_cat);
    mu_run_test(test_dup);
    mu_run_test(test_reverse);
    mu_run_test(test_map);
    mu_run_test(test_cmp);
    mu_run_test(test_chr);
    mu_run_test(test_upto);
    mu_run_test(test_any);
    mu_run_test(test_many);
    mu_run_test(test_find);
    mu_run_test(test_match);
    text_restore(&save);
    mu_assert(save == NULL, "save != NULL");
    return NULL;
}

int main(void)
{
    char *result = run_tests();
    if (result == NULL)
        printf("All tests passed\n");
    else {
        printf("Test failed: ");
        printf("%s\n", result);
    }
    printf("Tests run: %d\n", tests_run);
    return result != NULL;
}
