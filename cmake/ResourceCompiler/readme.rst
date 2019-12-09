ResourceCompiler
------------------

Convert a file into a c++ readable binary data.

Syntax:
    compile_resources(<output_list> <namespace> <files_glob>)

Arguments:
    <output_list>   A list on which the generated file will be appended

    <namespace>     Used as namespace in generated files

    <files_glob>    A glob-expanded list of files

This command generate a file named res_<namespace>.h in build directory with the following format:

.. code-block:: objectivec

    namespace myset {
        // cmake/ResourceCompiler/resources/star.png
        extern const char* star;
        extern const char* star_end;
    }

To use a resource from c++ just include it

.. code-block:: objectivec

    #include "res_myset.h"

    int main() {
        const char *myFile = myset::star;
        size_t size = myset::star_end - myset::star;
    }

Actual star.png.cpp is generated at build time with the following format:

.. code-block:: objectivec

    namespace myset {
        const char star_array[] = {
            '\x89', '\x50', '\x4e', '\x47', '\x0d', '\x0a', '\x1a', '\x0a', '\x00', '\x00',
            '\x00', '\x0d', '\x49', '\x48', '\x44', '\x52', '\x00', '\x00', '\x04', [....] };
        const char* star = star_array;
        const char* star_end = star_array + sizeof(star_array);
    }

Credits:
    Alessandro Arcangeli <alessandroarcangeli.rm@gmail.com>
