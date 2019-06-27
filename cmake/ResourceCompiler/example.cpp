#include <cstdio>
#include "res_myset.h"

int main() {
    const char *myFile = myset::star;
    size_t size = myset::star_end - myset::star;
    printf("%zu bytes:\n", size);
    for (int i = 0; i < size; i++) {
        printf(" %02X", (unsigned char) myFile[i]);
        if (i % 16 == 15) {
            printf("\n");
        }
        if (i >= 1023) {
            printf("[...]\n");
            break;
        }
    }
}
