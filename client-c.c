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
    char wrongSpotLetters[];
    /* add a username and a scorestreak that tracks how many words correct in a row for a word length */


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

    /* sets correctLetters and wrongSpotLetters string to length of word and fill with astrics, will replace astrics with letters guessed correctly below */
    for(int i=0;i<=numLetters;i++){
        correctLetters[i]="*";
        wrongSpotLetters[i]="*";
    }

    /* sets the word to guess to the first string found in file*/
    /*DELETE COMMENT BEFORE FINAL TURN IN **DOUBLE CHECK IF +1 IS NECESSARY** fgets reads a line from the specified stream and stores it into the string pointed to by wordToGuess. 
    It stops when either (n-1) (numLetters+1 -1) characters are read, the newline character is read, or the end-of-file is reached, 
    whichever comes first. via TutorialsPoint */
    fgets(*wordToGuess,numLetters+1,*fp);
    
    /* player guessing the word */
    while(correctWord==false && guessAttempts!=6){
        printf("Enter your guess: ");
        scanf("%s",userWord); 
        if(userWord.length()!=numLetters){
            printf("Incorrect word length entered.");
        }
        //if want to see if its a valid word: found database txt file https://raw.githubusercontent.com/dwyl/english-words/master/words.txt
        /* checking if letters are in word */
        else if(){
            for(int i=0;i<=numLetters;i++){
                //check if letters are in the correct place, if they are: add to correctLetters[] and lettersInWord[]
                if(userWord[i]==wordToGuess[i]){
                    correctLetters[i]=userWord[i];
                    strncat(lettersInWord,&userWord[i],1);
                    strncat(lettersInWord,", ",1);
                }
                //check if letters are in word, if they are add to lettersInWord[], checks if letter is already in list first
                if(strchr(*lettersInWord,userWord[i])==NULL){
                    if(strchr(*wordToGuess,userWord[i])){
                        strncat(lettersInWord,&userWord[i],1);
                        strncat(lettersInWord,", ",1);
                    }
                }
               
                /** might need seperate for loops for each of these **/

                //check if letters are in lettersInWord but NOT in correctLetters[], if so add to wrongSpotLetters[]
                if(userWord[i])
                //
                //if letters are not in word add to incorrectLetters[]
            }
        }   

    }

    int size; 
    while( (size=fread(buf, 1, MAX_SIZE, stdin) )) {  
        if(send(s, buf, size, 0) < 0) { 
            perror("client: send"); 
        } 
    } 
 // Done, close the s socket descriptor  close(s); 
    close(s);
}

