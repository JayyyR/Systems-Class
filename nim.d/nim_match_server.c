#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <limits.h>

int main(int argc, char *argv[])
{
  int conn, conn2;
  conn = atoi(argv[1]);
  conn2 = atoi(argv[2]);
  //f//printf(stderr, "conn nms is %d\n", conn);
  //fprintf(stderr, "conn2 nms is %d\n", conn2);
  char ch;
  int playerResigned = 0;
  /*
  int listener;  
  int conn, conn2;    
  struct sockaddr_in *localaddr, peer;
  int ecode;
  socklen_t length;
  char ch;
  struct addrinfo hints, *addrlist;
  
  
   
   //  Want to specify local server address of:
   //     addressing family: AF_INET
   //     ip address:        any interface on this system 
   //     port:              0 => system will pick free port

  
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET; hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_NUMERICSERV | AI_PASSIVE; hints.ai_protocol = 0;
  hints.ai_canonname = NULL; hints.ai_addr = NULL;
  hints.ai_next = NULL;
  
  ecode = getaddrinfo(NULL, "0", &hints, &addrlist);
  if (ecode != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ecode));
    exit(1);
  }
  
  localaddr = (struct sockaddr_in *) addrlist->ai_addr;
  
 
   //  Create socket on which we will accept connections. This is NOT the
   //  same as the socket on which we pass data.
   
  if ( (listener = socket( addrlist->ai_family, addrlist->ai_socktype, 0 )) < 0 ) {
    perror("inet_rstream:socket");
    exit(1);
  }
  
  
  
  
  if (bind(listener, (struct sockaddr *)localaddr, sizeof(struct sockaddr_in)) < 0) {
    perror("inet_rstream:bind");
    exit(1);
  }
  
  
   //  Print out the port number assigned to this process by bind().
   
  length = sizeof(struct sockaddr_in);
  if (getsockname(listener, (struct sockaddr *)localaddr, &length) < 0) {
    perror("inet_rstream:getsockname");
    exit(1);
  }
  
  
  //grab host name
  char hostname[1024];
  hostname[1023] = '\0';
  gethostname(hostname, 1023);
  //printf("host name is %s.\n", hostname);
  
  //make port num a string
  char* portNum;
  portNum =  malloc((LINE_MAX* sizeof(char))+1);
  sprintf(portNum, "%d", ntohs(localaddr->sin_port));
  
  //write host then port to a file
  FILE *file; 
  file = fopen("hostnport.txt","w"); // apend file (add text to a file or create a file if it does not exist.
  fprintf(file,"%s %s.", hostname, portNum); //writes
  fclose(file); //done!
  
  printf("RSTREAM:: assigned port number %d\n", ntohs(localaddr->sin_port));
  
  
   //  Now accept a single connection. Upon connection, data will be
   //  passed through the socket on descriptor conn.
   
  
  listen(listener,1);
  length = sizeof(peer);
  
  if ((conn=accept(listener, (struct sockaddr *)&peer, &length)) < 0) {
    perror("inet_rstream:accept");
    exit(1);
  }
  if ((conn2=accept(listener, (struct sockaddr *)&peer, &length)) < 0) {
    perror("inet_rstream:accept");
    exit(1);
  }
  
  //write 1 to player 1
  int play1;
  if((play1 = write(conn, "1", 1)) < 0) {
    perror("inet_wstream:write");
    exit(1);
  }
  
  //write 2 to player 2
  int play2;
  if((play2 = write(conn2, "2", 1)) < 0) {
    perror("inet_wstream:write");
    exit(1);
  }
  
  */
  
  //printsin(&peer,"RSTREAM::", "accepted connection from"); 
  while(1){

    //read in move from client1
    char* moveMade;
    moveMade =  malloc((LINE_MAX* sizeof(char))+1);
    //char* gameOver;
    //gameOver =  malloc((LINE_MAX* sizeof(char))+1);
    //printf("heyyo\n");
    //printf("BEFORE FIRST READ OF CH\n");
    read(conn, &ch, 1);
    
    
   
    
    moveMade[0] = ch;
    read(conn, &ch, 1);
    //printf("second read is .%c.\n", ch);
    
     //printf("BEFORE EOF CHECK\n");
    if(ch != ' ' && ch != 'q'){
      //printf("MOVE made was control c!\n");
      playerResigned = 1;
      //write 0 0 to simulate a resignation and break
      if((write(conn2, "0", 1)) < 0) {
	perror("inet_wstream:write");
	exit(1);
      }
      if((write(conn2, " ", 1)) < 0) {
	perror("inet_wstream:write");
	exit(1);
      }
      if((write(conn2, "0", 1)) < 0) {
	perror("inet_wstream:write");
	exit(1);
      }
      break;
    }
    //printf("After EOF CHECK\n");
    
    //temp game over check...not sure middle char is always 1?
    if (ch == 'q'){
     //fprintf(stderr, "ending\n");
      
      break;
    }
    
    moveMade[1] = ch;
    read(conn, &ch, 1);
    
    //printf("third read is .%c.\n", ch);
    moveMade[2] = ch;
    // gameOver[0] = ch;
    
    
    //write move to client2
    int num;
    int d = 0;
    //fprintf(stderr, "move made is %s\n.........", moveMade);
    
    
    
    if (strcmp(moveMade, "0 0") == 0){
      //printf("move was 0 0\n");
      playerResigned = 1;
    }
    
    while(moveMade[d] != '\0'){
      //fprintf(stderr, "writing....\n");
      if((num = write(conn2, &moveMade[d], 1)) < 0) {
	perror("inet_wstream:write");
	exit(1);
      }
      d++;
    }
    
    if (playerResigned ==1 )
      break;
    
    //char* gameOverw;
    //gameOverw =  malloc((LINE_MAX* sizeof(char))+1);
    //read in move from client2
    char* moveMade2;
    moveMade2 =  malloc((LINE_MAX* sizeof(char))+1);
    read(conn2, &ch, 1);
    //printf("first read is .%c.\n", ch);
    moveMade2[0] = ch;
    read(conn2, &ch, 1);
    //printf("second read is .%c.\n", ch);
    
    if(ch != ' ' && ch != 'q'){
      //printf("MOVE made was control c2!\n");
      playerResigned = 1;
      //write 0 0 to simulate a resignation and break
      if((write(conn, "0", 1)) < 0) {
	perror("inet_wstream:write");
	exit(1);
      }
      if((write(conn, " ", 1)) < 0) {
	perror("inet_wstream:write");
	exit(1);
      }
      if((write(conn, "0", 1)) < 0) {
	perror("inet_wstream:write");
	exit(1);
      }
      break;
    }
    
    moveMade2[1] = ch;
    read(conn2, &ch, 1);
    //printf("third read is .%c.\n", ch);
    moveMade2[2] = ch;
    //read(conn, &ch, 1);
    //read(conn, &ch, 1);
    // gameOverw[0] = ch;
    //fprintf(stderr, "move made2 is %s\n", moveMade2);
    
    //write move to client1
    int num2;
    int dd = 0;
    while(moveMade2[dd] != '\0'){
      if((num2 = write(conn, &moveMade2[dd], 1)) < 0) {
	perror("inet_wstream:write");
	exit(1);
      }
      dd++;
    }
    
  }
  exit(0);
}
