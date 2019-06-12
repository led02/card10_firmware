#ifndef _API_H
#define _API_H

#ifndef API
#  define API(id, def) def
#endif

#define API_FOO 0x35c3
API(API_FOO, void foo(short x, int y, char z, int w));

#define API_BAR 0xc0ffee
API(API_BAR, void bar(char*astr));

typedef struct {
    int foo;
    int bar;
    int baz;
} qux_t;

#define API_QUX 0xCCC
API(API_QUX, void qux(qux_t q));

#endif /* _API_H */
