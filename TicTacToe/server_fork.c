


/*--------------------------------------------------------------------
     But: programme du Tic Tac Toe utilisant des socket 
               client-serveur de type Stream et fork()
	  (nb: le processus client s'interrompe quand il ne veux
	     plus continuer la partie; le processus serveur reste 
	     actif dans l'attente d'une nouvelle connexion 
	    nb2: fork() permet la creation de processus qui prendra
	     en charge la requete du client courant et libere ainsi
	     le serveur; ce dernier se remet dans l'attente de
	     nouvelle requetes client)
   
  --------------------    LE SERVEUR   -------------------------------*/

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <time.h>//pour srand()

//sortie de programme avec message d'erreur
void fatalError(char *msg) {
  perror(msg);
  exit(1); 
}

//message d'erreur avec fermeture du socket client
void minorError(char *msg, int sock) {
  perror(msg);
  close(sock); //ferme la connexion socket avec le client
} 

//mise a jour de l'interface dans le buffer
void setInterface(char *buf, char *tab){
  sprintf(buf,"\n\t\t %c | %c | %c \n"
	  "\t\t-----------\n"
	  "\t\t %c | %c | %c \n"
	  "\t\t-----------\n"
	  "\t\t %c | %c | %c \n\n\n",
	  tab[0],tab[1],tab[2],tab[3],tab[4],tab[5],tab[6],tab[7],tab[8]);
}//setInterface()

//mise a jour du jeu apres chaque nouveau coup
int setTable(char player,char position,char *tab, int tabLen){
  int i;
  int free = 0; //case libre ??
  
  if(player != '0'){ //ce n' est pas un debut de partie
    for(i = 0; i < tabLen; i++){
      if(position == tab[i]){ //verifie egalement si la case n' est pas occupee !
	tab[i] = player;
	free = 1;
	break;
	
      }//if
    }//for
    
    if(free == 0) return (-1); //case occupee !
  
  }else{//nouvelle partie
    strcpy(tab,"123456789");
  }
  return (0);
}//setTable()

enum { None, Server, Player, Tie};//TABLEAU D' ENTIER COMMENCANT PAR 0

//y at-il un gagnant ?
int test_won(char *tab){
  int i,flag = 0;
  if ((tab[0] == 'x' && tab[1] == 'x' && tab[2] == 'x') ||
      (tab[3] == 'x' && tab[4] == 'x' && tab[5] == 'x') ||
      (tab[6] == 'x' && tab[7] == 'x' && tab[8] == 'x') || 
      (tab[0] == 'x' && tab[3] == 'x' && tab[6] == 'x') || 
      (tab[1] == 'x' && tab[4] == 'x' && tab[7] == 'x') || 
      (tab[2] == 'x' && tab[5] == 'x' && tab[8] == 'x') || 
      (tab[0] == 'x' && tab[4] == 'x' && tab[8] == 'x') || 
      (tab[2] == 'x' && tab[4] == 'x' && tab[6] == 'x'))
    return Server;

  if ((tab[0] == 'o' && tab[1] == 'o' && tab[2] == 'o') ||
      (tab[3] == 'o' && tab[4] == 'o' && tab[5] == 'o') ||
      (tab[6] == 'o' && tab[7] == 'o' && tab[8] == 'o') || 
      (tab[0] == 'o' && tab[3] == 'o' && tab[6] == 'o') || 
      (tab[1] == 'o' && tab[4] == 'o' && tab[7] == 'o') || 
      (tab[2] == 'o' && tab[5] == 'o' && tab[8] == 'o') || 
      (tab[0] == 'o' && tab[4] == 'o' && tab[8] == 'o') || 
      (tab[2] == 'o' && tab[4] == 'o' && tab[6] == 'o'))
    return Player;

  //match nul ??
  for (i=0; i<9; i++) {
    if ((tab[i] != 'x') && (tab[i] != 'o'))
      flag=1;
  }
  if (!flag)
    return Tie;//match nul

  return None; // le match continue 
}//testWon()

//le message a renvoyer au client
void setMesg(int w, char *buf, int *d){
  if(w == 0){ 
    strcat(buf,"Vous etes le joueur 'o' et moi le joueur 'x'.\n");
    strcat(buf,"Pour gagner, alignez 3 pions sur une ligne.\n ");
    strcat(buf,"Choisissez une position entre 1 et 9 pour"
	   " votre prochain pion -->  "); 
    
  }else if(w == -1){
    strcat(buf, "Cette case deja est deja occupee; \n\tfaites un autre choix, SVP ! --> ");
  }else { 
    *d = 1; 
    if(w == Player ){ 
      strcat(buf,"\tVOUS GAGNEZ !\n"); 
    } 
    else if(w == Server){ 
      strcat(buf,"\tLE SERVEUR GAGNE !\n"); 
    } 
    else if(w == Tie ){ 
      strcat(buf,"MATCH NUL !\n"); 
    } 
    strcat(buf,"Voulez-vous continuer (o/n) ? --> "); 
  } 
}//setMsg()

// le jeu du serveur
//nb: on assume que la fonction 'test_won()' a deja verifie qu'il 
//       n y a pas encore de gagnant a ce stade
char serverPlay(char *tab, int tabLen){
  int i, position, free = 0;
  char choice;

  //tenter de finir 
  
  for(i=0; i <= 6; i++){ 
    switch(i){
      //tenter la ligne
    case 0://tenter egalement la diagonale gauche
      if((tab[0] == tab [4]) && (tab[0] == 'x') &&
	 (tab[8] != 'o'))       return tab[8];
      else if((tab[0] == tab [8]) && (tab[0] == 'x') &&
	      (tab[4] != 'o'))       return tab[4];
      else if((tab[4] == tab [8]) && (tab[4] == 'x') &&
	      (tab[0] != 'o'))       return tab[0];
      //tenter egalement la colonne
      if((tab[i] == tab [i+3]) && (tab[i] == 'x') &&
	 (tab[i+6] != 'o'))       return tab[i+6];
      else if((tab[i] == tab [i+6]) && (tab[i] == 'x') &&
	      (tab[i+3] != 'o'))       return tab[i+3];
      else if((tab[i+3] == tab [i+6]) && (tab[i+3] == 'x') &&
	      (tab[i] != 'o'))       return tab[i];	 
        	 
    case 3:
    case 6: 
      if((tab[i] == tab [i+1]) && (tab[i] == 'x') &&
	 (tab[i+2] != 'o'))       return tab[i+2];
      else if((tab[i] == tab [i+2]) && (tab[i] == 'x') &&
	      (tab[i+1] != 'o'))       return tab[i+1];
      else if((tab[i+1] == tab [i+2]) && (tab[i+1] == 'x') &&
	      (tab[i] != 'o'))       return tab[i];
      break;
      //tenter les colonnes      
    case 2:
      //tenter egalement la diagonale droite
      if((tab[2] == tab [4]) && (tab[2] == 'x') &&
	 (tab[6] != 'o'))       return tab[6];
      else if((tab[2] == tab [6]) && (tab[2] == 'x') &&
	      (tab[4] != 'o'))       return tab[4];
      else if((tab[4] == tab [6]) && (tab[4] == 'x') &&
	      (tab[2] != 'o'))       return tab[2];
       
    case 1:if((tab[i] == tab [i+3]) && (tab[i] == 'x') &&
	      (tab[i+6] != 'o'))       return tab[i+6];
    else if((tab[i] == tab [i+6]) && (tab[i] == 'x') &&
	    (tab[i+3] != 'o'))       return tab[i+3];
    else if((tab[i+3] == tab [i+6]) && (tab[i+3] == 'x') &&
	    (tab[i] != 'o'))       return tab[i];        	 
    break;
             
    default : break;
    }//switch  	  
  }//for
	 	 		       
  //sinon, tenter de bloquer l'adversaire
  for(i=0; i <= 6; i++){ 
    switch(i){
      //tenter la ligne
    case 0://tenter egalement la diagonale gauche
      if((tab[0] == tab [4]) && (tab[0] == 'o') &&
	 (tab[8] != 'x'))       return tab[8];
      else if((tab[0] == tab [8]) && (tab[0] == 'o') &&
	      (tab[4] != 'x'))       return tab[4];
      else if((tab[4] == tab [8]) && (tab[4] == 'o') &&
	      (tab[0] != 'x'))       return tab[0];
      //tenter egalement la colonne
      if((tab[i] == tab [i+3]) && (tab[i] == 'o') &&
	 (tab[i+6] != 'x'))       return tab[i+6];
      else if((tab[i] == tab [i+6]) && (tab[i] == 'o') &&
	      (tab[i+3] != 'x'))       return tab[i+3];
      else if((tab[i+3] == tab [i+6]) && (tab[i+3] == 'o') &&
	      (tab[i] != 'x'))       return tab[i];	 
        	 
    case 3:
    case 6: 
      if((tab[i] == tab [i+1]) && (tab[i] == 'o') &&
	 (tab[i+2] != 'x'))       return tab[i+2];
      else if((tab[i] == tab [i+2]) && (tab[i] == 'o') &&
	      (tab[i+1] != 'x'))       return tab[i+1];
      else if((tab[i+1] == tab [i+2]) && (tab[i+1] == 'o') &&
	      (tab[i] != 'x'))       return tab[i];
      break;
      //tenter les colonnes      
    case 2:
      //tenter egalement la diagonale droite
      if((tab[2] == tab [4]) && (tab[2] == 'o') &&
	 (tab[6] != 'x'))       return tab[6];
      else if((tab[2] == tab [6]) && (tab[2] == 'o') &&
	      (tab[4] != 'x'))       return tab[4];
      else if((tab[4] == tab [6]) && (tab[4] == 'o') &&
	      (tab[2] != 'x'))       return tab[2];
       
    case 1:if((tab[i] == tab [i+3]) && (tab[i] == 'o') &&
	      (tab[i+6] != 'x'))       return tab[i+6];
    else if((tab[i] == tab [i+6]) && (tab[i] == 'o') &&
	    (tab[i+3] != 'x'))       return tab[i+3];
    else if((tab[i+3] == tab [i+6]) && (tab[i+3] == 'o') &&
	    (tab[i] != 'x'))       return tab[i];        	 
    break;
             
    default : break;
    }//switch  	  
  }//for 	 

  //Sinon, choisir au hasard une des cases vides
  for(i = 0; i < tabLen; i++){
    if( (tab[i] != 'o') && (tab[i] != 'x') ){
      free ++; 
    }
  }
  position = 1 + rand () % free;
  free = 0;
  for(i = 0; i < tabLen; i++){
    if( (tab[i] != 'o') && (tab[i] != 'x') ){
      free ++; 
      if (free == position)  choice = tab[i];
    }
  }
  return choice;
} //serverPlay()


/*______________________________main()___________________________*/

int main(int argc, char *argv[]) {
  char c;//parametre de setTable
  int decision ;// recommencer ou non la partie ?
  int winner = 0;
  char table[9];//memorisation des positions du jeu > tableau de max 9
  int sockfd, newsockfd, portno, clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  srand(time(0)); //initialisation aleatoire de rand() avec le temps en seconde

  if (argc != 2) {
    fprintf(stderr,"Error, usage: \"%s <port>\"", argv[0]);
    fprintf(stderr,"\n\tTHE PROGRAM WILL TERMINATE !\n\n");
    exit(1);
  }

  portno = atoi(argv[1]);
  
  /* creer une socket stream*/
  if ((sockfd =socket(AF_INET,SOCK_STREAM, 0)) < 0) 
    fatalError("Erreur d'ouverture socket"); //sortie de programme
  
  memset((char *) &serv_addr,0, sizeof(serv_addr)); //initialsiation
  
  /* bind  une addresse */
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
	   sizeof(serv_addr)) < 0)
    fatalError("Error on bind())"); //sortie de programme

  /* specifier le maximum duquel le socket refusera les requetes clients ulterieures*/
  listen(sockfd,5);
     
  /* le serveur s'apprete a accepter plusieurs connexions */
  for (;;) { //maintient du processus serveur quand le client termine
     
    fprintf(stderr,"Server ready, waiting on the port [%i]\n", portno);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
 		       (struct sockaddr *) &cli_addr,
 		       &clilen);
    if (newsockfd < 0){
      perror("Error on accept())");
      continue; // sortir de la boucle en court
    }
     
    /* creer un processus pour prendre en charge la connexion */
    if (fork() == 0) { //on est dans le processus fils
      close(sockfd); //fermer le socket server
	
      //initialisation d'une partie  
      setTable('0','0', table, 9);    //initialisation
      decision = 0;// recommencer ou non la partie ?   
    
      do { // tant que le client veut continuer
	setInterface(buffer,table);
             
	if(winner != -1)//la case choisie par le client etait libre
	  winner = test_won(table);
	
	setMesg(winner, buffer, &decision);
                           
	if ((n = write(newsockfd,buffer,256)) > 0) {
	  memset(buffer,0,256);
	  if((n = read(newsockfd,buffer,255)) < 0){	
	    minorError("Problem of reading on the socket", newsockfd);
	    break;
	  }	
               
	  if(decision == 0 ){
	    if( 0 == setTable('o',buffer[0],table,9)){
	      if(( winner = test_won(table)) == 0){
		c = serverPlay(table,9);//le serveur joue
		setTable('x',c,table,9);
	      }
	    }else
	      winner = -1; //case deja occupee
                          	 
	  }else{ // fin de la partie
	    if (buffer[0] == 'o'){ 
	      setTable('0','0', table, 9); 
	      decision = 0;
       	       
	    }else 
	      break;// le client ne veut plus jouer                         	       
       	       	  
	  }//else
             
	} else if (n < 0){ 
	  minorError("Problem of writing on the socket\n", newsockfd);
	  break;
	}	
        
      }while(1);
        
      close(newsockfd); // le serveur ferme la connexion ce qui va 
      //liberer le processus client de sa boucle d'ecoute et se terminera 
      exit(0);//fin du processus fils break;  
                        
    }else{ //cas d' erreur (fork() == -1) et cas du processus parent (fork() > 0)
      close(newsockfd); //liberer le socket client
      //sortir de la boucle...
    }
  }//for()
  return 0;
}












