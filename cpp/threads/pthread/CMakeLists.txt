include(CheckIncludeFile)
check_include_file("pthread.h" HAVE_PTHREAD_H)

if (HAVE_PTHREAD_H)
    link_libraries(pthread)
    add_executable(pthread_mutex mutex.cpp)
    add_executable(pthread_monitor monitor.cpp)
endif ()
