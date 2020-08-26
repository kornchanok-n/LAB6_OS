/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
   gcc server2.c -lsocket
*/
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define BUFFER_SIZE 1024


void dostuff(int); /* function prototype */
void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen, pid;
     struct sockaddr_in serv_addr, cli_addr;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     while (1) {
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         pid = fork();
         if (pid < 0)
             error("ERROR on fork");
         if (pid == 0)  {
             close(sockfd);
             dostuff(newsockfd);
             exit(0);
         }
         else close(newsockfd);
     } /* end of while */
     return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/

//Before Modify
/*
void dostuff (int sock)
{
   int n;
   char buffer[1500];
   
   bzero(buffer,1500);
   n = read(sock,buffer,1499);
   if (n < 0) error("ERROR reading from socket");
   printf("Here is the message: %s\n",buffer);
   n = write(sock,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");

}
*/


//After Modify

void dostuff (int sock)
 {
     int n;
     long int word_size;
     char buffer[BUFFER_SIZE];

     // read data
     n = read(sock, buffer, BUFFER_SIZE-1); //Get data to buffer
     n = write(sock,"Successfully Get Your Data !!!",30); //Send response to client

     word_size = atoi(buffer);
     printf("Loading data %ld bytes.\n\n", word_size);
     bzero(buffer,BUFFER_SIZE);

     // allocate
     char *recv;
     recv = (char*)malloc(word_size*sizeof(char));
     long int byte=0;
     int size_to_cpy=0;
     int remainder = word_size%(BUFFER_SIZE-1);
     long int progress= 0;
     int i;

     /*Getting data section*/

     while(byte<word_size){

         progress= byte*100/word_size;
         
         /* //display progress
         printf("Working in progress %ld percent...\n",progress);
         printf("Loading data %ld/%ld bytes... \n",byte,word_size);
         printf("Status bar [");
         for(i=0;i<(progress/5);i++) printf("*");
         for(i=0;i<20-(progress/5);i++) printf(" ");
         printf("]\n\n");
         */
         

         n = read(sock, buffer, BUFFER_SIZE-1); //Get data to buffer
         if (n < 0) error("ERROR reading from socket");
         n = write(sock,"Successfully Get Your Data !!!",30); //Send response to client

         if(BUFFER_SIZE >= word_size-byte)
             size_to_cpy=remainder;
         else size_to_cpy=BUFFER_SIZE-1;
         memcpy(&recv[byte], buffer, strlen(buffer));
         byte += strlen(buffer);
         bzero(buffer, BUFFER_SIZE);

     }
    /*  //display progress
     printf("Working in progress 100 percent...\n");
     printf("Loading data %ld/%ld bytes... \n",word_size,word_size);
     printf("Status bar [");
     for(i=0;i<20;i++) printf("*");
     printf("]\n\n");
     */
     

     printf("Loading complete!!! \n\n");
     //printf("Your data here :\n\n %s", recv); //display data

     free(recv);
 }
