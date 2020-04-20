
//Heman Baral
//CMSC 312 
//Assignment 3
//Printer Server FIFO




#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>

#define PRINT_QUEUE 30
#define PRINT_JOB  30

static int print_sum;

sem_t mutex,empty,full; 


/////////////////////////////////////////////////////////
typedef struct {

  int buffer_size[PRINT_QUEUE];

  long front;

  long rare;

  int queue_max;

  int isEmpty;

  int queue_end;

} print_job;


/////////////////////////////////////////////////////////

int print_size(int min, int max){

    return rand() % (max + 1 - min) + min;
}

////////////////////////////////////////////////////////
print_job *current_job (void);

void rm_queue (print_job *array);

void job_add (print_job *array, int in);

void delete_job (print_job *array, int *out);

void *user_thread (void *args);

void *printer_thread (void *args);


/////////////////////////////////////////////////////////

//method to add producer to the buffere/logic to check if the queue has space or not
void *user_thread (void *array)
{

  int printJobs = (rand() % 20) + 1;

  sem_wait(&full);

  print_sum += printJobs;

  sem_post(&full);

  print_job *FIFO;

  FIFO = (print_job *)array;

  int i;
  
  for (i = 0; i < printJobs; i++) {

    sem_wait(&mutex);

    while (FIFO->queue_max) {

      usleep (10000);

    }

    int printSize = print_size(100, 1000);

    printf ("Producer %d added %d to buffer\n\n", &printSize, printSize);


    job_add (FIFO, printSize);

    sem_post(&mutex);

    usleep (10000);
  }
  return (NULL);
}


/////////////////////////////////////////////////////////


void *printer_thread (void *array)

{

  print_job *FIFO;

  FIFO = (print_job *)array;

  int d;

  while((FIFO->queue_end != 1) || (!FIFO->isEmpty)){

    sem_wait(&empty);

    while (FIFO->isEmpty) {

      usleep(1000);

    }
    delete_job (FIFO, &d);

    sem_post(&empty);

    //printf ("printer Thread: \t%d\n\n", d);

    printf ("Consumer ID:%d dequeue %d %d from buffer \n\n",&FIFO, &d, d);

    usleep(100000);
  }

  return (NULL);
}

/////////////////////////////////////////////////////////

print_job *current_job (void)

{
  print_job *array;

  array = (print_job *)malloc (sizeof (print_job));

  if (array == NULL) return (NULL);

  array->isEmpty = 1;

  array->queue_max = 0;

  array->front = 0;

  array->rare = 0;  

  return (array);
}

/////////////////////////////////////////////////////////

//remove the queue after all the job is done

void rm_queue (print_job *array)
{
  free (array);
}

/////////////////////////////////////////////////////////


//adds job to the queueu

void job_add (print_job *array, int in)
{
  array->buffer_size[array->rare] = in;

  array->rare++;

  if (array->rare == PRINT_QUEUE)

    array->rare = 0;

  if (array->rare == array->front)

    array->queue_max = 1;

  array->isEmpty = 0;

  return;
}

/////////////////////////////////////////////////////////


//deletes job after it is done (dequeue)

void delete_job (print_job *array, int *out)
{
  *out = array->buffer_size[array->front];

  array->front++;

  if (array->front == PRINT_QUEUE)

    array->front = 0;

  if (array->front == array->rare)

    array->isEmpty = 1;

  array->queue_max = 0;

  return;
} 


/////////////////////////////////////////////////////////




int main (int argc, char *argv[])
{
  print_job *FIFO;

  pthread_t producer[PRINT_JOB], consumer[PRINT_JOB];
  
  int num_user, num_printer;
  
  srand(time(0));

  if(argc != 3){
  
    printf("Error!!!\n");
  
    printf("Enter 2 Arguement\n");
  
    return -1;
  
  } else {
  
    num_user = atoi(argv[1]);
  
    num_printer = atoi(argv[2]);
  
  }

  /////////////////////////////////////////////////////////

  //
  sem_init (&mutex, 0, 1);
  
  sem_init (&empty, 0, 1);
  
  sem_init (&full, 0, 1);


  /////////////////////////////////////////////////////////

  
  //Checks if the number of producer is invalid
  if((num_user > PRINT_JOB) || (num_user < 0)) {
  
    printf("Error!!! Enter Valid User Number\n");
  
    return -1;
  
  }

   //Checks if the number of consumer is invalid
  if((num_printer > PRINT_JOB) || (num_printer < 0)) {
  
    printf("Error!!! Enter Valid Printer Number\n");
  
    return -1;
  
  }

  

  //Allocating memory for current job

  FIFO = current_job ();
  
  if (FIFO ==  NULL) {
  
    fprintf (stderr, "Error!!! Allocating Memory\n");
  
    exit (1);
  
  }

  FIFO->queue_end = 0;

  clock_t timer = clock();

  int i;

  for(i = 0; i < num_user; i++){
  
    pthread_create (&producer[i], NULL, user_thread, FIFO);
  
  }

  for(i = 0; i < num_printer; i++){
  
    pthread_create (&consumer[i], NULL, printer_thread, FIFO);
  
  }

  
  for(i = 0; i < num_user; i++){
  
    pthread_join (producer[i], NULL);
  
  }

  sem_wait (&full);
  
  FIFO->queue_end = 1;
  
  sem_post (&full);

  for(i = 0; i < num_printer; i++){
  
    pthread_join (consumer[i], NULL);
  
  }


  /////////////////////////////////////////////////////////

  timer = clock() - timer;
  
  double total_time = ((double)timer)/(CLOCKS_PER_SEC/1000);
  
  
  printf("\n\n");

  printf("Total Print Jobs: %d\n\n",print_sum);
  
  printf("Avarage Time in msec: %f \n\n", total_time/print_sum);

  printf("Total Run Time: %f\n\n", total_time);
    
  
  rm_queue (FIFO);
  
  sem_destroy (&mutex);
  
  sem_destroy (&empty);
  
  sem_destroy (&full);

  return 0;
}
