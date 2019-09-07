#include "string"
#include "utils/string_sprintf.h"
#include <cstdint>
#include <cstdio>
#include <windows.h>

std::basic_string<wchar_t> getString(HINSTANCE module, int uId) {
  wchar_t data[1000] = {};
  int length = LoadStringW(module, uId, data, sizeof(data) / 2);
  return data;

  // alternative
  //   size_t ptr = 0;
  //   int res = LoadStringW(module, uId, (wchar_t *)&ptr, 0);
  //   return (wchar_t *)ptr;
}

void printCommands() { printf("Command Line: %s\n", GetCommandLineA()); }

void printEnv() {
  char *env = GetEnvironmentStringsA();

  printf("ENV\n");
  while (*env) {
    int len = strlen(env);
    printf("  - %s\n", env);
    env += len + 1;
  }
}

std::string getDateInfo(SYSTEMTIME &time) {
  return string_sprintf("%04i-%02i-%02i %02i:%02i:%02i:%03i", time.wYear,
                        time.wMonth, time.wDay, time.wHour, time.wMinute,
                        time.wSecond, time.wMilliseconds);
}

void printTimeZone() {
  TIME_ZONE_INFORMATION info;
  memset(&info, 0, sizeof(info));
  DWORD ret = GetTimeZoneInformation(&info);
  printf("TimeZone:\n");
  printf("  - Daylight=%s(%i)\n",
         ret == TIME_ZONE_ID_STANDARD
             ? "STANDARD"
             : ret == TIME_ZONE_ID_DAYLIGHT ? "STANDARD" : "UNKNOWN",
         ret);
  printf("  - Bias=%i\n", info.Bias);
  printf("  - StandardName=%ls\n", info.StandardName);
  printf("  - StandardBias=%i\n", info.StandardBias);
  printf("  - Switch Daylight->Standard=%s\n", getDateInfo(info.StandardDate).c_str());
  printf("  - DaylightName=%ls\n", info.DaylightName);
  printf("  - DaylightBias=%i\n", info.DaylightBias);
  printf("  - Switch Standard->Daylight=%s\n", getDateInfo(info.DaylightDate).c_str());
}

int main(int argc, char *argv[]) {
  printCommands();
  printEnv();
  printTimeZone();
  return 0;
}
