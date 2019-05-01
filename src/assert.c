#include "assert.h"

const except_t AssertException = {"AssertException"};

void (assert)(int e)
{
    assert(e);
}
