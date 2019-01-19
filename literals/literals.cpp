#include "iostream"
#include "cassert"

void rawString();
using namespace std;

int main() {
    //rawString();

    const char *normal = "è"; // based on file encoding
    const char *extendedCharacter = "\u00E5"; // implementation specific

    // a standard unicode character is in range 000000-10FFFF

    const char *utf8 = u8"\u00E5"; // implementation specific
    assert(utf8[0] == (char) 0xc3);
    assert(utf8[1] == (char) 0xa5);
    assert(utf8[2] == 0);

    const char16_t *utf16 = u"\u1234\u89ab"; // utf16 encoding
    assert(utf16[0] == (char16_t) 0x1234);
    assert(utf16[1] == (char16_t) 0x89ab);
    assert(utf16[2] == 0);

//    // this is true in little endian
//    const char *utf16_c = (char *) utf16;
//    assert(utf16_c[0] == (char) 0x34);
//    assert(utf16_c[1] == (char) 0x12);
//    assert(utf16_c[2] == (char) 0xab);
//    assert(utf16_c[3] == (char) 0x89);
//    assert(utf16_c[4] == 0);

    // an utf-16 character can occupy 2 characters
    utf16 = u"\U00101234";
    assert(utf16[0] == (char16_t) 0xdbc4);
    assert(utf16[1] == (char16_t) 0xde34);
    assert(utf16[2] == 0);

    // an utf-16 character can occupy 2 characters
    const char32_t *utf32 = U"\U00101234";
    assert(utf32[0] == (char32_t) 0x00101234);
    assert(utf32[1] == 0);
}

void rawString() {
    cout << "standard literal" << endl;
    // standard string literal

    cout << R"(raw literal example)" << endl;
    // raw literal

    cout << R"(row literal can contain any character (for example "\n\a\r\0"))" << endl;
    // row literal can contain any character (for example "\n\a\r\0")

    // row string can contain newline
    cout << R"(line 1
line 2
line 3)" << endl;
    // line 1
// line 2
// line 3

    // in the rare case that you have to write a )" in a string you can use a different escape
    cout << R"***(you can write any )" and )"** )***" << endl;
    // you can write any )" and )"**

    // but also
    cout << R"/*+23__escape__(you probably never fall in an escape character)/*+23__escape__" << endl;
    // you probably never fall in an escape character
}
