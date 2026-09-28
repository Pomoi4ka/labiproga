#include <stdio.h>
#include <limits.h>

void bits(long y)
{
    unsigned long x = y;
    const long nbits = sizeof y*8;
    for (int i = 0; i < nbits; ++i) {
        printf("%c", "01"[(x>>(nbits - i - 1))&1]);
    }
    printf("\n");
}

int main(void)
{
    const long konst = 0xfa55bea7;
    int i;
    printf("%p: %ld\n", (void *)&konst, konst);
    printf("LONG_MAX = %ld\n", LONG_MAX);
    printf("LONG_MIN = %ld\n", LONG_MIN);
    
    for (i = 0; i < sizeof konst; ++i)
        printf("%02x ", ((unsigned char *)&konst)[i]);
    printf("\n");
    bits(konst);
    
    return 0;
}