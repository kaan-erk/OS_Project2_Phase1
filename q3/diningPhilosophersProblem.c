#include <pthread.h>
#include <stdio.h>
#include <unistd.h>   // for sleep()
#include <semaphore.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };

// This sempahore are created to help you 
sem_t mutex;
sem_t S[N];

void test(int phnum) {
    if (state[phnum] == HUNGRY &&
        state[LEFT] != EATING &&
        state[RIGHT] != EATING)
    {
        state[phnum] = EATING;
        sleep(2);

        printf("Philosopher %d takes fork %d and %d\n",
               phnum + 1, LEFT + 1, phnum + 1);

        printf("Philosopher %d is Eating\n", phnum + 1);

        sem_post(&S[phnum]);
    }
}

// take up chopsticks
void take_fork(int phnum) {
    // TODO: use semaphore wait  → solution:
    sem_wait(&mutex);

    state[phnum] = HUNGRY;
    printf("Philosopher %d is Hungry\n", phnum + 1);

    test(phnum);  // Attempt to eat

    // TODO: use semaphore post → solution:
    sem_post(&mutex);


    // If test() didn't allow eating, wait here
    sem_wait(&S[phnum]);
}

// put down chopsticks
void put_fork(int phnum) {
    // TODO: use semaphore wait → solution:
    sem_wait(&mutex);
    

    state[phnum] = THINKING;
    printf("Philosopher %d putting fork %d and %d down\n",
           phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is thinking\n", phnum + 1);

    // After putting chopsticks down, check neighbors
    test(LEFT);
    test(RIGHT);

    // TODO: use semaphore post → solution:
    sem_post(&mutex);
}

void* philosopher(void* num) {
    while (1) {
        int* i = num;
        sleep(1);
        take_fork(*i);
        sleep(0);
        put_fork(*i);
    }
}

int main() {
    int i;
    pthread_t thread_id[N];

    // TODO
    // Intialization

    sem_init(&mutex, 0, 1);
    for (i = 0; i < N; i++)
        sem_init(&S[i], 0, 0);
   
    for (i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL,
                       philosopher, &phil[i]);
        printf("Philosopher %d is thinking\n", i + 1);
    }

    for (i = 0; i < N; i++)
        pthread_join(thread_id[i], NULL);

    return 0;
}
