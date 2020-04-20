//Heman Baral
//CMSC 312

#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>



#define SHMSZ     27

int main() {


	int shmid, shmid2;
    key_t key, key2;
    char *shm,*shm2, *s;



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

    if( (shm2 = shmat(shmid2, NULL, 0)) == (char *) -1 )
    {
        perror("shmat");
        exit(1);
    }


*shm = '1';




//pid_t is process id type	

pid_t pid;


/* fork another process */

pid = fork();




/* error occurred */
if (pid < 0) { 


fprintf(stderr, "Fork Failed");

exit(-1); 

}

/* child process */ 

else if (pid == 0) { 



	//Process b

  while( *shm != '1' )
    {
        sleep(1);
    }


 shm2 = "shared";

 s = "shared";

 for( s = shm2; *s != (char) NULL; s++ )
 {
      
 putchar(*s);

}


 printf("\n");



    *shm = '2';



return 0;
//execlp("/bin/ls", "ls", NULL);

}

 else {

      wait (NULL);
    




/* parent process */






	   /* parent will wait for the child to complete */ 

	//pid_t is process id type	

	pid_t pid2;


	/* fork another process */

	pid2 = fork();


	if (pid2 < 0) { 


	fprintf(stderr, "Fork Failed");

	exit(-1); 

	}

	/* child process */ 

	else if (pid2 == 0) { 



	//Process c
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

    *shm = '3';



    return 0;

 

	//execlp("/bin/ls", "ls", NULL);

	}

	

	// *shm = '1';

    //waiting pointer to first bit of memory equals 3


	
	//Process a

   	while( *shm != '3' )
    	sleep(1);



    printf("GoodBye\n");



    shmdt(shmid);
    shmdt(shmid2);



	

  

	printf ("Child Complete");
	
	printf("\n");

	return 0;

	exit(0);


	}


} 








