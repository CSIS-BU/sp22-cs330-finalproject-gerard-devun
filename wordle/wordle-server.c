#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <strings.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#define MAX_PENDING 5 
#define MAX_SIZE 2048 
int 
main(int argc, char** argv) 
{ 
    FILE *fp;
    struct sockaddr_in sin;
    struct sockaddr_in cliAddr; 
    char buf[MAX_SIZE]; 
    int buf_len; 
    socklen_t addr_len; 
    int s, new_s; 
    char *port,*init_dir,*end_dir; 
    int streak=0;
    if(argc>1) { 
        fprintf(stderr, "usage: %s \n", argv[0]); 
        exit(1); 
    }
    port = "12000";
    /* build address data structure */ 
    memset(&sin,'0',sizeof(sin));
    bzero((char *)&sin, sizeof(sin)); 
    sin.sin_family = AF_INET; 
    sin.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    sin.sin_port = htons(atoi(port)); 
    /* setup passive open */ 
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {  perror("simplex-talk: socket"); 
        exit(1); 
    } 
    if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {  perror("simplex-talk: bind"); 
        exit(1); 
    } 
    listen(s, 10); 
    /* wait for connection, then receive and print text */  
    while(1) { 
        if ((new_s = accept(s, (struct sockaddr *)&cliAddr, &addr_len)) < 0) {  
            fprintf(stderr, "error: %s accept\n", argv[0]);
            exit(1); 
        }
        if(fork() == 0)
        {
            int numLetters;
            char wordToGuess[20];
            char returnString[20];
            recv(new_s, buf, sizeof(buf), 0);
            buf[strlen(buf)-1] = '\0';
            numLetters = atoi(buf);
            fprintf(stdout, "letters: %sL.txt \n", buf);
            //select word from word database
            //opens txt file based on number of letters wanted, can read or write file

            /*****GERARD***** shouldnt 3 be set to %s or numLetters below, so "WordDatabase/ "+%s+"L.txt" */

            fp=fopen("WordDatabase/3L.txt","r+");
            //if file isnt found, print an error and exit the program
            if(!fp){
                //printf(strcat("ERROR: Could not find ", buf, "letter database file."));
                fflush(stdout);
                close(new_s);
                break;
            }
            fgets(wordToGuess, 20, fp);
            fclose(fp);
            wordToGuess[strlen(wordToGuess)-1] = '\0';
            fprintf(stdout, "Word: %s... Waiting for Guess\n", wordToGuess);
            //Give player 6 guesses
            int numGuesses=0;
            for(int i=0;i<1;i++)
            {
                recv(new_s, buf, sizeof(buf), 0);
                buf[strlen(buf)-1] = '\0';
                for(int k=0;k<20;k++){
                    returnString[k] = '\0';
                    for(int j=0;j<numLetters;j++)
                    {   
                        //sees if letters are in right spot
                        if(buf[j] == wordToGuess[j])
                        {
                            returnString[j] = '*';
                        }
                        else
                        {
                         returnString[j] = '_';
                            //sees if letter is in word NOTE: Breaks after seeing first letter, doesnt check if letter is there twice+
                            for(int l=0;l<numLetters;l++)
                            {
                                if(buf[j]==wordToGuess[l])
                                {
                                    returnString[j] = '~';
                                    break;
                                }
                            }
                        }
                    }
                    fprintf(stdout, "Result: %s\n", returnString);
                    //write(new_s,returnString,strlen(returnString));
                }
            }
            while ( (buf_len = recv(new_s, buf, sizeof(buf), 0)) ) {  
                fwrite(buf, 1, buf_len, stdout);
            }
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
