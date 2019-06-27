#include <cstdint>
#include <stdio.h>

void unsignedShift() {
    uint32_t num1 = 0xc0000001;
    uint32_t num2 = num1 >> 16; // 0x0000c000
    uint32_t num3 = num1 / (uint32_t) 0x10000; // 0x0000c000

    printf("Using uint32_t:\n");
    printf("  - num1: 0x%08x\n", num1);
    printf("  - num2: 0x%08x\n", num2);
    printf("  - num3: 0x%08x\n", num3);
    printf("\n");
}

void signedShift() {
    int32_t num1 = 0xc0000001;
    int32_t num2 = num1 >> 16; // 0xffffc000
    int32_t num3 = num1 / (int32_t) 0x10000; // 0xffffc001

    printf("Using int32_t:\n");
    printf("  - num1: 0x%08x\n", num1);
    printf("  - num2: 0x%08x\n", num2);
    printf("  - num3: 0x%08x\n", num3);
    printf("\n");
}

int main() {
    unsignedShift();
    signedShift();
    return 0;
}
