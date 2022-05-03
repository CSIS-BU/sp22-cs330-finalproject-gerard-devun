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
    struct hostent *hp; 
    struct sockaddr_in sin; 
    char *host, *port; 
    char buf[MAX_SIZE]; 
    int s; 
    int len; 

    //holds number of letters in guessing word
    int numLetters,correctLetters;

    if (argc==3) { 
        host = argv[1]; 
        port = argv[2]; 
    } 
    else { 
        fprintf(stderr, "usage: %s host port\n", argv[0]); 
        exit(1); 
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
    printf("Welcome to Wordle!!\nHow to play:\nGuess a word, the server will return \033[30;102mgreen\033[0m if letter is in the correct spot,\n\033[30;103myellow\033[0m if the letter is in the word but wrong location,\nand no color if the letter is not in the word.\n");
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

    numLetters = atoi(buf);
    char guessedWord[20];

    while(1)
    {
        for(int i=0;i<6;i++)
        {
            printf("Enter Your Guess: ");
            while(fgets(buf, MAX_SIZE, stdin) )
            {
                buf[strlen(buf)-1] = '\0';
                if(strlen(buf)<numLetters||strlen(buf)>numLetters)
                {
                    printf("Your guess is invalid, try again: ");
                    continue;
                }
                strcpy(guessedWord,buf);
                buf[strlen(buf)] = '\n';
                if(send(s, buf, strlen(buf), 0) < 0) { 
                    perror("client: send"); 
                }
                break;
            }

            recv(s, buf, sizeof(buf), 0);
            buf[strlen(buf)-1] = '\0';
            fprintf(stdout, "\033[A\33[2KT\rFeedback Results: ");
            correctLetters = 0;
            for(int j=0;j<numLetters;j++)
            {
                if(buf[j]=='*')
                {
                    printf("\033[33;42m");
                    fprintf(stdout, "\033[30;102m%c\033[0m",guessedWord[j]);
                    correctLetters++;
                }
                else if(buf[j]=='~')
                {
                    fprintf(stdout, "\033[30;103m%c\033[0m",guessedWord[j]);
                }
                else
                {
                    fprintf(stdout, "%c",guessedWord[j]);
                }
            }
            printf("\n");
            if(correctLetters >= numLetters)
            {
                break;
            }
            
        }
        if(send(s, buf, strlen(buf), 0) < 0) { 
            perror("client: send"); 
        }
        fflush(stdin);
        for(int i=MAX_SIZE-1;i>=0;i--)
            buf[i] = '\0';
        recv(s, buf, sizeof(buf), 0);
        fprintf(stdout, "%s", buf);
        printf("Play Again? (Y/N): ");
        while(fgets(buf, MAX_SIZE, stdin) )
        {
            if(buf[0] == 'y' || buf[0] == 'Y')
            {
                printf("Restarting...\n");
            }
            else if(buf[0] == 'n' || buf[0] == 'N')
            {
                printf("Thanks for Playing!\n");
            }
            else
            {
                printf("Invalid Input, Play Again? (Y/N): ");
                continue;
            }
            if(send(s, buf, strlen(buf), 0) < 0) { 
                perror("client: send"); 
            }
            if(buf[0] == 'n' || buf[0] == 'N')
            {
                close(s);
                return 0;
            }
            break;
        }
        fflush(stdout);
    }
}
