#include <stdarg.h>

#include "../src/fold.c"

int run_test(utf8s name, utf8s base, utf8s glyph, utf8s expected);


int main () {
    int tests_nom = run_test(
        "old_name",
        "previouslyknownas",
        "starglyph",
        "previoustlyknownarglyph"
    );
    int tests_randabc = run_test(
        "random_alphabet",
        "abcdefghijklmnopqrstuvwxyz", 
        "mscbnjgqtvwldxruikaphzfeoy",
        "abcdefghijklmnopqrscbnjgqtuvwldxruikaphyzfeoy"
    );
    int tests_empty = run_test(
        "empty",
        "",
        "",
        NULL
    );
    return (2 * tests_nom + 3 * tests_randabc + 5 * tests_empty);
}


int run_test(utf8s name, utf8s base, utf8s glyph, utf8s expected) {
    utf8s actual = combine(base, glyph);
    if (expected && actual)  {
        if (strcmp(expected, actual)) {
            printf("Failed test %s with output %s\n", name, actual);
/*            int i=0;for(; *(expected + i) == *(actual + i) && i<24; ++i);
            printf("index: %d\n", i);*/
            return 1;
        }
        printf("Passed test %s\n", name);
        return 0;
    } else if (expected || actual) {
        if (expected)
            printf("Failed test %s with no output\n", name);
        else
            printf("Failed test %s with unexpected output: %s\n", actual);
        return -1;
    } else
        printf("Passed test %s with no output\n", name);
    return 0;
}
