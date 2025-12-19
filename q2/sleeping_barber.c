#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define DEFAULT_CUSTOMERS 7
#define DEFAULT_CHAIRS 5
#define DEFAULT_MAX_ARRIVAL_MS 500
#define DEFAULT_MAX_HAIRCUT_MS 500

int num_customers;
int num_chairs;
int max_arrival_ms;
int max_haircut_ms;

int *waiting_room;
int front = 0, rear = 0;
int waiting_count = 0;
int served_count = 0;

pthread_mutex_t mutex;
sem_t customers_sem;
sem_t barber_sem;
sem_t seat_belt;

void enqueue(int id) {
    waiting_room[rear] = id;
    rear = (rear + 1) % num_chairs;
    waiting_count++;
}

int dequeue() {
    int id = waiting_room[front];
    front = (front + 1) % num_chairs;
    waiting_count--;
    return id;
}

void *barber_thread(void *arg) {
    (void)arg;
    while (1) {
        pthread_mutex_lock(&mutex);
        while (waiting_count == 0) {
            printf("Barber goes to sleep.\n");
            fflush(stdout);
            pthread_mutex_unlock(&mutex);
            sem_wait(&customers_sem);
            pthread_mutex_lock(&mutex);
        }
        int cust = dequeue();
        printf("Barber calls next customer: Customer %d.\n", cust);
        fflush(stdout);
        sem_post(&barber_sem);
        pthread_mutex_unlock(&mutex);

        printf("Barber is cutting hair of Customer %d.\n", cust);
        fflush(stdout);
        usleep((rand() % max_haircut_ms) * 1000);

        printf("Barber finished cutting hair of Customer %d.\n", cust);
        fflush(stdout);
        sem_post(&seat_belt);
    }
    return NULL;
}

void *customer_thread(void *arg) {
    int id = *(int *)arg;
    free(arg);

    usleep((rand() % max_arrival_ms) * 1000);
    printf("Customer %d arrives.\n", id);
    fflush(stdout);

    pthread_mutex_lock(&mutex);
    if (waiting_count < num_chairs) {
        enqueue(id);
        printf("Customer %d sits in waiting room (%d/%d chairs occupied).\n",
               id, waiting_count, num_chairs);
        fflush(stdout);
        sem_post(&customers_sem);
        pthread_mutex_unlock(&mutex);

        sem_wait(&barber_sem);
        printf("Customer %d is getting a haircut.\n", id);
        fflush(stdout);

        sem_wait(&seat_belt);
        printf("Customer %d leaves after haircut.\n", id);
        fflush(stdout);
    } else {
        printf("Customer %d leaves (no available chair).\n", id);
        fflush(stdout);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *customer_generator(void *arg) {
    (void)arg;
    int id = 1;
    while (1) {
        int *cid = malloc(sizeof(int));
        *cid = id++;
        pthread_t t;
        pthread_create(&t, NULL, customer_thread, cid);
        pthread_detach(t);
        usleep((rand() % max_arrival_ms) * 1000);
    }
    return NULL;
}

int main(int argc, char **argv) {
    srand(time(NULL));
    num_customers = (argc > 1) ? atoi(argv[1]) : DEFAULT_CUSTOMERS;
    num_chairs = (argc > 2) ? atoi(argv[2]) : DEFAULT_CHAIRS;
    max_arrival_ms = (argc > 3) ? atoi(argv[3]) : DEFAULT_MAX_ARRIVAL_MS;
    max_haircut_ms = (argc > 4) ? atoi(argv[4]) : DEFAULT_MAX_HAIRCUT_MS;

    waiting_room = malloc(sizeof(int) * (num_chairs > 0 ? num_chairs : 1));

    pthread_mutex_init(&mutex, NULL);
    sem_init(&customers_sem, 0, 0);
    sem_init(&barber_sem, 0, 0);
    sem_init(&seat_belt, 0, 0);

    pthread_t barber;
    pthread_create(&barber, NULL, barber_thread, NULL);
    pthread_detach(barber);

    pthread_t gen;
    pthread_create(&gen, NULL, customer_generator, NULL);
    pthread_detach(gen);

    while (1) {
        sleep(1);
    }

    return 0;
}