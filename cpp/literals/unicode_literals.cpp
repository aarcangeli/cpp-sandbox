#include "iostream"
#include "cassert"
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <iomanip>

using namespace std;

bool isLittleEndian() {
    uint32_t integer = 1;
    auto *asByteArray = (uint8_t *) &integer;
    if (asByteArray[0] == 1) {
        // Little endian
        return true;
    } else if (asByteArray[3] == 1) {
        // Big Endian
        return false;
    } else {
        // Unexpected behaviour
        // it should never happen
        uint16_t i = 0;
        while (++i) printf("Panic error!  ");
        assert(false);
        return -1;
    }
}

int main() {
    // NB: a unicode code point is in the range 000000-10FFFF

    const char *utf8 = "ab"; // 2 standard characters
    assert(utf8[0] == (char) 0x61);
    assert(utf8[1] == (char) 0x62);
    assert(utf8[2] == 0);

    const char16_t *utf16 = u"\u1234\u89ab"; // 2 characters in utf16 encoding
    assert(utf16[0] == (char16_t) 0x1234);
    assert(utf16[1] == (char16_t) 0x89ab);
    assert(utf16[2] == 0);

    if (isLittleEndian()) {
        const char *utf16_c = (char *) utf16;
        assert(utf16_c[0] == (char) 0x34);
        assert(utf16_c[1] == (char) 0x12);
        assert(utf16_c[2] == (char) 0xab);
        assert(utf16_c[3] == (char) 0x89);
        assert(utf16_c[4] == 0);
    } else {
        const char *utf16_c = (char *) utf16;
        assert(utf16_c[0] == (char) 0x12);
        assert(utf16_c[1] == (char) 0x34);
        assert(utf16_c[2] == (char) 0x89);
        assert(utf16_c[3] == (char) 0xab);
        assert(utf16_c[4] == 0);
    }

    // surrogate example: an utf-16 character can occupy 2 code points
    utf16 = u"\U00101234";
    assert(utf16[0] == (char16_t) 0xdbc4);
    assert(utf16[1] == (char16_t) 0xde34);
    assert(utf16[2] == 0);
    // unicode 0x00101234 is in fact splitted into surrogate pair 0xdbc4 and 0xde34
    // see http://www.russellcottrell.com/greek/utilities/surrogatepaircalculator.htm

    // an utf-16 character can occupy 2 characters
    const char32_t *utf32 = U"\U00101234";
    assert(utf32[0] == (char32_t) 0x00101234);
    assert(utf32[1] == 0);
}
