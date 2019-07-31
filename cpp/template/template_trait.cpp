#include <iostream>

using namespace std;


class str_const { // constexpr string
private:
    const char *const p_;
    const std::size_t sz_;
public:
    template<std::size_t N>
    constexpr str_const(const char(&a)[N]) : p_(a), sz_(N - 1) {}
    constexpr char operator[](std::size_t n) { // []
        return n < sz_ ? p_[n] : throw std::out_of_range("");
    }
    constexpr std::size_t size() { return sz_; } // size()
};

template<typename T = unsigned int>
constexpr T binary_const(
        str_const b,
        std::size_t n = 0,
        T x = 0) {
    return
            n == b.size() ? x :
            b[n] == ',' ? binary_const<T>(b, n + 1, x) :
            b[n] == ' ' ? binary_const<T>(b, n + 1, x) :
            b[n] == '0' ? binary_const<T>(b, n + 1, (x * 2) + 0) :
            b[n] == '1' ? binary_const<T>(b, n + 1, (x * 2) + 1) :
            throw std::domain_error(
                    "Only '0', '1', ',', and ' ' may be used");
}

constexpr int t = binary_const("10");

template<typename Ta>
auto mult(const Ta &a, const Ta &b) -> decltype(a * b) {
    return a * b;
}

int main() {
    auto i = mult(2LL, 2LL);
    decltype((int64_t) 2) ciao;
    printf("ret: %lli %s", i, typeid(ciao).name());
}
