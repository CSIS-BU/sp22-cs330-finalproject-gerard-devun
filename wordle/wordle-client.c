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
    /*
    //boolean for if the word is guessed correctly
    bool correctWord=false;
    //word to guess
    char wordToGuess[];
    //guessed word
    char userWord[];
    //number of attempted guesses
    int guessAttempts=0;
    //letters guessed correctly, size of the wordToGuess
    char correctLetters[];
    //list of letters not in word
    char incorrectLetters[];
    //list of letters guessed that are in word
    char lettersInWord[];
    //letters in word but wrong spot, size of the wordToGuess
    char wrongSpotLetters[];*/
    /* add a username and a scorestreak that tracks how many words correct in a row for a word length */
    host = "127.0.0.1"; 
    port = "12000";
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
    printf("Welcome to Wordle!!\nHow to play:\nGuess a word, the server will return a * if letter is in the correct spot,\na ~ if the letter is in the word but wrong location,\nand a _ if the letter is not in the word.\n");
    printf("First, Enter number of letters in guessing word (3-10): ");

    //checks input for valid number
    while(fgets(buf, MAX_SIZE, stdin) ) {  
        if(atoi(buf)<3||atoi(buf)>10)
        {
            printf("You did not input a valid number, try again: ");
            continue;
        }
        if(send(s, buf, strlen(buf), 0) < 0) { 
            perror("client: send"); 
        }
        if(buf[strlen(buf)-1]=='\n')
            break;
    }



    //A LOT OF BELOW IS DONE ON SERVER INSTEAD

    numLetters = atoi(buf);

    while(fgets(buf, MAX_SIZE, stdin) ) {
        buf[strlen(buf)-1] = '\0';
        if(strlen(buf)<numLetters||strlen(buf)>numLetters)
        {
            printf("Your guess is invalid, try again: ");
            continue;
        }
        buf[strlen(buf)] = '\n';
        if(send(s, buf, strlen(buf), 0) < 0) { 
            perror("client: send"); 
        }
        break;
    }

  
    int size; 
    while(fgets(buf, MAX_SIZE, stdin) ) {  
        if(send(s, buf, strlen(buf), 0) < 0) { 
            perror("client: send"); 
        }
        if(buf[strlen(buf)-1]=='\n')
            break;
    }
    
    while(fgets(buf, MAX_SIZE, stdin) ) {  
        if(send(s, buf, strlen(buf), 0) < 0) { 
            perror("client: send"); 
        }
        if(buf[strlen(buf)-1]=='\n')
            break;
    }
    close(s);
}

