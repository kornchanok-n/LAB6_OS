#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    //time measure
    struct timeval t1, t2;
    double elapsedTime;

    //buffer & socket
    char buffer[BUFFER_SIZE];
    char filename[100];
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;


    signal(SIGPIPE, SIG_IGN);

    long int filesize=0;
    // Getting File
    printf("Enter your filename: ");
    scanf("%s", filename);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
	    printf("File not found %s\n", filename);
	    perror("FILE CANNOT BE OPEN");
	    exit(1);
    }
    //seek file
    fseek(fp, 0, SEEK_END); // seek end
    filesize = ftell(fp); // obtain val
    fseek(fp, 0, SEEK_SET); // seek start


    int Filesize=filesize;

    //connect to server
    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        perror("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        perror("ERROR connecting");


    /*send data section start*/

    gettimeofday(&t1, NULL); //get start time
    sprintf(buffer, "%ld", filesize);
    n = write(sockfd, buffer, strlen(buffer)); //Send data
    n = read(sockfd,buffer,BUFFER_SIZE); //Wait response from server
    printf("\nSending data %d bytes... \n\n", Filesize);

    long int progress=0;
    long int sent=0;
    int i;

    while(fgets(buffer, sizeof(buffer), fp) != NULL){

        progress=sent*100/Filesize;

        /* //display progress
        printf("Working in progress %d percent...\n",progress);
        printf("Sending file %d/%d bytes...\n",sent,Filesize );
        printf("Status bar [");
        for(i=0;i<progress/5;i++) printf("*");
        for(i=0;i<20-progress/5;i++) printf(" ");
        printf("]\n\n");
        */

        sent+=strlen(buffer);

	n = write(sockfd, buffer, strlen(buffer)); //Send data
        if(n<0) perror("ERROR writing to socket");
        n = read(sockfd,buffer,BUFFER_SIZE); //Wait response from server
	bzero(buffer, BUFFER_SIZE);

    }
    
    /* //display progress
    printf("Working in progress 100 percent...\n",progress);
    printf("Sending file %d/%d bytes...\n",Filesize,Filesize );
    printf("Status bar [");
    for(i=0;i<20;i++) printf("*");
    printf("]\n\n");
    */
    printf("Sending Complete!!!\n\n");

    /*sending data section end*/

    //clear buff and close socket
    bzero(buffer, BUFFER_SIZE);
    gettimeofday(&t2, NULL); //get end time
    close(sockfd);
    fclose(fp);

    //calculate time
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000;
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;

    printf("Sending data in %.3lf ms.\n\n", elapsedTime);
    //printf("Client end.\n");

    return 0;
}
