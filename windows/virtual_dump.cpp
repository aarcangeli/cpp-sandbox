#include <cassert>
#include <cstdio>
#include <string>
#include <windows.h>

std::string padLeft(int size, std::string from) {
  while (from.length() < size) {
    from.append(" ");
  }
  return from;
}

std::string getState(DWORD state) {
  switch (state) {
  case MEM_COMMIT:
    return "MEM_COMMIT";
  case MEM_FREE:
    return "MEM_FREE";
  case MEM_RESERVE:
    return "MEM_RESERVE";
  default:
    return "Unknown";
  }
}

std::string getProtect(uint32_t state) {
  std::string result;
  if (state & PAGE_GUARD) {
    result += "PAGE_GUARD|";
    state &= ~PAGE_GUARD;
  }
  if (state & PAGE_NOCACHE) {
    result += "PAGE_NOCACHE|";
    state &= ~PAGE_NOCACHE;
  }
  if (state & PAGE_WRITECOMBINE) {
    result += "PAGE_WRITECOMBINE|";
    state &= ~PAGE_WRITECOMBINE;
  }
  switch (state) {
  case PAGE_NOACCESS:
    return result + "PAGE_NOACCESS";
  case PAGE_READONLY:
    return result + "PAGE_READONLY";
  case PAGE_READWRITE:
    return result + "PAGE_READWRITE";
  case PAGE_WRITECOPY:
    return result + "PAGE_WRITECOPY";
  case PAGE_EXECUTE:
    return result + "PAGE_EXECUTE";
  case PAGE_EXECUTE_READ:
    return result + "PAGE_EXECUTE_READ";
  case PAGE_EXECUTE_READWRITE:
    return result + "PAGE_EXECUTE_READWRITE";
  case PAGE_EXECUTE_WRITECOPY:
    return result + "PAGE_EXECUTE_WRITECOPY";
  case PAGE_GUARD:
    return result + "PAGE_GUARD";
  case PAGE_NOCACHE:
    return result + "PAGE_NOCACHE";
  case PAGE_WRITECOMBINE:
    return result + "PAGE_WRITECOMBINE";
  case MEM_COMMIT:
    return result + "MEM_COMMIT";
  case MEM_RESERVE:
    return result + "MEM_RESERVE";
  case MEM_DECOMMIT:
    return result + "MEM_DECOMMIT";
  case MEM_RELEASE:
    return result + "MEM_RELEASE";
  case MEM_FREE:
    return result + "MEM_FREE";
  case MEM_PRIVATE:
    return result + "MEM_PRIVATE";
  case MEM_MAPPED:
    return result + "MEM_MAPPED";
  case MEM_RESET:
    return result + "MEM_RESET";
  case MEM_TOP_DOWN:
    return result + "MEM_TOP_DOWN";
  case MEM_WRITE_WATCH:
    return result + "MEM_WRITE_WATCH";
  case MEM_PHYSICAL:
    return result + "MEM_PHYSICAL";
  case MEM_ROTATE:
    return result + "MEM_ROTATE";
  case MEM_LARGE_PAGES:
    return result + "MEM_LARGE_PAGES";
  case MEM_4MB_PAGES:
    return result + "MEM_4MB_PAGES";
  default:
    return result + "<unknown>";
  }
}

std::string getType(DWORD state) {
  switch (state) {
  case 0:
    return "0";
  case MEM_IMAGE:
    return "MEM_IMAGE";
  case MEM_MAPPED:
    return "MEM_MAPPED";
  case MEM_PRIVATE:
    return "MEM_PRIVATE";
  default:
    return "<unknown>";
  }
}

int main() {
  size_t ptr = 0;
  int sizeState = 10;
  int sizeProtect = 10;
  int sizeType = 10;

  // iterate to get column size
  do {
    MEMORY_BASIC_INFORMATION memInfo;
    if (!VirtualQuery((void *)(ptr), &memInfo, sizeof(memInfo))) {
      break;
    }
    sizeState = std::max(sizeState, (int)getState(memInfo.State).length());
    sizeProtect =
        std::max(sizeProtect, (int)getProtect(memInfo.Protect).length());
    sizeType = std::max(sizeType, (int)getType(memInfo.Type).length());
    ptr += memInfo.RegionSize;
  } while (ptr);

  // print header
  printf("%s ", padLeft(18, "start").c_str());
  printf("%s  ", padLeft(18, "end").c_str());
  printf("%s  ", padLeft(sizeState, "State").c_str());
  printf("%s  ", padLeft(sizeProtect, "Protect").c_str());
  printf("%s  ", padLeft(sizeType, "Type").c_str());
  printf("\n");

  // iterate again and print
  ptr = 0;
  do {
    MEMORY_BASIC_INFORMATION memInfo;
    if (!VirtualQuery((void *)(ptr), &memInfo, sizeof(memInfo))) {
      break;
    }
    assert((size_t)memInfo.BaseAddress == ptr);

    printf("0x%016llX-", ptr);
    printf("0x%016llX  ", ptr + memInfo.RegionSize);
    printf("%s  ", padLeft(sizeState, getState(memInfo.State)).c_str());
    printf("%s  ", padLeft(sizeProtect, getProtect(memInfo.Protect)).c_str());
    printf("%s  ", padLeft(sizeType, getType(memInfo.Type)).c_str());
    printf("\n");

    ptr += memInfo.RegionSize;
  } while (ptr);

  printf("END\n");
  return 0;
}
