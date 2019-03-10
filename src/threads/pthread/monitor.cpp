#include <cstdio>
#include <unistd.h>
#include "pthread.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
bool predicate = false;
bool mustExit = false;

void *monitorThread(void *unused) {
    printf("  - [monitor] Started\n");

    while (true) {
        pthread_mutex_lock(&mutex);
        predicate = false;
        printf("  - [monitor] Sleeping\n");
        while (!predicate) {
            pthread_cond_wait(&cond, &mutex);
        }
        if (mustExit) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);

        printf("  - [monitor] Awaked\n");
        // working
    }

    printf("  - [monitor] Stopped\n");
    return nullptr;
}

void signalMonitor() {
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    predicate = true;
    pthread_mutex_unlock(&mutex);
}

void signalExit() {
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    predicate = true;
    mustExit = true;
    pthread_mutex_unlock(&mutex);
}

int main() {
    pthread_t threadSignal;

    pthread_create(&threadSignal, 0, &monitorThread, nullptr);
    sleep(3);

    for (int j = 0; j < 5; ++j) {
        printf("  - Signaling monitor\n");
        signalMonitor();
        sleep(3);
    }

    printf("  - Signaling exit\n");
    signalExit();

    pthread_join(threadSignal, nullptr);

    return 0;
}
