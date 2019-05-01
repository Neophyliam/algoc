#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include "except.h"

const except_t KeyboardInterrupt = {"KeyboardInterrupt"};

void sigint_handler(int signum)
{
    RAISE(KeyboardInterrupt);
}

int main(void)
{
    signal(SIGINT, sigint_handler);
    for (;;) {
        getchar();
    }
    return 0;
}
