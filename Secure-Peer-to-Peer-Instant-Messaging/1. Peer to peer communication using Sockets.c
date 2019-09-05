#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

sem_t sem1;
char shared_msg[15];

void * thread1(void * arg){

	int soc1 = *(int*)arg;
    	int n;
    	char clmsg[15];
      int sizemsg=strlen(clmsg);

      if((n=recv(soc1,shared_msg,15,0)) > 0)
      {

	   sem_wait(&sem1);
	   write( soc1 , shared_msg , sizemsg);
	   sleep(2);
         sem_post(&sem1);
         send(soc1, shared_msg, n, 0);
	   close(soc1);
      }
}
void * thread2(void * arg){


	int soc1 = *(int*)arg;
    	int n;

      char clmsg[15];
      int sizemsg=strlen(clmsg);

      if((n=recv(soc1,clmsg,15,0)) > 0)
      {

	   sem_wait(&sem1);
	   write( soc1 , clmsg , sizemsg);
	   sleep(2);
         sem_post(&sem1);
         send(soc1, clmsg, n, 0);
	   close(soc1);
      }
}
void * thread3(void * arg){


	int soc1 = *(int*)arg;
    	int n;

      char clmsg[15];
      int sizemsg=strlen(clmsg);

      if((n=recv(soc1,clmsg,15,0)) > 0)
      {

	   sem_wait(&sem1);
	   write( soc1 , clmsg , sizemsg);
	   sleep(2);
         sem_post(&sem1);
         send(soc1, clmsg, n, 0);
	   close(soc1);
      }
}


int main(void)
{
    int serversocfd ;
    int clientsocfd ;
    int client_size,true=1;
    struct sockaddr_in client;
    struct sockaddr_in server;
    int  *soc2Arg;
    pthread_t tid1[1];
    pthread_t tid2[1];
    pthread_t tid3[1];
    pthread_attr_t	attr[1];
    pthread_attr_init(&attr[0]);
    pthread_attr_setscope(&attr[0], PTHREAD_SCOPE_SYSTEM);
    serversocfd = socket(AF_INET , SOCK_STREAM , 0);
    printf("Socket created\n");
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("131.247.3.8");
    server.sin_port = htons( 1050 );
    if(setsockopt(serversocfd,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1)
    {
     	printf("setsockopt:error");
      exit(1);
    }
    if( bind(serversocfd, (struct sockaddr*)&server , sizeof(server)) < 0){

	printf("\nError in binding");
        exit(1);
    }
    printf("bind complete");

    listen(serversocfd , 3);
    int clientsize = sizeof(struct sockaddr_in);

    if( (clientsocfd = accept(serversocfd, (struct sockaddr *)&client, (socklen_t*)&clientsize)) ){

         soc2Arg = malloc(1);
        *soc2Arg = clientsocfd;
         pthread_create( &tid1[0], &attr[0] ,  thread1, (void*)soc2Arg);
	   pthread_create( &tid2[0], &attr[0] ,  thread2, (void*)soc2Arg);
         pthread_create( &tid3[0], &attr[0] ,  thread3, (void*)soc2Arg);
    }
    pthread_join(tid1[0], NULL);
    pthread_join(tid2[0], NULL);
    pthread_join(tid3[0], NULL);
    close(serversocfd);
    return 0;
}
