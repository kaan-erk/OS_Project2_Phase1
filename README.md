# Process Synchronization with Pthreads and Semaphores

This repository contains C implementations of classic operating system synchronization problems using **Pthreads** and **POSIX semaphores**.

Implemented problems:
- Producer–Consumer (Bounded Buffer)
- Sleeping Barber Problem
- Dining Philosophers Problem

---

## How to Compile and Run

All programs are written in C and require the pthread library.

### Compile
```bash
gcc program_name.c -o program_name -lpthread
Run
bash
Kodu kopyala
./program_name
Replace program_name.c with the corresponding source file
(e.g., producer_consumer.c, sleeping_barber.c, dining_philosophers.c)

Program Explanation
Producer–Consumer Problem
The producer generates even random numbers and places them into a bounded buffer of size 25.
The consumer removes items from the buffer and prints them.

The buffer is shared between producer and consumer threads and accessed in a circular manner using in and out indices.

Sleeping Barber Problem
The barber shop has:

One barber

A limited number of waiting chairs

Multiple customers arriving at random times

If no customers are present, the barber sleeps.
Customers either wait if a chair is available or leave if the shop is full.
The barber serves customers one at a time.

Dining Philosophers Problem
Five philosophers alternate between thinking and eating.
Each philosopher needs two forks (left and right) to eat.

The solution ensures that:

No two adjacent philosophers eat at the same time

Shared forks are accessed safely

Deadlock is avoided

Synchronization Explanation
Producer–Consumer Synchronization
empty semaphore: Counts available buffer slots

full semaphore: Counts filled buffer slots

mutex semaphore: Ensures mutual exclusion while accessing the buffer

These prevent race conditions and ensure correct producer–consumer coordination.

Sleeping Barber Synchronization
customers_sem: Counts waiting customers and wakes the barber

barber_sem: Signals when the barber is ready

seat_belt: Synchronizes haircut completion

mutex: Protects shared waiting room data

This design prevents race conditions and avoids deadlocks by releasing the mutex before blocking on semaphores.

Dining Philosophers Synchronization
mutex semaphore: Protects shared philosopher states

S[i] semaphores: Control when each philosopher is allowed to eat

The test() function ensures a philosopher eats only if both neighbors are not eating, preventing deadlock.
Starvation is minimized by giving neighbors a chance to eat after forks are released.
