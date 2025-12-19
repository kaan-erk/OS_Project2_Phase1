#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 25
int in = 0;
int out = 0;

sem_t empty;
sem_t full;
sem_t mutex;

int produce_even() { return (rand() % 5000) * 2 + 1; }

int buffer[BUFFER_SIZE];
int count = 0;  // Number of items in the buffer

void* producer(void* arg) {
    while (1) {
        //TODO
        int produced = produce_even();
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[in] = produced;
        usleep(500);
        printf("Produced: %d from index %d\n", produced, out);
        in = (in + 1) % BUFFER_SIZE;
        count++;
        sem_post(&full);
        sem_post(&mutex);
        sleep(rand() % 2); // Sleep for a random period
    }
    return NULL;
}

void* consumer(void* arg) {    
    while (1) {
        //TODO
        sem_wait(&full);
        sem_wait(&mutex);
        int consumed = buffer[out];
        printf("Consumed: %d from index %d\n", consumed, out);
        out = (out + 1) % BUFFER_SIZE;
        count--;
        sem_post(&empty);
        sem_post(&mutex);
        sleep(rand() % 2); // Sleep for a random period
    }
    return NULL;
}

int main() {
    pthread_t producers, consumers;
    int producer_ids, consumer_ids;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    // Create producer threads
        producer_ids =  1;
        pthread_create(&producers, NULL, producer, &producer_ids);
   
    // Create consumer threads
        consumer_ids =  1;
        pthread_create(&consumers, NULL, consumer, &consumer_ids);
   
    // Join threads (in this case, the program will run indefinitely)
        pthread_join(producers, NULL);
        pthread_join(consumers, NULL);
    
    return 0;
}
