#include "iostream"
#include <locale>
#include <codecvt>
#include <iomanip>

using namespace std;

void printWChars(const wstring &wstr) {// write bytes
    cout << hex << setfill('0');
    for (int i = 0; i < wstr.size(); ++i) {
        cout << "wstr[" + to_string(i) + "] = 0x" << setw(4) << wstr[i] << endl;
    }
}

// utility function for output
void hex_print(const string &s) {
    cout << hex << setfill('0');
    for (const unsigned char &c : s)
        cout << setw(2) << static_cast<int>(c) << ' ';
    cout << dec << '\n';
}

int main() {
    // wide character data
    wstring wstr = L"z\u00df\u6c34";
    printWChars(wstr);

    // wide to UTF-8
    wstring_convert<codecvt_utf8<wchar_t>> conv1;
    string u8str = conv1.to_bytes(wstr);
    cout << "UTF-8: ";
    hex_print(u8str);
    cout << endl;

    // However it doesn't work with surrogate pairs?????
    // Expected result: F0 9F 80 84
    // Msvc 2017: ed a0 bc ed b0 84 (FAIL)
    // MinGW: Cannot convert a surrogate pair: wstring_convert::to_bytes (FAIL)
    // clang: f0 9f 80 84 (OK)
    // gcc: f0 9f 80 84 (OK)
    try {
        // http://www.fileformat.info/info/unicode/char/1f004/index.htm
        wstr = L"\U0001F004";
        printWChars(wstr);
        cout << "UTF-8 from string with surrogate: ";
        u8str = conv1.to_bytes(wstr);
        hex_print(u8str);
    } catch (std::range_error &e) {
        cout << "Cannot convert a surrogate pair: " << e.what() << endl;
    }
    cout << endl;
}
