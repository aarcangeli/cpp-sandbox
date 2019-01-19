#include "iostream"
#include "cassert"
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <iomanip>

void rawString();
void wideStrings();
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
    wideStrings();
    rawString();

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

    // an utf-16 character can occupy 2 spaces
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

// utility function for output
void hex_print(const string &s) {
    cout << hex << setfill('0');
    for (unsigned char c : s)
        cout << setw(2) << static_cast<int>(c) << ' ';
    cout << dec << '\n';
}

void printWChars(const wstring &wstr) {// write bytes
    cout << hex << setfill('0');
    for (int i = 0; i < wstr.size(); ++i) {
        cout << "wstr[" + to_string(i) + "] = 0x" << setw(4) << wstr[i] << endl;
    }
}

void wideStrings() {
    // wide character data
    wstring wstr = L"z\u00df\u6c34";
    printWChars(wstr);

    // wide to UTF-8
    wstring_convert<codecvt_utf8<wchar_t>> conv1;
    string u8str = conv1.to_bytes(wstr);
    cout << "UTF-8: ";
    hex_print(u8str);
    cout << endl;

    // however it doesn't work with surrogate pairs
    try {
        wstr = L"\U00101234";
        printWChars(wstr);
        cout << "UTF-8 from string with surrogate: ";
        u8str = conv1.to_bytes(wstr);
        hex_print(u8str);
    } catch (std::range_error &e) {
        cout << "Cannot convert a surrogate pair: " << e.what() << endl;
    }
    cout << endl;
}
