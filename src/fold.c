#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef uint32_t u_int32_t;

#include "utf8.c"

typedef unsigned char * utf8s;
typedef u_int32_t utf32c;
typedef struct {
    utf8s base;
    utf8s glyph;
    utf8s out_write;
} combinestep;

#ifndef TERMWILD /* Terminal wildcard: if glyph[0] is not in base,
                 /*                      combine === strcat
                   */
#define TERMWILD 0
#endif
#ifndef PARTGLYPH /* Retry with partial glyph: if glyph[0] is not in base,
                  /*                             retry with glyph[1], [2], ...
                    */
#define PARTGLYPH 0
#endif

combinestep combine_loop(utf8s base, utf8s glyph, utf8s out_write);
utf8s combine(utf8s base, utf8s glyph);
int run_test(utf8s name, utf8s base, utf8s glyph, utf8s expected);


utf8s combine(utf8s base, utf8s glyph) {
    utf8s output = malloc(strlen(base) + strlen(glyph) + 1);
    combinestep step = combine_loop(base, glyph, output);
    while (!step.out_write && PARTGLYPH && *glyph)
        step = combine_loop(base, ++glyph, output);
    if (!step.out_write)
        return NULL; // Invalid input
    while (*(step.glyph))
         step = combine_loop(step.base, step.glyph, step.out_write);
    *step.out_write = '\0';
    return output;
}


// This should probably be split again
combinestep combine_loop(utf8s base, utf8s glyph, utf8s out_write) {
    int base_readi = 0;
    int glyph_readi = 0;
    // Search base for glyph[0], appending searched chars to output
    utf32c glyph0 = u8_nextchar(glyph, &glyph_readi);
    utf32c basec;
    do {
        basec = u8_nextchar(base, &base_readi);
        if (!basec && TERMWILD) // see #define TERMWILD
            basec = glyph0;
        if (!basec) // by omitting else, it checks if glyph0 is a zero byte
            return (combinestep){base, glyph, NULL}; // Triggers fallback logic
        out_write += u8_wc_toutf8(out_write, basec);
    } while (glyph0 != basec);
    // Discard used chars from base
    base += base_readi;
    base_readi = 0; // no more references, delete if no use is found

    // Search glyph for first char also in base, appending only non-matches
    int matchloc = 0; // This doesn't get used, just for parameter
    for (utf32c glyphc = u8_nextchar(glyph, &glyph_readi);
         !u8_strchr(base, glyphc, &matchloc) && glyphc;
         glyphc = u8_nextchar(glyph, &glyph_readi))
        out_write += u8_wc_toutf8(out_write, glyphc);
    glyph += --glyph_readi;
    return (combinestep){base, glyph, out_write};
}



