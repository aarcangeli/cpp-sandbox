#include <cstdio>
#include <unistd.h>
#include "pthread.h"

/*
 * In this example we are creating 5 different thread that
 * try to lock the same mutex for a second.
 *
 * This test shows that a mutex is locked in an undefined order.
 *
 * Typical output:
 *   - thread 0 started
 *   - thread 0 stopped
 *   - thread 2 started
 *   - thread 2 stopped
 *   - thread 3 started
 *   - thread 3 stopped
 *   - thread 1 started
 *   - thread 1 stopped
 *   - thread 4 started
 *   - thread 4 stopped
 *   - END
 */

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

void *demoSync(void *_id) {
    size_t id = (size_t) _id;
    pthread_mutex_lock(&myMutex);
    printf("  - thread %zu started\n", id);

    sleep(1);

    printf("  - thread %zu stopped\n", id);
    pthread_mutex_unlock(&myMutex);
    return nullptr;
}

int main() {
    const int totalThreads = 5;
    pthread_t thread[totalThreads];

    for (size_t i = 0; i < totalThreads; ++i) {
        pthread_create(&thread[i], nullptr, &demoSync, (void *) i);
    }

    for (pthread_t i : thread) {
        pthread_join(i, nullptr);
    }

    printf("  - END\n");
}
