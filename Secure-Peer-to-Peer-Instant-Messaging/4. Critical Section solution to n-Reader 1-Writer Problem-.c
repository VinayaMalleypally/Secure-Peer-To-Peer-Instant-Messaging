#define _REENTRANT
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
/*varible to store number of readers */
int readerscount = 0;
/*two semaphore varibles declaration */
sem_t writer_lock,readerscount_lock;
/*shared flag to print the error message in the reader thread */
int in_cs = 0;

struct shared_dat
   {
   int value; /*shared variable */
   };

 struct shared_dat *counter;

/*function to prolong execution time */
 void relaxandspendtime(){
        int i;
        for(i=0;i<250000000;i++)
{
 i=i;
}
 }
/*writer thread */
void *thread_writer(void *arg){
        int w=0;
        sem_wait(&writer_lock);

        in_cs=1;
        relaxandspendtime();
        counter->value++;
 /* updates the value 25000 time */
  for(w=0;w<25000;w++){
  counter->value = counter->value;
        }
        /* reasets the shared flag */
        in_cs--;

printf("writer done and the value is %d \n",counter->value);

sem_post(&writer_lock);

}


void * thread_reader(void *arg){
        /* check the flag for printing the message */
        if(in_cs==1){

                printf(" writer is busy in the critical section \n");
        }

        sem_wait(&readerscount_lock);
        readerscount++;
        if(readerscount==1){
                sem_wait(&writer_lock);
   sem_post(&readerscount_lock);
        }
 else{
        sem_post(&readerscount_lock);
 }


        printf("from readernumber: %d with tid: %d the  valueofcounter : %d  \n",(int *)arg,syscall(SYS_gettid),counter->value);
        sem_wait(&readerscount_lock);
        readerscount--;
        if(readerscount==0){
                sem_post(&writer_lock);
        sem_post(&readerscount_lock);
        }
 else{
        sem_post(&readerscount_lock);
 }


relaxandspendtime();
}

int main(int argc,char *argv[]){
/* read the number of readers from commandline arguments*/
        int numOfReaders = atoi(argv[1]);
 /*initialize the semophore variables */
        sem_init(&writer_lock,0,1);
        sem_init(&readerscount_lock,0,1);

        pthread_t readers[14];
        pthread_t writer;
        pthread_attr_t attr;

        counter = (struct shared_dat *) malloc(sizeof(struct shared_dat));
        /* initializing the shared variable */
        counter-> value =0;
        pthread_attr_init(&attr);
        pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

        if(numOfReaders<1 || numOfReaders>14){
                printf("\n please give number of readers value from 1 to 14");
                exit(0);
        }
        int k,i;
         k = (int) (numOfReaders/2);
  /*creating the threads*/
         for(i = 0; i < k; i++){
                pthread_create(&readers[i], &attr,&thread_reader,(void*) i);
                }
                pthread_create(&writer, &attr,&thread_writer, NULL);
        for(i = k ; i < numOfReaders ; i++) {
                pthread_create(&readers[i], &attr,&thread_reader,(void*) i);
        }

 /*parent thread waiting for all the threads to complete execution */
 for(i=0;i<numOfReaders;i++)
{
pthread_join(readers[i],NULL);
}
pthread_join(writer,NULL);


exit(0);

}
