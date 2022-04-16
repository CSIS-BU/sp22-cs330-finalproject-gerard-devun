#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <string.h> 
#include <unistd.h> 
#include <stdlib.h> 
#define MAX_SIZE 2048 
int 
main(int argc, char * argv[]) 
{ 
    FILE *fp; 
    struct hostent *hp; 
    struct sockaddr_in sin; 
    char *host, *port; 
    char buf[MAX_SIZE]; 
    int s; 
    int len; 

    //holds number of letters in guessing word
    int numLetters;

    if (argc==3) { 
        host = argv[1]; 
        port = argv[2]; 
    } 
    else { 
        fprintf(stderr, "usage: %s host port\n", argv[0]);  exit(1); 
    } 
    /* translate host name into peer's IP address */  
    hp = gethostbyname(host); 
    if (!hp) { 
        fprintf(stderr, "%s: unknown host\n", host);  exit(1); 
    } 
    /* build address data structure */ 
    bzero((char *)&sin, sizeof(sin)); 
    sin.sin_family = AF_INET; 
    bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);  sin.sin_port = htons(atoi(port)); 
    /* active open */ 
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {  perror("simplex-talk: socket"); 
        exit(1); 
    } 
     if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)  {
        perror("simplex-talk: connect");  close(s); 
        exit(1); 
    } 

    //ask player to input number of letters in word
    printf("Enter number of letters in guessing word (3-10): ");
    //checks if user inputs one number, and if the number falls between 3 and 10, if not ask to input again
    while(scanf("%i", numLetters)!=1 && (numLetters>3 || numLetters<10)){
        //tell user input was invalid
        printf("You did not input a valid number, try again: ");
        scanf("%*s");
    }
    //opens txt file based on number of letters wanted, can read or write file
    *fp=fopen(numLetters+"L.txt","r+");
    if(!fp){
        printf("ERROR: Could not find "+ numLetters + "letter database file.");
    }
    
    int size; 
    while( (size=fread(buf, 1, MAX_SIZE, stdin) )) {  
        if(send(s, buf, size, 0) < 0) { 
            perror("client: send"); 
        } 
    } 
 // Done, close the s socket descriptor  close(s); 
}

