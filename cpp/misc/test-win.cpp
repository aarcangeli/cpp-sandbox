#include <cstdio>
#include <windows.h>

int main(int argc, char *argv[]) {
  HMODULE module = LoadLibrary("kernel32.dll");
  if (!module) {
    printf("Module Not found");
    return 1;
  }

  FARPROC symbol = GetProcAddress(module, "GetCommandLineA");
  if (!symbol) {
    printf("Symbol not found");
    return 1;
  }

  printf("Running");
  typedef size_t *(*PROT)();
  PROT fn = (PROT)symbol;
  size_t *ret = fn();
  printf("ret: %i %s\n", argc, ret);

  return 0;
}
