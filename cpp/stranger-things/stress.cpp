
#include <thread>

void work() {
    printf("Started\n");
    while (1);
}

int main() {
    std::thread *pThread;
    for (int i = 0; i < 12; ++i) {
        pThread = new std::thread(&work);
    }
    pThread->join();
}
