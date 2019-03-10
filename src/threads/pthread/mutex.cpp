#include <cstdio>
#include <unistd.h>
#include "pthread.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool signalized = false;

void *demoSync(void *id) {
    pthread_mutex_lock(&mutex);

    printf("  - [%zu] started\n", (size_t) id);
    sleep(1);
    printf("  - [%zu] stopped\n", (size_t) id);

    pthread_mutex_unlock(&mutex);
    return nullptr;
}

int main() {
    const int totalThreads = 20;
    pthread_t thread[totalThreads];

    for (size_t i = 0; i < totalThreads; ++i) {
        pthread_create(&thread[i], 0, &demoSync, (void *) i);
        usleep(10);
    }

    for (int i = 0; i < totalThreads; ++i) {
        pthread_join(thread[i], nullptr);
    }

    printf(" - END\n");
}
