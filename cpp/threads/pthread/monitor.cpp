#include <cstdio>
#include <unistd.h>
#include "pthread.h"

/*
 * In this example we are testing the a condition variable (pthread_cond_t myCond)
 *
 * 'predicate' and 'mustExit' is shared variables protected by 'myMutex',
 * so you must ensure that 'myMutex' is locked before using it (both
 * read and update)
 *
 * There are two threads: a controller and a monitor.
 *
 *   - The controller set the 'predicate' flag when it require an action from the 'monitor' thread
 *   - The monitor sleeps when the 'predicate' flag is unset and does something if set
 *
 * Typical output:
 *   - [monitor] Started
 *   - [monitor] Sleeping
 *   - [controller] Signaling monitor
 *   - [monitor] Awakened
 *   - [monitor] Sleeping
 *   - [controller] Signaling monitor
 *   - [monitor] Awakened
 *   - [monitor] Sleeping
 *   - [controller] Signaling monitor
 *   - [monitor] Awakened
 *   - [monitor] Sleeping
 *   - [controller] Signaling monitor
 *   - [monitor] Awakened
 *   - [monitor] Sleeping
 *   - [controller] Signaling monitor
 *   - [monitor] Awakened
 *   - [monitor] Sleeping
 *   - [controller] Signaling exit
 *   - [monitor] Stopped
 */

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t myCond = PTHREAD_COND_INITIALIZER;
bool predicate = false;
bool mustExit = false;

void *monitorThread(void *unused) {
    printf("  - [monitor] Started\n");

    while (true) {
        pthread_mutex_lock(&myMutex);
        printf("  - [monitor] Sleeping\n");
        while (!predicate) {
            pthread_cond_wait(&myCond, &myMutex);
        }
        predicate = false;
        if (mustExit) {
            pthread_mutex_unlock(&myMutex);
            break;
        }
        pthread_mutex_unlock(&myMutex);

        printf("  - [monitor] Awakened\n");
        // working
    }

    printf("  - [monitor] Stopped\n");
    return nullptr;
}

void signalMonitor() {
    pthread_mutex_lock(&myMutex);
    pthread_cond_signal(&myCond);
    predicate = true;
    pthread_mutex_unlock(&myMutex);
}

void signalExit() {
    pthread_mutex_lock(&myMutex);
    pthread_cond_signal(&myCond);
    predicate = true;
    mustExit = true;
    pthread_mutex_unlock(&myMutex);
}

int main() {
    pthread_t threadSignal;

    pthread_create(&threadSignal, nullptr, &monitorThread, nullptr);
    sleep(3);

    for (int j = 0; j < 5; ++j) {
        printf("  - [controller] Signaling monitor\n");
        signalMonitor();
        sleep(3);
    }

    printf("  - [controller] Signaling exit\n");
    signalExit();

    pthread_join(threadSignal, nullptr);

    return 0;
}
