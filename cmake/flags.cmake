# From https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html

message("### SO info")
message("CMAKE_HOST_SYSTEM: ${CMAKE_HOST_SYSTEM}") # es: "Windows-10.0.16299"
message("CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}") # es: "Windows"
message("CMAKE_HOST_SYSTEM_PROCESSOR: ${CMAKE_HOST_SYSTEM_PROCESSOR}") # es: "AMD64"
message("CMAKE_HOST_SYSTEM_VERSION: ${CMAKE_HOST_SYSTEM_VERSION}") # es: "10.0.16299"

message("### SO flags")
if (CMAKE_HOST_WIN32)
    # When the host system is running Windows, including Windows 64-bit and MSYS.
    # Set to false on Cygwin
endif ()
if (CMAKE_HOST_UNIX)
    # When the host system is UNIX or UNIX like (i.e. APPLE and CYGWIN).
endif ()
if (CMAKE_HOST_APPLE)
    # When the host system is Apple macOS.
endif ()
if (ANDROID)
    message("ANDROID: SI")
else ()
    message("ANDROID: NO")
endif ()

message("### System Info")
message("CMAKE_SYSTEM: ${CMAKE_SYSTEM}")

message("### Compiler Info")
message("CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message("CMAKE_FLAGS: ${CMAKE_FLAGS}")
message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message("CMAKE_C_COMPILER_ID: ${CMAKE_C_COMPILER_ID}")

message("### Per project info")
message("public_sandbox_BINARY_DIR: ${public_sandbox_BINARY_DIR}")
message("public_sandbox_DESCRIPTION: ${public_sandbox_DESCRIPTION}")
message("public_sandbox_VERSION: ${public_sandbox_VERSION}")
message("public_sandbox_SOURCE_DIR: ${public_sandbox_SOURCE_DIR}")
message("public_sandbox_HOMEPAGE_URL: ${public_sandbox_HOMEPAGE_URL}")
