 
 
 
/* --------------------    LE CLIENT   -------------------------------*/ 
 
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h> 
 
//message d'erreur puis sortie de programme
void fatalError(char *msg) {
   perror(msg); // idem >> fprintf(stderr,msg);
   perror("\n\tTHE PROGRAM WILL TERMINATE !\n\n");
   exit(1); 
 } 

/*________________main()__________________________*/

int main(int argc, char *argv[]) {
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[256];
     
  if (argc < 3) {
    fprintf(stderr,"\nERROR, usage: \"%s <hostname> <port>\"", argv[0]);
    fprintf(stderr,"\n\tTHE PROGRAM WILL TERMINATE !\n\n");
    exit(1);
  }
     
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
     
  if (sockfd < 0)
    fatalError("Error while opening socket");
     
  server = gethostbyname(argv[1]);
     
  if (server == NULL) 
    fatalError("Error, hostname incorrect !");
     
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
  serv_addr.sin_port = htons(portno);
     
  if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0)
    fatalError("Error while connecting");
     
  //tant qu'on peut lire sur le socket
  while(n = read(sockfd,buffer,256)){ 
    printf("%s",buffer);
    memset(buffer,0,256);
       
    fgets(buffer,256,stdin);//lecture au clavier
       
    //ecriture sur socket
    if((n = write(sockfd,buffer,strlen(buffer))) < 0)
      fatalError ("Error while writing on the socket");
       
    memset(buffer,0,256);
  }
     
  close(sockfd); //fermer le flux
     
  return 0;
}
