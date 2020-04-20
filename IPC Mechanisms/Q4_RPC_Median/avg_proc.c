//Heman Baral
//CMSC 312


#include <rpc/rpc.h>
#include "avg.h"
#include <stdio.h>

/* run locally on 'server' called by a remote client. */

static double sum_avg;

/* 
 * routine notice the _1 the version number 
 * notice the client handle, not sued here but needs to be 
 * a parameter 
 */
// double * average_1(input_data *input, CLIENT *client) 
//   {

//   /* input is paramters were marshalled by genrated routine */
//   /*  a pointer to a double, is set to begining of data array  */
//   double *dp = input->input_data.input_data_val;

//   u_int i;
//   sum_avg = 0;

//    iterate until end of number of times (data_len) 
//   for( i = 1; i <= input->input_data.input_data_len; i++ )
//     {
//     sum_avg = sum_avg + *dp;  /* add what ptrs points  to ( '*' gets content ) */
//     dp++;
//     }

//   sum_avg = sum_avg / input->input_data.input_data_len;

//   return( &sum_avg );
// }


static double num_median;


  void sort (double array[], int n) {

    int temp;

    int a, b;

    // the following two loops sort the array x in ascending order
    for(a=0; a<n-1; a++) {

        for(b=a+1; b<n; b++) {

            if(array[b] < array[a]) {

                // swap elements
                temp = array[a];

                array[a] = array[b];

                array[b] = temp;
            }
        }
    }

  }



double * median(input_data *input, CLIENT *client) 
  {

  /* input is paramters were marshalled by genrated routine */
  /*  a pointer to a double, is set to begining of data array  */
  double *dp = input->input_data.input_data_val;
  int dl = input->input_data.input_data_len;


  //put data length into array

   double array[input->input_data.input_data_len];




  u_int i;
  

  //sum_avg = 0;

  /* iterate until end of number of times (data_len) */
  for( i = 0; i <= input->input_data.input_data_len; i++ )
    {

    array[i] =  *dp; /* add what ptrs points  to ( '*' gets content ) */
    
    dp++;

    }


    //sorting array in order







    sort(array, dl);


    //median function
    //checking for even 

    if(dl%2 == 0){


    num_median = ((array[dl/2] + array[dl/2 - 1]) / 2.0);

    }

    //checking for odds
    else{

    num_median = array[dl/2];

    }

    return( &num_median );

}




/* 
 * server stub 'average_1_svc function handle called in avg_svc that was
 * generated by rpcgen 
 * FYI:
 *   result = (*local)((char *)&argument, rqstp);
 *   where local is (char *(*)(char *, struct svc_req *)) average_1_svc;
 */
 
double * average_1_svc(input_data *input, struct svc_req *svc) 
  {
  CLIENT *client;
  return( median( input, client) );
  }