//HEMAN BARAL
//CMSC 312
//ASSIGNMENT 2 Q1 incorrect version


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
 
#define SIZE 5
#define NUMB_THREADS 6
#define PRODUCER_LOOPS 2
 
#define MINIMUM(x,y) ((x<y) ? x:y)

typedef int buffer_t;
buffer_t buffer[SIZE];
int buffer_index;
 
pthread_mutex_t buffer_mutex;
/* initially buffer will be empty.  full_sem
   will be initialized to buffer SIZE, which means
   SIZE number of producer threads can write to it.
   And empty_sem will be initialized to 0, so no
   consumer can read from buffer until a producer
   thread posts to empty_sem */
sem_t full_sem;  /* when 0, buffer is full */
sem_t empty_sem; /* when 0, buffer is empty. Kind of
                    like an index for the buffer */
 
//---global variable

int val = 1;
int input;
//int min, test;

sem_t gate;
sem_t mutex;





void *pc(){

    sem_wait(&gate);

    sem_wait(&mutex);

    val = val - 1;

    if(val > 0){

        sem_post(&gate);

    }

    sem_post(&mutex);
}

void *vc(){

    sem_wait(&mutex);

    val = val + 1;

    if(val == 1){

        sem_post(&gate);

    }

    sem_post(&mutex);
}
 
 
void insertbuffer(buffer_t value) {
    if (buffer_index < SIZE) {
        buffer[buffer_index++] = value;
    } else {
        printf("Buffer overflow\n");
    }
}
 
buffer_t dequeuebuffer() {
    if (buffer_index > 0) {
        return buffer[--buffer_index]; // buffer_index-- would be error!
    } else {
        printf("Buffer underflow\n");
    }
    return 0;
}
 
 
void *producer(void *thread_n) {
    int thread_numb = *(int *)thread_n;
    buffer_t value;
    int i=0;
    while (i++ < PRODUCER_LOOPS) {
        sleep(rand() % 10);
        value = rand() % 100;

        //sem_wait(&full_sem); // sem=0: wait. sem>0: go and decrement it
        //---change sem_wait to pc

        pc(&buffer_mutex);

        /* possible race condition here. After this thread wakes up,
           another thread could aqcuire mutex before this one, and add to list.
           Then the list would be full again
           and when this thread tried to insert to buffer there would be
           a buffer overflow error */
        pthread_mutex_lock(&buffer_mutex); /* protecting critical section */
        insertbuffer(value);
        pthread_mutex_unlock(&buffer_mutex);
       // sem_post(&empty_sem); // post (increment) emptybuffer semaphore
        //change sem_post to VC

         vc(&buffer_mutex);

        printf("Producer %d added %d to buffer\n", thread_numb, value);
               printf("Value is : %d, Mutex is: %d\n", val, input);

    }
    pthread_exit(0);
}
 
void *consumer(void *thread_n) {
    int thread_numb = *(int *)thread_n;
    buffer_t value;
    int i=0;


    while (i++ < PRODUCER_LOOPS) {

        //sem_wait(&empty_sem);
      //---change sem wait to PC
      pc(&buffer_mutex);


        /* there could be race condition here, that could cause
           buffer underflow error */
        pthread_mutex_lock(&buffer_mutex);
        value = dequeuebuffer(value);
        pthread_mutex_unlock(&buffer_mutex);
        //sem_post(&full_sem); // post (increment) fullbuffer semaphore
        //change sem_post to vc
        vc(&buffer_mutex);

       printf("Consumer %d dequeue %d from buffer\n", thread_numb, value);
              printf("Value is : %d, Mutex is: %d\n", val, input);

   }
    pthread_exit(0);
}
 
int main(int argc, char **argv) {
    buffer_index = 0;


       // min = MINIMUM(1, val);
       // sem_init(&gate, 0, min);
       // sem_init(&mutex, 0, 1);


         if(val>0){

          input = 1;

         }

         else{

       input = 0;

          }



       //printf("Value is : %d, Mutex is: %d\n", val, input);
       sem_init(&gate, 0, 0);
       sem_init(&mutex, 0, 1);


 
    pthread_mutex_init(&buffer_mutex, NULL);

    sem_init(&full_sem, // sem_t *sem
             0, // int pshared. 0 = shared between threads of process,  1 = shared between processes
             SIZE); // unsigned int value. Initial value
    sem_init(&empty_sem,
             0,
             0);
    /* full_sem is initialized to buffer size because SIZE number of
       producers can add one element to buffer each. They will wait
       semaphore each time, which will decrement semaphore value.
       empty_sem is initialized to 0, because buffer starts empty and
       consumer cannot take any element from it. They will have to wait
       until producer posts to that semaphore (increments semaphore
       value) */


    pthread_t thread[NUMB_THREADS];
    int thread_numb[NUMB_THREADS];
    int i;
    for (i = 0; i < NUMB_THREADS; ) {
        thread_numb[i] = i;
        pthread_create(thread + i, // pthread_t *t
                       NULL, // const pthread_attr_t *attr
                       producer, // void *(*start_routine) (void *)
                       thread_numb + i );  // void *arg
        i++;
        thread_numb[i] = i;
        // playing a bit with thread and thread_numb pointers...
        pthread_create(&thread[i], // pthread_t *t
                       NULL, // const pthread_attr_t *attr
                       consumer, // void *(*start_routine) (void *)
                       &thread_numb[i]);  // void *arg
        i++;
    }
 
    for (i = 0; i < NUMB_THREADS; i++)
        pthread_join(thread[i], NULL);
 
    pthread_mutex_destroy(&buffer_mutex);
    sem_destroy(&full_sem);
    sem_destroy(&empty_sem);
 
    return 0;
}