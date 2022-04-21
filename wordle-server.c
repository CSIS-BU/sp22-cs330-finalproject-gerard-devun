#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <strings.h> 
#include <stdlib.h> 
#include <unistd.h> 
#define MAX_PENDING 5 
#define MAX_SIZE 2048 

//add scoreboard

int main(int argc, char** argv) { 
    struct sockaddr_in sin; 
    char buf[MAX_SIZE]; 
    int buf_len; 
    socklen_t addr_len; 
    int s, new_s; 
    char *port; 
    if(argc==2) { 
        port = argv[1]; 
    } 
    else { 
        fprintf(stderr, "usage: %s port\n", argv[0]); 
        exit(1); 
    } 
    /* build address data structure */ 
    bzero((char *)&sin, sizeof(sin)); 
    sin.sin_family = AF_INET; 
    sin.sin_addr.s_addr = INADDR_ANY; 
    sin.sin_port = htons(atoi(port)); 
    /* setup passive open */ 
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {  perror("simplex-talk: socket"); 
        exit(1); 
    } 
    if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {  perror("simplex-talk: bind"); 
        exit(1); 
    } 
    listen(s, MAX_PENDING); 
    /* wait for connection, then receive and print text */  
    while(1) { 
        if ((new_s = accept(s, (struct sockaddr *)&cliAddr, &addr_len)) < 0) {  
            fprintf(stderr, "error: %s accept\n", argv[0]);
            exit(1); 
        }
        //Forks the process into a parent and child process. A child process is created for every connected client.
        if(fork() == 0)
        {
            //receive amount of letters from client
            while ( (buf_len = recv(new_s, buf, sizeof(buf), 0)) ) {  
                fwrite(buf, 1, buf_len, stdout); 
            }
            File *fp;
            int numLetters = atoi(buf); //need to remove \n from string first
            //select word from word database
            //opens txt file based on number of letters wanted, can read or write file
            fp=fopen(""+ numLetters +"L.txt","r+");
            //if file isnt found, print an error and exit the program
            if(!fp){
                printf("ERROR: Could not find "+ numLetters + "letter database file.");
                exit(1);
            }
            //allow for 6 guesses
            for(int i=0; i<6;i++)
            {
                //receive guess and then give feedback to client
            }
            //win and loss conditions
            fflush(stdout);
            close(new_s);
            break;
        }
        else
        {
            close(new_s);
            continue;
        }
    } 
    close(s); 
    return 0; 
}
