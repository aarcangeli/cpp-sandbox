#include <cstdio>

float zero = 0.0f;

int main() {
  float inf = 1.0f / zero;

  // inf: inf
  printf("inf: %f\n", inf);

  // MINGW: (int)inf: -2147483648
  // MSVC: (int)inf: -2147483648
  // GCC: (int)inf: -2147483648
  printf("(int)inf: %i\n", (int)inf);

  // MINGW: (int)inf: 2147483647
  // MSVC: (int)inf: 0
  // GCC: (int)inf: 2147483647
  // clang linux: (int)inf: 0
  printf("(int)2e35f: %i\n", (int)2e35f);

  return 0;
}
