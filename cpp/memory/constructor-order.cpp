#include "stdio.h"

template<int value>
class A {
public:
    A() {
        printf("A<%i>::A()\n", value);
    }
};

class Tester : public A<0> {
    A<1> test1;
    A<2> test2;

public:
    // this order is ignored
    Tester() : test2{}, test1{}, A{} {}
};

int main() {
    Tester tester;
}
