#include <iostream>
#include <string>

template<typename T>
std::string printType() {
    return typeid(T).name();
}

template<typename = void>
std::string printParameters() {
    return "";
}

template<typename Head>
std::string printParameters(Head head) {
    return printType<Head>();
}

template<typename Head, typename Head2, typename ...Tail>
std::string printParameters(Head head, Head2 head2, Tail... tail) {
    return printType<Head>() + ", " + printParameters(head2, tail...);
}

template<typename ...Types>
std::string generic(Types... params) {
    return std::string("generic(") + printParameters(params...) + ")";
}

struct TestStruct {
};

class Test {
};

// Visual Studio:
// generic(int, float, double, long, char const *, class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >)
// generic(struct TestStruct, struct TestStruct *)
// generic(class Test, class Test *)
// generic(void (__cdecl*)(void))
// generic(int (__cdecl*)(int,char * *))
//
// MinGW:
// generic(i, f, d, l, PKc, NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE)
// generic(10TestStruct, P10TestStruct)
// generic(4Test, P4Test)
// generic(PFvvE)
// generic(PFiiPPcE)
// generic(PFNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEvE)

void testFn() {
}

int main(int argc, char **argv) {
    TestStruct st;
    Test t;
    std::cout << generic(3, 4.0f, 5.0, 124L, "literal", std::string()) << std::endl;
    std::cout << generic(st, new TestStruct()) << std::endl;
    std::cout << generic(t, new Test()) << std::endl;
    std::cout << generic(&testFn) << std::endl;
    std::cout << generic(&main) << std::endl;
    std::cout << generic(&printType<int>) << std::endl;
    return 0;
}
