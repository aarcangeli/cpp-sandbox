#include <algorithm>
#include "stdio.h"

class Integer {
    const int number;

public:
    Integer() : number(0) { printf("z  [%08Xh] Integer::Integer()\n", (size_t) this); }
    Integer(int number) : number(number) { printf("  [%08zXh] Integer::Integer(int)\n", (size_t) this); }

    Integer(Integer &copy) : number(copy.number) { printf("  [%08zXh] Integer::Integer(Integer&)\n", (size_t) this); }
    Integer(Integer &&move) : number(move.number) { printf("  [%08zXh] Integer::Integer(Integer&&)\n", (size_t) this); }

    ~Integer() noexcept { printf("  [%08zXh] Integer::~Integer()\n", (size_t) this); }

    Integer operator+(const Integer &b) {
        printf("  [%08zXh] Integer Integer::operator+(const Integer&)\n", (size_t) this);
        return number + b.number;
    }
};

int main() {
    printf("Integer a = 5;\n");
    Integer a = 5;
    printf("\n");

    printf("Integer b = a + 2;\n");
    Integer b = a + 2;
    printf("\n");

    printf("destructing;\n");
}
