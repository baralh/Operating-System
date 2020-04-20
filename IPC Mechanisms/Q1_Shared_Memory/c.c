//Heman Baral
//CMSC 312


/*
 * shm-client - client program to demonstrate shared memory.
 * shm-client - not sure of the origin of these code segments.
 * possibly: David Marshalls course : http://www.cs.cf.ac.uk/Dave/C/CE.html
 * or Steve Holmes : http://www2.its.strath.ac.uk/courses/c/
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define SHMSZ     27

int
main()
{
    int shmid, shmid2;
    key_t key, key2;
    char *shm, *shm2, *share, *s;

    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 5678;
    key2 = 5679;

    /*
     * Locate the segment.
     */



    /*
     shmget creates the process
     */
    if( (shmid = shmget(key, SHMSZ, 0666)) < 0 ) 
    {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */

    /*
     shmat attaches process
     */

    if( (shm = shmat(shmid, NULL, 0)) == (char *) -1 )
    {
        perror("shmat");
        exit(1);
    }


      /*
     shmget creates the process
     */

    if( (shmid2 = shmget(key2, SHMSZ, 0666)) < 0 )
    {
        perror("shmget");
        exit(1);
    }


      /*
     shmget creates the process
     */

    if( (shm2 = shmget(shmid2, SHMSZ, 0666)) < 0 )
    {
        perror("shmget");
        exit(1);
    }






    // while( *shm != '1' )
    // {
    //     sleep(1);
    // }



    //waiting pointer to first bit of memory equals 2

    while( *shm != '2' )
    {
        sleep(1);
    }

 


 


    shm2 = "memory";

    s = "memory";

  /*
     * Now put some things into the memory for the
     * other process to read.
     */

    for( s = shm2; *s != (char) NULL; s++ )
    {
    putchar(*s);
    }

    printf("\n");


    //printing from memory
    //printf("%s\n",shm2);


 
    *shm = '3';


    return 0;






}