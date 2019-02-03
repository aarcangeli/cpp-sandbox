#include "iostream"
#include "cassert"
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <iomanip>

using namespace std;

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
