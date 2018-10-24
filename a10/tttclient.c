#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tttmodel.h"

#define DEFAULT_HOST "localhost"

// connect to the given host:port
int makeConnection(char* host, char* port);
// begins playing the game over the given socket as the given player
void playgame(const char me, int sock);
// reads the game state from the socket
void ttt_read(TTT* model, int sock);
// displays the game state to console
void display(const TTT* model);
// determines if it's my turn to move
int ismyturn(const TTT* model, const char me);
// gets a move from the user via the console
int getmove(const TTT* model);
// sends a move to the server
void sendmove(int sock, int move);

// error message then program exit
void fatalError(char *msg) {
    perror(msg);
    perror("\n\tTHE PROGRAM WILL TERMINATE !\n\n");
    exit(1);
}

int main(int argc, char* argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    
    if(argc < 3) {
        fprintf(stderr,"\nERROR, usage: \"%s <hostname> <port>\"", argv[0]);
        fprintf(stderr,"\n\tTHE PROGRAM WILL TERMINATE !\n\n");
        exit(1);
    }
    
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(sockfd < 0) {
        fatalError("Error while opening socket");
    }
    
    server = gethostbyname(argv[1]);
    
    if (server == NULL) {
        fatalError("Error, hostname incorrect !");
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) {
        fatalError("Error while connecting");
    }
    
    // as long as you can read on the socket
    while(n = read(sockfd,buffer,256)) {
        printf("%s",buffer);
        memset(buffer,0,256);
        
        // reading input from keyboard
        fgets(buffer,256,stdin);
        
        // writing the socket
        if((n = write(sockfd,buffer,strlen(buffer))) < 0) {
            fatalError ("Error while writing on the socket");
        }
        memset(buffer,0,256);
    }
    // closing the stream
    close(sockfd);
    
    return 0;
}
