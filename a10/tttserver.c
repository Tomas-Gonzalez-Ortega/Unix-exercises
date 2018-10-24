#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define LISTEN_Q 6

#include "tttmodel.h"
#include "logger.h"

// signal handler to reap children to prevent zombies
void SIGCHLD_handler(int signum);
// makes a server socket with a random port #
int makeServerSocket();
// two players play a game of TTT over the sockets
void playgame(int player1, int player2);
// the given player plays their move
void playmove(TTT* model, char player, int sock);
// write the game state to both players
int ttt_write2(const TTT* model, int sock1, int sock2);
// write the game state to one player
int ttt_write1(const TTT* model, int sock);

/** Outputs with error message */
void fatalError(char *msg) {
    perror(msg);
    exit(1);
}

/** error message with client socket closed */
void minorError(char *msg, int sock) {
    perror(msg);
    close(sock);
}

/** updating the interface in the buffer */
void setInterface(char *buf, char *tab) {
    sprintf(buf,"\n\t\t %c | %c | %c \n"
            "\t\t-----------\n"
            "\t\t %c | %c | %c \n"
            "\t\t-----------\n"
            "\t\t %c | %c | %c \n\n\n",
            tab[0],tab[1],tab[2],tab[3],tab[4],tab[5],tab[6],tab[7],tab[8]);
}

/** updates the game after each new move */
int setTable(char player,char position,char *tab, int tabLen) {
    int i;
    int free = 0;
    
    if(player != '0') {
        for(i = 0; i < tabLen; i++) {
            if(position == tab[i]) { // check if the box is not occupied
                tab[i] = player;
                free = 1;
                break;
            }
        }
        if(free == 0) {
            return (-1); // occupied
        }
    }
    else { // new match
        strcpy(tab,"123456789");
    }
    return (0);
}

enum { None, Server, Player, Tie };

/** checks for a winner */
int test_won(char *tab) {
    int i,flag = 0;
    if ((tab[0] == 'x' && tab[1] == 'x' && tab[2] == 'x') ||
        (tab[3] == 'x' && tab[4] == 'x' && tab[5] == 'x') ||
        (tab[6] == 'x' && tab[7] == 'x' && tab[8] == 'x') ||
        (tab[0] == 'x' && tab[3] == 'x' && tab[6] == 'x') ||
        (tab[1] == 'x' && tab[4] == 'x' && tab[7] == 'x') ||
        (tab[2] == 'x' && tab[5] == 'x' && tab[8] == 'x') ||
        (tab[0] == 'x' && tab[4] == 'x' && tab[8] == 'x') ||
        (tab[2] == 'x' && tab[4] == 'x' && tab[6] == 'x')) {
        return Server;
    }
    if ((tab[0] == 'o' && tab[1] == 'o' && tab[2] == 'o') ||
        (tab[3] == 'o' && tab[4] == 'o' && tab[5] == 'o') ||
        (tab[6] == 'o' && tab[7] == 'o' && tab[8] == 'o') ||
        (tab[0] == 'o' && tab[3] == 'o' && tab[6] == 'o') ||
        (tab[1] == 'o' && tab[4] == 'o' && tab[7] == 'o') ||
        (tab[2] == 'o' && tab[5] == 'o' && tab[8] == 'o') ||
        (tab[0] == 'o' && tab[4] == 'o' && tab[8] == 'o') ||
        (tab[2] == 'o' && tab[4] == 'o' && tab[6] == 'o')) {
        return Player;
    }
    // draw?
    for(i = 0; i < 9; i++) {
        if((tab[i] != 'x') && (tab[i] != 'o')) {
            flag=1;
        }
    }
    if(!flag) {
        return Tie;
    }
    return None; // the match continues
}

/** sets a message to the customer */
void setMesg(int w, char *buf, int *d) {
    if(w == 0) {
        strcat(buf,"You are the player 'o' and me the player 'x'.\n");
        strcat(buf,"To win, line up 3 pawns on a line.\n ");
        strcat(buf,"Choose a position between 1 and 9 for"
               " your next pawn -->  ");
    }
    else if(w == -1) {
        strcat(buf, "This box is already occupied; \n\tplease make another choice --> ");
    }
    else {
        *d = 1;
        if(w == Player) {
            strcat(buf,"\tYOU WIN !\n");
        }
        else if(w == Server) {
            strcat(buf,"\tSERVEUR WINS !\n");
        }
        else if(w == Tie) {
            strcat(buf,"TIE !\n");
        }
        strcat(buf,"Do you want to continue (o/n) ? --> ");
    }
}

/** the server game
Assuming that test_won() has verified that there is no winner yet */
char serverPlay(char *tab, int tabLen) {
    int i, position, free = 0;
    char choice;
    
    // try to finish
    for(i = 0; i <= 6; i++) {
        switch(i) {
            case 0:// try lines and diagonals
                if((tab[0] == tab [4]) && (tab[0] == 'x') &&
                   (tab[8] != 'o')) {
                    return tab[8];
                }
                else if((tab[0] == tab [8]) && (tab[0] == 'x') &&
                        (tab[4] != 'o')) {
                    return tab[4];
                }
                else if((tab[4] == tab [8]) && (tab[4] == 'x') &&
                        (tab[0] != 'o')) {
                    return tab[0];
                }
                // try the columns
                if((tab[i] == tab [i+3]) && (tab[i] == 'x') &&
                   (tab[i+6] != 'o')) {
                    return tab[i+6];
                }
                else if((tab[i] == tab [i+6]) && (tab[i] == 'x') &&
                        (tab[i+3] != 'o')) {
                    return tab[i+3];
                }
                else if((tab[i+3] == tab [i+6]) && (tab[i+3] == 'x') &&
                        (tab[i] != 'o')) {
                    return tab[i];
                }
            case 3:
            case 6:
                if((tab[i] == tab [i+1]) && (tab[i] == 'x') &&
                   (tab[i+2] != 'o')) {
                    return tab[i+2];
                }
                else if((tab[i] == tab [i+2]) && (tab[i] == 'x') &&
                        (tab[i+1] != 'o')) {
                   return tab[i+1];
                }
                else if((tab[i+1] == tab [i+2]) && (tab[i+1] == 'x') &&
                        (tab[i] != 'o')) {
                    return tab[i];
                }
                break;
                // try the columns
            case 2:
                // try the right diagonal
                if((tab[2] == tab [4]) && (tab[2] == 'x') &&
                   (tab[6] != 'o')) {
                    return tab[6];
                }
                else if((tab[2] == tab [6]) && (tab[2] == 'x') &&
                        (tab[4] != 'o')) {
                    return tab[4];
                }
                else if((tab[4] == tab [6]) && (tab[4] == 'x') &&
                        (tab[2] != 'o')) {
                    return tab[2];
                }
            case 1:if((tab[i] == tab [i+3]) && (tab[i] == 'x') &&
                      (tab[i+6] != 'o')) {
                return tab[i+6];
            }
            else if((tab[i] == tab [i+6]) && (tab[i] == 'x') &&
                    (tab[i+3] != 'o')) {
                return tab[i+3];
            }
            else if((tab[i+3] == tab [i+6]) && (tab[i+3] == 'x') &&
                    (tab[i] != 'o')) {
                return tab[i];
            }
                break;
            default : break;
        }
    }
    
    // otherwise, try to block the opponent
    for(i = 0; i <= 6; i++) {
        switch(i) {
            case 0: // try also the left diagonal
                if((tab[0] == tab [4]) && (tab[0] == 'o') &&
                   (tab[8] != 'x')) {
                    return tab[8];
                }
                else if((tab[0] == tab [8]) && (tab[0] == 'o') &&
                        (tab[4] != 'x')) {
                    return tab[4];
                }
                else if((tab[4] == tab [8]) && (tab[4] == 'o') &&
                        (tab[0] != 'x')) {
                    return tab[0];
                }
                // also try the column
                if((tab[i] == tab [i+3]) && (tab[i] == 'o') &&
                   (tab[i+6] != 'x')) {
                    return tab[i+6];
                }
                else if((tab[i] == tab [i+6]) && (tab[i] == 'o') &&
                        (tab[i+3] != 'x')) {
                    return tab[i+3];
                }
                else if((tab[i+3] == tab [i+6]) && (tab[i+3] == 'o') &&
                        (tab[i] != 'x')) {
                    return tab[i];
                }
            case 3:
            case 6:
                if((tab[i] == tab [i+1]) && (tab[i] == 'o') &&
                   (tab[i+2] != 'x')) {
                    return tab[i+2];
                }
                else if((tab[i] == tab [i+2]) && (tab[i] == 'o') &&
                        (tab[i+1] != 'x')) {
                    return tab[i+1];
                }
                else if((tab[i+1] == tab [i+2]) && (tab[i+1] == 'o') &&
                        (tab[i] != 'x')) {
                    return tab[i];
                }
                break;
                // try the columns
            case 2:
                // try also the right diagonal
                if((tab[2] == tab [4]) && (tab[2] == 'o') &&
                   (tab[6] != 'x')) {
                    return tab[6];
                }
                else if((tab[2] == tab [6]) && (tab[2] == 'o') &&
                        (tab[4] != 'x')) {
                    return tab[4];
                }
                else if((tab[4] == tab [6]) && (tab[4] == 'o') &&
                        (tab[2] != 'x')) {
                    return tab[2];
                }
            case 1:
                if((tab[i] == tab [i+3]) && (tab[i] == 'o') &&
                   (tab[i+6] != 'x')) {
                    return tab[i+6];
                }
                else if((tab[i] == tab [i+6]) && (tab[i] == 'o') &&
                        (tab[i+3] != 'x')) {
                    return tab[i+3];
                }
                else if((tab[i+3] == tab [i+6]) && (tab[i+3] == 'o') &&
                        (tab[i] != 'x')) {
                    return tab[i];
                }
                break;
            default : break;
        }
    }
    
    // Otherwise, randomly select one of the empty spaces
    for(i = 0; i < tabLen; i++) {
        if((tab[i] != 'o') && (tab[i] != 'x')) {
            free++;
        }
    }
    position = 1 + rand () % free;
    free = 0;
    for(i = 0; i < tabLen; i++) {
        if((tab[i] != 'o') && (tab[i] != 'x')) {
            free++;
            if(free == position) {
                choice = tab[i];
            }
        }
    }
    return choice;
}

int main() {
    char c;
    int decision ;
    int winner = 0;
    char table[9];
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    // random initialization with time in seconds
    srand(time(0));
    
    if(argc != 2) {
        fprintf(stderr,"Error, usage: \"%s <port>\"", argv[0]);
        fprintf(stderr,"\n\tTHE PROGRAM WILL TERMINATE !\n\n");
        exit(1);
    }
    
    portno = atoi(argv[1]);
    
    /** Create a socket stream */
    if((sockfd = socket(AF_INET,SOCK_STREAM, 0)) < 0) {
        fatalError("Socket opening error");
    }
    
    // initialisation
    memset((char *) &serv_addr,0, sizeof(serv_addr));
    
    /** bind an address */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        fatalError("Error on bind())");
    }
    
    /** specifies a maximum number of client requests */
    listen(sockfd,5);
    
    /** server ready to accept multiple connections */
    for(;;) { // maintains the process when the client finishes
        
        fprintf(stderr,"Server ready, waiting on the port [%i]\n", portno);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd,
                           (struct sockaddr *) &cli_addr,
                           &clilen);
        if(newsockfd < 0) {
            perror("Error on accept())");
            continue; // get out of the loop
        }
        
        /** creates a process to support the connection */
        if(fork() == 0) { // child process
            close(sockfd);
            
            // set a match table
            setTable('0','0', table, 9);
            decision = 0; // play again?
            
            do { // as long as the client wants to continue
                setInterface(buffer,table);
                if(winner != -1) {
                    winner = test_won(table); // box chosen was free
                }
                setMesg(winner, buffer, &decision);
                if((n = write(newsockfd,buffer,256)) > 0) {
                    memset(buffer,0,256);
                    if((n = read(newsockfd,buffer,255)) < 0) {
                        minorError("Problem of reading on the socket", newsockfd);
                        break;
                    }
                    if(decision == 0) {
                        if(0 == setTable('o',buffer[0],table,9)) {
                            if((winner = test_won(table)) == 0) {
                                c = serverPlay(table,9); // server plays
                                setTable('x',c,table,9);
                            }
                        }
                        else {
                            winner = -1; // box already occupied
                        }
                    }
                    else { // end of match
                        if(buffer[0] == 'o') {
                            setTable('0','0', table, 9);
                            decision = 0;
                        }
                        else {
                            break;// the client does not want to play anymore
                        }
                    }
                }
                else if(n < 0) {
                    minorError("Problem of writing on the socket\n", newsockfd);
                    break;
                }
            } while(1);
            // the server closes the connection which will free/end the client process from its listening loop
            close(newsockfd); /
            exit(0);
        }
        else { //error case (fork () == -1) and parent process case (fork() > 0)
            close(newsockfd); // release the client socket
        }
    }
    return 0;
}
