include(CheckCXXSourceRuns)

CHECK_CXX_SOURCE_RUNS("
    #include <thread>
    int returnValue = 1;
    void work() {
        returnValue = 0;
    }
    int main() {
        std::thread test(&work);
        test.join();
        return returnValue;
    }
" HAVE_THREAD_H)

IF (HAVE_THREAD_H)
    add_executable(async async.cpp)
endif ()
