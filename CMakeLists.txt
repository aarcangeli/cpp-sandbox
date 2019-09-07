cmake_minimum_required(VERSION 3.12)
project(public_sandbox C CXX ASM)

set(CMAKE_CXX_STANDARD 11)

if (MINGW)
    # remove dependencies to libstdc++-6.dll, libgcc_s_dw2-1.dll, libwinpthread-1.dll
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif ()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(cmake)
add_subdirectory(cpp)
add_subdirectory(windows)
