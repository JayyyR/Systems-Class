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
#include <sys/wait.h>

#include	<signal.h>
#include	<errno.h>
#include	<string.h>
#include	<stdio.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include        <arpa/inet.h>
#include        <stdlib.h>

void printsin(struct sockaddr_in*, char*, char*);


//structs
struct node {
  char* data;
  struct node *next;
  struct node *prev;
};

struct list {
  struct node *start;
  struct node *end;
};

//Declarations
int initList(struct list *sList, char* data);
int push(struct list *sList, char* data);
char* grab(struct list *sList);
int emptyCheck(struct list *sList);
int pop(struct list *sList);
int print(struct list *sList);
void my_handler(int signum);

//globals
struct list handleList;
int handleLength = 0;
int playerSearching = 0;
int noPush =1;


void my_handler(int signum)
{
    if (signum == SIGUSR2)
    {
        printf("Received SIGUSR2!\n");
	//remove file
	remove("hostnport.txt");
    }
}



int main(int argc, char *argv[])
{
  printf("\nServer is running\n");
  
  //initialize linked lists of handles
  
  /*//String to send asking player for handle
  char* handleQ;
  handleQ =  malloc((LINE_MAX* sizeof(char))+1);
  handleQ = "What's your Player Handle?";
  printf("handle q is %s\n", handleQ);
  */
  int server2match[2];
  int hits;
  int tcpSocket;  /* fd for socket on which we get connection requests */
  int udpSocket;
  int conn, conn2, udpConn;      /* fd for socket thru which we pass data */
  struct sockaddr_in *tcpAddr, tcpPeer;
  int tcpecode;
  fd_set mask;
  struct timeval timeout;
  socklen_t length;
  socklen_t lengthUDP;
  //char ch;
  struct addrinfo tcpHints, *tcpAddrlist;
  int PID=0;
  
  char* p1handletoPass;
  p1handletoPass =  malloc((LINE_MAX* sizeof(char))+1);
  
  char* password;
  password =  malloc((LINE_MAX* sizeof(char))+1);
  
  struct sockaddr_in *udpAddr, udpPeer;
  int udpecode;
  struct addrinfo udpHints, *udpAddrlist;
  
  //password mode
  if (argc == 2){
    password = argv[1];
    //printf("password is: %s\n", password);
    
    
  }
  //////FOR TCP SOCKET////////////////////////////////////////////////////////////////
  memset(&tcpHints, 0, sizeof(tcpHints));
  tcpHints.ai_family = AF_INET; tcpHints.ai_socktype = SOCK_STREAM;
  tcpHints.ai_flags = AI_NUMERICSERV | AI_PASSIVE; tcpHints.ai_protocol = 0;
  tcpHints.ai_canonname = NULL; tcpHints.ai_addr = NULL;
  tcpHints.ai_next = NULL;
  
  tcpecode = getaddrinfo(NULL, "0", &tcpHints, &tcpAddrlist);
  if (tcpecode != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(tcpecode));
    exit(1);
  }
  
  tcpAddr = (struct sockaddr_in *) tcpAddrlist->ai_addr;
  
  /*
   *  Create socket on which we will accept connections. This is NOT the
   *  same as the socket on which we pass data.
   */
  if ( (tcpSocket = socket( tcpAddrlist->ai_family, tcpAddrlist->ai_socktype, 0 )) < 0 ) {
    perror("inet_rstream:socket");
    exit(1);
  }
  
  if (bind(tcpSocket, (struct sockaddr *)tcpAddr, sizeof(struct sockaddr_in)) < 0) {
    perror("inet_rstream:bind");
    exit(1);
  }
  
  /*
   *  Print out the port number assigned to this process by bind().
   */
  length = sizeof(struct sockaddr_in);
  if (getsockname(tcpSocket, (struct sockaddr *)tcpAddr, &length) < 0) {
    perror("inet_rstream:getsockname");
    exit(1);
  }
  
  
  //grab host name
  char hostname[1024];
  hostname[1023] = '\0';
  gethostname(hostname, 1023);
  //printf("host name is %s.\n", hostname);
  
  //make port num a string
  char* tcpPortNum;
  tcpPortNum =  malloc((LINE_MAX* sizeof(char))+1);
  sprintf(tcpPortNum, "%d", ntohs(tcpAddr->sin_port));
  
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  //////FOR UDP SOCKET////////////////////////////////////////////////////////////////
  memset(&udpHints, 0, sizeof(udpHints));
  udpHints.ai_family = AF_INET; udpHints.ai_socktype = SOCK_DGRAM;
  udpHints.ai_flags = AI_NUMERICSERV | AI_PASSIVE; udpHints.ai_protocol = 0;
  udpHints.ai_canonname = NULL; udpHints.ai_addr = NULL;
  udpHints.ai_next = NULL;
  
  udpecode = getaddrinfo(NULL, "0", &udpHints, &udpAddrlist);
  if (udpecode != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(udpecode));
    exit(1);
  }
  
  udpAddr = (struct sockaddr_in *) udpAddrlist->ai_addr;
  
  /*
   *  Create socket on which we will accept connections. This is NOT the
   *  same as the socket on which we pass data.
   */
  if ( (udpSocket = socket( udpAddrlist->ai_family, udpAddrlist->ai_socktype, 0 )) < 0 ) {
    perror("inet_rstream:socket");
    exit(1);
  }
  
  if (bind(udpSocket, (struct sockaddr *)udpAddr, sizeof(struct sockaddr_in)) < 0) {
    perror("inet_rstream:bind");
    exit(1);
  }
  
  /*
   *  Print out the port number assigned to this process by bind().
   */
  lengthUDP = sizeof(struct sockaddr_in);
  if (getsockname(udpSocket, (struct sockaddr *)udpAddr, &lengthUDP) < 0) {
    perror("inet_rstream:getsockname");
    exit(1);
  }
  
  
  //make port num a string
  char* udpPortNum;
  udpPortNum =  malloc((LINE_MAX* sizeof(char))+1);
  sprintf(udpPortNum, "%d", ntohs(udpAddr->sin_port));
  
  //printf("RSTREAM:: assigned port number udp %d\n", ntohs(udpAddr->sin_port));
  
  //   
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  
  //write host then port to a file
  FILE *file; 
  file = fopen("hostnport.txt","w"); /* apend file (add text to 
  a file or create a file if it does not exist.*/ 
  fprintf(file,"%s %s,%s.", hostname, tcpPortNum, udpPortNum); /*writes*/ 
  fclose(file); /*done!*/ 
  
  //printf("RSTREAM:: assigned port number %d\n", ntohs(tcpAddr->sin_port));
  
  /*
   *  Now accept a single connection. Upon connection, data will be
   *  passed through the socket on descriptor conn.
   */
      
  
  
  for(;;) {
    //printf("in for\n");
    
    signal(SIGUSR2, my_handler);
    
    
    listen(tcpSocket,1);
    listen(udpSocket,1);
    length = sizeof(tcpPeer);
    lengthUDP = sizeof(udpPeer);
    /* Here's the new stuff. Hang a select on the file descriptors 0 (stdin)
     * and socket_fd looking to see if either descriptor is able to be read.
     * If it's stdin, shut down. If it's socket_fd, proceed as normal. If
     * Nothing happens for a minute, shut down also.
     */
    //printf("before mask\n");
    FD_ZERO(&mask);
    FD_SET(udpSocket,&mask);
    FD_SET(tcpSocket,&mask);
    
    int maxSock;
    if (udpSocket > tcpSocket)
      maxSock = udpSocket;
    else
      maxSock = tcpSocket;
    
    timeout.tv_sec = 3600;
    timeout.tv_usec = 0;
    //printf("after mask\n");
    if ((hits = select(maxSock+1, &mask, (fd_set *)0, (fd_set *)0,&timeout)) < 0) {
      perror("nim_server:select");
      exit(1);
    }
    int status;
    
    //grab PID if its done
    PID = wait3(&status, WNOHANG, NULL);
    if (PID != -1){
      //remove the PID and handles from the linked list
      char* PIDstr;
      PIDstr =  malloc((LINE_MAX* sizeof(char))+1);
      sprintf(PIDstr, "%d", PID);
      //add a tab to make it equal to what we did to the child
      strcat(PIDstr, "\t");
      //printf("PIDSTRING is .%s.\n", PIDstr);
      //printf("printing list BEFORE CHANGES----------\n");
      //print(&handleList);
      //printf("----------------------\n");
      
      struct node *q;
      q = handleList.start;
      //printf("Before while loop q data is .%s.\n", q->data);
      while (q != handleList.end && q-> data != NULL){
	//printf("in while q data is .%s.\n", q->data);
	//if the node is the PID
	if (strcmp(q->data, PIDstr) == 0){
	  //printf("in IF\n");
	  //delete the node and the next two (the handles)
	  if (handleLength != 3){
	    //printf("in if\n");
	    //printf("qdata is .%s.\n", q->data);
	    //if not at top of list
	    if (q->prev != NULL){
	      //printf("qprev is not null\n");
	      q->prev->next = q->next->next->next;
	      if (q->next->next->next != NULL){
		//printf("qnextnextnext is not null\n");
		q->next->next->next->prev = q->prev;
	      }
	      else{
		//printf("qnextnextnext is null\n");
		handleList.end = q->prev;
		if (q->prev->prev == NULL){
		  handleList.start = q->prev;
		  //printf("qprevprev is null\n");
		}
	      }
	    }
	    //3 entries to be removed are at top of the list
	    else{
	      //printf("qprev was null\n");
	      handleList.start = q->next->next->next;
	    }
	    //printf("before seg fault\n");
	    free(q->next->next);
	    free(q->next);
	    free(q);
	    handleLength -= 3;
	  }
	  else{
	    //printf("in else\n");
	    pop(&handleList);
	    pop(&handleList);
	    pop(&handleList);
	    handleLength = 0;
	    initList(&handleList, NULL);
	  }
	}
	//printf("outside of if\n");
	q = q->next;
      }
      //printf("printing list AFTER CHANGES----------\n");
      //print(&handleList);
      //printf("----------------------\n");
    }
    
    
    //printf("PID is %d--------------------------------------------------------------------------\n", PID);
    //printf("after select\n");
    if ( (hits==0) || ((hits>0) && (FD_ISSET(0,&mask))) ) {
      printf("Shutting down\n");
      exit(0);
    }
    if (FD_ISSET(udpSocket, &mask)){
      //printf("hey\n");
      
      
      
      
      char* msg;
      msg =  malloc((LINE_MAX* sizeof(char))+1);
      //printf("message here is %s\n", msg);
      //printf("message here is %s\n", msg);
      //printf("bef\n");
      char cha;
      int pp = 0;
      while (cha != '\n'){
	if ((udpConn=recvfrom(udpSocket, &cha, sizeof(cha), 0, (struct sockaddr *)&udpPeer, &lengthUDP)) < 0) {
	  perror("recv_udp:recvfrom");
	  exit(1);
	}
	//printf("cha in loop is %c\n", cha);
	msg[pp] = cha;
	pp++;
      }
      //reset cha
      cha = 0;
      //printf("Got data : %s\n",msg);
      
      
      //send password
      strcat(password, "\n");
      //printf("password before sending is .%s.\n", password);
      int eee = 0;
      while(password[eee] != '\0'){
	//printf("writing password %c\n", password[eee]);
	if ( sendto(udpSocket, &password[eee], sizeof(password[eee]), 0, (struct sockaddr *)&udpPeer, sizeof(struct sockaddr_in)) < 0) {
	  perror("inet_wstream:connect");
	  exit(1);
	}
	eee++;
      }
      //remove \n from password for next send
      password[strlen(password)-1] = '\0';
      
      char pwordCorrect;
      if( recvfrom(udpSocket, &pwordCorrect, sizeof(pwordCorrect), 0, (struct sockaddr *)&udpPeer, &lengthUDP) < 0){
	  perror("recv_udp:recvfrom");
	  exit(1);
	}
      //printf("pword Correct player 1 is %c\n", pwordCorrect);
      //if pword is not right, continue
      if (pwordCorrect == '0'){
	//printf("continuing1\n");
	continue;
      }
      
      char* msgWrite;
      msgWrite =  malloc((LINE_MAX* sizeof(char))+1);
      strcat(msgWrite, "Nobody is Currently Playing or Looking for an Opponent\n");
      
      int tempHandle = handleLength;
      //if someone is connected to the server reset msg first and add the handles
      if (tempHandle > 0){
	//reset message to nothing so we can add handles
	memset(msgWrite, 0, sizeof(msgWrite));
	struct node *p;
	p = handleList.start;
	
	
	//printf("printing list-----++++++++++++++++++++++++++++++))))))))))))000-----\n");
	//print(&handleList);
	//printf("----------------------\n");
	//printf("temphandle is %d\n", tempHandle);
	while (tempHandle > 0){
	  //printf("temphandle in while is %d\n", tempHandle);
	  //printf("pdata is .%s.\n", p->data);
	  if (p->data[strlen(p->data)-1] != '\t'){
	    //printf("in tab if\n");
	    strcat(msgWrite, "'");
	    strcat(msgWrite, p->data);
	    strcat(msgWrite, "'");
	  }
	  else{
	    //printf("in else if\n");
	    tempHandle--;
	    p = p->next;
	    continue;
	  }
	  
	  //printf("tempHandle before if checks is %d\n", tempHandle);
	  //printf("handleLength before if checks is %d\n", handleLength);
	  
	  //if a person is searching
	  if (tempHandle == handleLength)
	    strcat(msgWrite, " is currently looking for an opponent.\n");
	  //if temp handle is 2, 5, 8 etc....
	  else if ((tempHandle+1)%3 == 0)
	    strcat(msgWrite, " is engaged in a heated match of nim with ");
	  //if it's teh person the player is engaged in a heated match with
	  else
	    strcat(msgWrite, "\n");
	      
	  p = p->next;
	  //printf("AFTER NEXT\n");
	  tempHandle--;
	}
      }
      
      //add \n to msgWrite for handling when reading
      strcat(msgWrite, "\t");
      //printf("message before writing is %s\n", msgWrite);
      pp = 0;
      while (msgWrite[pp] != '\0'){
	if ( sendto(udpSocket, &msgWrite[pp], sizeof(msgWrite[pp]), 0, (struct sockaddr *)&udpPeer, sizeof(struct sockaddr_in)) < 0) {
	  perror("inet_wstream:connect");
	  exit(1);
	}
	pp++;
      }
      
      //printf("after write\n");
      /*
       *     char* msg;
       *     msg =  malloc((LINE_MAX* sizeof(char))+1);
       *     if ((udpConn=recvfrom(udpSocket, &msg, 3, 0, (struct sockaddr *)&udpPeer, &lengthUDP)) < 0) {
       * perror("recv_udp:recvfrom");
       * exit(1);
    }*/
      
      
    }
    
    //if the select chose tcpSocket
    else if (FD_ISSET(tcpSocket, &mask) && playerSearching == 0){
      //printf("in first player\n");
      //printf("tcpsocket\n");
      //grab 2 connections
      if ((conn=accept(tcpSocket, (struct sockaddr *)&tcpPeer, &length)) < 0) {
	perror("inet_rstream:accept");
	exit(1);
      }
      
      //send password
      strcat(password, "\n");
      //printf("password before sending is .%s.\n", password);
      int countNumbo;
      int eee = 0;
      while(password[eee] != '\0'){
	//printf("writing password %c\n", password[eee]);
	if((countNumbo = write(conn, &password[eee], 1)) < 0) {
	  perror("inet_wstream:write");
	  exit(1);
	}
	eee++;
      }
      //remove \n from password for next send
      password[strlen(password)-1] = '\0';
      
      char pwordCorrect;
      read(conn, &pwordCorrect, 1);
      //printf("pword Correct player 1 is %c\n", pwordCorrect);
      //if pword is not right, continue
      if (pwordCorrect == '0'){
	//printf("continuing1\n");
	continue;
      }
      
      //read in handle 1
      
      //reset p1handle
      char* p1handle;
      p1handle =  malloc((LINE_MAX* sizeof(char))+1);
      int counter=0;
      //read handle for p1
      while (read(conn, &p1handle[counter], 1) == 1){
	//printf("reading..p1handle indexed at counter is: .%c.\n", p1handle[counter]);
	if (p1handle[counter] == '\n'){
	  //printf("breaking\n");
	  break;
	}
	counter++;
      }
      p1handletoPass = p1handle;
      //remove \n from handle
      //p1handle[strlen(p1handle)-1] = '\0';
      //printf("p1handle in server is %s.\n.", p1handle);
      /////////////////////////////////
      
      //add handle to linked list
      //copy to new string to remove \n first
      //printf("BEFORE PUSHING P1 handle is %s\n", p1handle);
      char* p1handleToPush = p1handle;
      p1handleToPush = malloc((LINE_MAX* sizeof(char))+1);
      strcpy(p1handleToPush, p1handle);
      //printf("AFTER COPYING P1 handle TO PUSH is %s\n", p1handleToPush);
      p1handleToPush[strlen(p1handleToPush)-1] = '\0';
      if (noPush == 1)
	initList(&handleList, p1handleToPush);
      else
	push(&handleList, p1handleToPush);
      handleLength++;
      noPush = 0;
      
      //printf("BEFORE WRITING TO PLAYER1\n");
      //write 1 to player 1
      int play1;
      if((play1 = write(conn, "1", 1)) < 0) {
	perror("inet_wstream:write");
	exit(1);
      }
      //printf("AFTER WRITING TO PLAYER1\n");
      
      //set flag to 1 to indicate a player is waiting for another
      playerSearching = 1;
      
      
    }
    else if (FD_ISSET(tcpSocket, &mask) && playerSearching == 1){
      //printf("in Second player\n");
      //printf("conn is %d\n", conn);
      
      
      if ((conn2=accept(tcpSocket, (struct sockaddr *)&tcpPeer, &length)) < 0) {
	perror("inet_rstream:accept");
	exit(1);
      }
      
      //add \n to password for passing
      strcat(password, "\n");
      //printf("password before sending to Player 2 is .%s.\n", password);
      int countNumbo;
      int eee = 0;
      while(password[eee] != '\0'){
	//printf("writing password2 %c\n", password[eee]);
	if((countNumbo = write(conn2, &password[eee], 1)) < 0) {
	  perror("inet_wstream:write");
	  exit(1);
	}
	eee++;
      }
      //remove \n from password for next send
      password[strlen(password)-1] = '\0';
      
      char pwordCorrect2;
      read(conn2, &pwordCorrect2, 1);
      //printf("pword Correct player 2 is %c\n", pwordCorrect2);
      //if pword is not right, continue
      if (pwordCorrect2 == '0'){
	//printf("continuing player 2\n");
	continue;}
      
      
      
      //read in handle 2
      char* p2handle;
      p2handle =  malloc((LINE_MAX* sizeof(char))+1);
      int counter = 0;
      //read handle for p2
      //printf("before while of p2handle99999999999999999999999: %s\n", p2handle);
      while (read(conn2, &p2handle[counter], 1) == 1){
	// printf("reading2..%s");
	//printf("reading..p2handle indexed at counter is: .%c.\n", p2handle[counter]);
	if (p2handle[counter] == '\n'){
	  //printf("breaking2\n");
	  break;
	}
	counter++;
      }
      //remove \n from handle
      //p2handle[strlen(p2handle)-1] = '\0';
      //printf("p2handle in server is %s.\n.", p2handle);
      /////////////////////////////////
      
      //printf("conn2 is %d\n", conn2);
      
      
      
      //add handle to linked list
      //copy to new string to remove \n first
      char* p2handleToPush;
      p2handleToPush = malloc((LINE_MAX* sizeof(char))+1);
      strcpy(p2handleToPush, p2handle);
      p2handleToPush[strlen(p2handleToPush)-1] = '\0';
      push(&handleList, p2handleToPush);
      handleLength++;
      
      //printf("After LINKED LIST SHIT P1HANDLE to passis %s\n", p1handletoPass);
      //printf("After LINKED LIST SHIT P2HANDLE is %s\n", p2handle);
      
      //printf("BEFORE WRITING TO PLAYER2\n");
      //write 2 to player 2
      int play2;
      if((play2 = write(conn2, "2", 1)) < 0) {
	perror("inet_wstream:write");
	exit(1);
      }
      //printf("AFTER WRITING TO PLAYER2\n");
      fflush(stdout);
      
      
      //write handle1 to player2 ///////////////////
      int countNum;
      int ee = 0;
      while(p1handletoPass[ee] != '\0'){
	if((countNum = write(conn2, &p1handletoPass[ee], 1)) < 0) {
	  perror("inet_wstream:write");
	  exit(1);
	}
	ee++;
      }
      
      
      //write handle2 to player1
      ee = 0;
      while(p2handle[ee] != '\0'){
	if((countNum = write(conn, &p2handle[ee], 1)) < 0) {
	  perror("inet_wstream:write");
	  exit(1);
	}
	ee++;
      }
      ///////////////////////////////////////////////
      
      
      
      
      char* connStr;
      connStr =  malloc((LINE_MAX* sizeof(char))+1);
      sprintf(connStr, "%d", conn);
      
      char* conn2Str;
      conn2Str =  malloc((LINE_MAX* sizeof(char))+1);
      sprintf(conn2Str, "%d", conn2);
      
      //char* testOut;
      //testOut =  malloc((LINE_MAX* sizeof(char))+1);
      pipe(server2match);
      int child = fork();
      if (child == 0){
	
	dup2(server2match[0], 0);
	
	
	dup2(1, 1);
	
	close(server2match[0]);
	close(server2match[1]);
	
	if (execl("./nim_match_server", "./nim_match_server", connStr, conn2Str, NULL) == -1){
	  fprintf(stderr, "Error: In incorrect directory\n"); exit(1);
	}
      }
      else{
	/*printf("IN parent ----\n");
	 * char* deleteHan;
	 *       deleteHan =  malloc((LINE_MAX* sizeof(char))+1);
	 * int e = 0;
	 * char oneLet;
	 * oneLet = fgetc(stdin);
	 * while (oneLet != EOF){
	 *  
	 *  if (oneLet != '\n'){
	 *    deleteHan[e] = oneLet;
	 *    e++;
      }
      }
      printf("----------------------------deletehan is %s\n", deleteHan);
      */
	close(server2match[0]);
	close(server2match[1]);
	//printf("IN parent ----\n");
	//printf("***********************************child is %d\n", child);
	char* childStr;
	childStr =  malloc((LINE_MAX* sizeof(char))+1);
	//printf("PUSHING CHILD....\n");
	sprintf(childStr, "%d", child);
	strcat(childStr, "\t");
	handleLength++;
	push(&handleList, childStr);
	//printf("after pipe+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	//printf("printing list----------\n");
	//print(&handleList);
	//printf("----------------------\n");
	
	
	
      }
      
      /*
       * char* deleteHan;
       *       deleteHan =  malloc((LINE_MAX* sizeof(char))+1);
       * int e = 0;
       * char oneLet;
       * oneLet = fgetc(stdin);
       * while (oneLet != EOF){
       *  
       *  if (oneLet != '\n'){
       *    deleteHan[e] = oneLet;
       *    e++;
       }
       }
       printf("----------------------------deletehan is %c\n", oneLet);*/
      /* char* handle2Delete;
       *     handle2Delete =  malloc((LINE_MAX* sizeof(char))+1);
       *     counter=0;
       *     //read handle for p1
       *     while (read(conn, &handle2Delete[counter], 1) == 1){
       * // printf("reading..\n");
       * if (handle2Delete[counter] == '\n'){
       *  //printf("breaking\n");
       *  break;
       }
       counter++;
       }
       printf("handle to delete is %s\n", handle2Delete);*/
      playerSearching = 0;
      //printf("test1 is %s\n", testOut);
    }
  }
  
  
  
  return 0;
}


int initList(struct list *sList, char* data){
  struct node *p;
  p = malloc(sizeof(struct node));
  p->data = data;
  p->next = NULL;
  p->prev = NULL;
  sList->start = p;
  sList->end = p;
  
  return 0;
}

int push(struct list *sList, char* data){
  struct node *p;
  p = malloc(sizeof(struct node));
  p->data = data;
  p->next = sList->start;
  p->prev = NULL;
  sList->start->prev = p;
  sList ->start = p;
  
  return 0;
}

char* grab(struct list *sList){
  if(sList->start != NULL){
    struct node *p = sList->start;
    return p->data;
  }
  return 0;
}

int emptyCheck(struct list *sList){
  if(sList->start != NULL)
    return 0;
  else
    return 1;
}

int pop(struct list *sList){
  if(sList->start != NULL){
    struct node *p = sList->start;
    sList->start = sList->start->next;
    free(p);
  }
  
  return 0;
}

int print(struct list *sList)
{
  struct node *p = sList->start;
  while(p != sList->end) {
    fprintf(stderr, "%s\n", p->data);
    p=p->next;
  }
  fprintf(stderr, "%s\n", p->data);
  
  return 0;
}