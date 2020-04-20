//Heman Baral
//CMSC 312 
//Assignment 3
//Printer Server SJF



#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>

#define PRINT_QUEUE 30
#define PRINT_JOB  30

static int count_jobs;
static int print_job_span;

///////////////////////////////////////////////////////////////////////

int print_size(int min, int max){

    return rand() % (max + 1 - min) + min;
}


///////////////////////////////////////////////////////////////////////

struct job
{
  int num;

  struct job *next;

};


///////////////////////////////////////////////////////////////////////


typedef struct {

  struct job *front;

  int isFull; 

  int isEmpty;

  int job_end;

} print_job;


///////////////////////////////////////////////////////////////////////

print_job *current_job (void);

void rm_queue (print_job *q);

void job_add (print_job *q, int in);

void delete_job (print_job *q, int *out);

void *user_thread (void *args);

void *printer_thread (void *args);


sem_t mutex,empty,full;



///////////////////////////////////////////////////////////////////////

//adds user thread to the queue by checking the logic

void *user_thread (void *q)
{


  int printJobs = (rand() % 20) + 1;


  sem_wait(&full);


  count_jobs += printJobs;


  sem_post(&full);


  print_job *SJF;


  SJF = (print_job *)q;

  int i;

  for (i = 0; i < printJobs; i++) {


    sem_wait(&mutex);


    while (SJF->isFull) {


      usleep (10000);
    }

    int print_no = print_size(100, 1000);



    printf ("Producer %d added %d to buffer\n\n", &print_no, print_no);


    job_add (SJF, print_no);


    sem_post(&mutex);


    usleep (10000);
  }
  return (NULL);
}


///////////////////////////////////////////////////////////////////////


//consumer thread dequeue producer formt the buffer

void *printer_thread (void *q)
{
  print_job *SJF;

  SJF = (print_job *)q;

  int d;

  while((SJF->job_end != 1) || (!SJF->isEmpty)){

    sem_wait(&empty);

    while (SJF->isEmpty) {

      usleep(1000);

    }

    delete_job (SJF, &d);

    sem_post(&empty);


    printf ("Consumer ID:%d dequeue %d %d from buffer \n\n",&SJF, &d, d);

    usleep(100000);

  }

  return (NULL);

}


///////////////////////////////////////////////////////////////////////


//fuction to do current job that is assigned for print job
print_job *current_job (void)

{

	print_job *q;

	q = (print_job *)malloc (sizeof (print_job));

	if (q == NULL) return (NULL);

	q->isEmpty = 1;

	q->isFull = 0;

	q->front = (struct job *)malloc(sizeof(struct job));

  q->front = NULL;

	return (q);
}


///////////////////////////////////////////////////////////////////////

//removing the queue after all the jobs are done

void rm_queue (print_job *q)

{
  free (q->front);

	free (q);
}


///////////////////////////////////////////////////////////////////////

//funtion to add jobs to the working queue 

void job_add (print_job *q, int in)

{
  struct job *temp, *timer;

  temp = (struct job *)malloc(sizeof(struct job));

  temp->num=in;

  temp->next=NULL;

  print_job_span++;

  if(q->front == NULL){

    q->front = temp;

  } else if(q->front->num > in) {

    temp->next = q->front;

    q->front = temp; 

  } 

  else {

    timer=q->front;

    while(timer->next != NULL && (timer->next)->num <= in) {

      timer = timer->next;

    }

    temp->next = timer->next;

    timer->next = temp;

  }

  q->isEmpty = 0;

  if(print_job_span > PRINT_QUEUE){

    q->isFull = 1;

  }

  return;

}


///////////////////////////////////////////////////////////////////////

//function to delete the job that has been completed 

void delete_job (print_job *q, int *out)

{
  if(q->front != NULL) {

    *out = q->front->num;

    q->front = q->front->next;

    print_job_span--;

  }

  if(print_job_span == 0){

    q->isEmpty = 1;

  }

  q->isFull = 0;

  return;

}


///////////////////////////////////////////////////////////////////////

int main (int argc, char *argv[])

{
  
  print_job *SJF;

  pthread_t producer[PRINT_JOB], consumer[PRINT_JOB];

  int t_user, p_thread;

  srand(time(0));

  if(argc != 3){

    printf("Error!!! Enter 2 Arguements\n");

    return -1;

  } else {

    t_user = atoi(argv[1]);

    p_thread = atoi(argv[2]);

  }


///////////////////////////////////////////////////////////////////////


  sem_init (&mutex, 0, 1);

  sem_init (&empty, 0, 1);

  sem_init (&full, 0, 1);


///////////////////////////////////////////////////////////////////////


  //checking if number of producer is correct

  if((t_user > PRINT_JOB) || (t_user < 0)) {

    printf("Error!!! Enter Number of User\n");

    return -1;
  
  }

   //checking if number of consumer is correct

  if((p_thread > PRINT_JOB) || (p_thread < 0)) {

    printf("Error!!! Enter Number of Printer\n");

    return -1;

  }

  //Allocating memory for the job

  SJF = current_job ();

  if (SJF ==  NULL) {

    fprintf (stderr, "Error!!! Fail to Allocate Memory\n");

    exit (1);

  }

  ///////////////////////////////////////////////////////////////////////

  SJF->job_end = 0;

  clock_t timer = clock();

  int i;

  for(i = 0; i < t_user; i++){

    pthread_create (&producer[i], NULL, user_thread, SJF);
  }

  for(i = 0; i < p_thread; i++){

    pthread_create (&consumer[i], NULL, printer_thread, SJF);
  }

  for(i = 0; i < t_user; i++){

    pthread_join (producer[i], NULL);
  }

  sem_wait (&full);

  SJF->job_end = 1;

  sem_post (&full);

  for(i = 0; i < p_thread; i++){

    pthread_join (consumer[i], NULL);

  }

  ///////////////////////////////////////////////////////////////////////

  timer = clock() - timer;

  double tota_time = ((double)timer)/(CLOCKS_PER_SEC/1000);


  printf("\n\nTotal Print Jobs: %d\n\n", count_jobs);

  printf("Average Time in msec: %f \n\n", tota_time/count_jobs);

  printf("Total Run Time %f\n\n", tota_time);


  
  rm_queue (SJF);
  
  sem_destroy (&mutex);

  sem_destroy (&empty);

  sem_destroy (&full);

  return 0;
}


