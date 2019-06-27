include(ResourceCompiler.cmake)

# example with glob
set(SOURCES example.cpp)


compile_resources(SOURCES myset resources/*.png)

message(STATUS "SOURCES = ${SOURCES}")
add_executable(ResourceCompiler_example ${SOURCES})

