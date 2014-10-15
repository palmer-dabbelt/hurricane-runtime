#ifndef TEST_MAIN_HXX
#define TEST_MAIN_HXX

#include <stdio.h>
#include <unistd.h>

extern int test_main(void);

int main(int argc __attribute__((unused)),
         char **argv __attribute__((unused)))
{
    if (argc == 1) {
        printf("Starting Valgrind\n");
        execl("/usr/bin/valgrind", "/usr/bin/valgrind", argv[0], "--dont-run-valgrind", NULL);
        return 0;
    } else {
        auto out = test_main();
        return out;
    }
}

#endif
