#include <stdio.h>
#include "except.h"
#include "assert.h"

const except_t IOException = {"IOException"};

int main(void)
{
    int i;
    TRY
        assert(i==1);
    EXCEPT(IOException)
        fprintf(stderr, "This is an io exception!\n");
    EXCEPT(AssertException)
        fprintf(stderr, "This is an assert exception!\n");
        TRY
            assert(2==1);
        EXCEPT(AssertException)
            fprintf(stderr, "This is an another assert exception!\n");
        ENDTRY;
    ELSE
        fprintf(stderr, "Exception caught!\n");
    FINALLY
        printf("This should be run anyway!\n");
        RERAISE;
    ENDTRY;
    // test the block
    {
        printf("The block has been run!\n");
    }
    return 0;
}
