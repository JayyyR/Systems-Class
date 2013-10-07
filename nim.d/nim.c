#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <limits.h>



//Joseph Acosta
//Program 4
//CS 415

int main(int argc, char *argv[]){
  
  char* board[4][7];
  //populate board with zeros
  int v, y;
  for(v = 0; v < 4; v++) {
    for(y = 0; y < 7; y ++)
      board[v][y] = "0";
  }
  
  
  //set intial configuration
  int x;
  //printf("x is %d\n", x);
  for (x = 0; x<7; x++){
    if (x<=0)
      board[0][x]= "x";
    if(x<=2)
      board[1][x] = "x";
    if (x<=4)
      board[2][x] = "x";
    if (x <= 6) 
      board[3][x] = "x";
  }
  
  
  
  
  
  
  
  int sock, num, ecode;
  struct sockaddr_in *tcpServer;
  struct addrinfo tcpHints, *tcpAddrlist;
  //socklen_t length;
  
  int udpSock, udpEcode;
  struct sockaddr_in *udpServer, udpPeer;
  struct addrinfo udpHints, *udpAddrlist;
  
  char pwordCorrect;
  
  //change this if later to check for -q
  if (argc >= 2 && (strcmp(argv[1], "-q") == 0)){
    //printf("in query mode\n");
    if (strcmp(argv[1], "-q") == 0){
      //printf("UDP!\n");
      memset( &udpHints, 0, sizeof(udpHints));
      udpHints.ai_family = AF_INET; udpHints.ai_socktype = SOCK_DGRAM;
      udpHints.ai_flags = AI_NUMERICSERV; udpHints.ai_protocol = 0;
      udpHints.ai_canonname = NULL; udpHints.ai_addr = NULL;
      udpHints.ai_next = NULL;
      
      //grab host and udp port
      char* hostname;
      hostname =  malloc((LINE_MAX* sizeof(char))+1);
      char* udpPortNum;
      udpPortNum =  malloc((LINE_MAX* sizeof(char))+1);
      
      FILE *file;
      file = fopen("hostnport.txt", "r");
      int t = 0;
      int m= 0;
      char oneChar;
      oneChar = fgetc(file);
      
      //grab host
      while (oneChar != ' '){
	hostname[t] = oneChar;
	t++; 
	oneChar = fgetc(file);
      }
      //skip to UDP port
      while (oneChar != ','){
	oneChar = fgetc(file);
      }
      oneChar = fgetc(file);
      while (oneChar != '.'){
	udpPortNum[m] = oneChar;
	m++;
	oneChar = fgetc(file);
      }
      
      //printf("Host name is %s. Port name is %s.\n", hostname, udpPortNum);
      udpEcode = getaddrinfo(hostname, udpPortNum, &udpHints, &udpAddrlist);
      if (udpEcode != 0) {
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(udpEcode));
	exit(1);
      }
      
      udpServer = (struct sockaddr_in *) udpAddrlist->ai_addr;
      
      //create Socket
      if ( (udpSock = socket( udpAddrlist->ai_family, udpAddrlist->ai_socktype, 0 )) < 0 ) {
	perror("inet_wstream:socket");
	exit(1);
      }
      
      socklen_t lengthUDP;
      lengthUDP = sizeof(udpPeer);
      
      
      //printf("before connect\n");
      char* msg;
      msg =  malloc((LINE_MAX* sizeof(char))+1);
      msg = "Give me the query please.\n";
      //connect
      
      //printf("msg here is %u\n", ntohl(htonl(getpid()))); 
      //printf("msg here is %s\n", msg);
      int pp = 0;
      while (msg[pp] != '\0'){
	if ( sendto(udpSock, &msg[pp], sizeof(msg[pp]), 0, (struct sockaddr *)udpServer, sizeof(struct sockaddr_in)) < 0) {
	  perror("inet_wstream:connect");
	  exit(1);
	}
	pp++;
      }
      
      
      //check if password mode
      int counterp = 0;
      char p;
      char* password;
      password =  malloc((LINE_MAX* sizeof(char))+1);
      while (p != '\n'){
	if (recvfrom(udpSock, &p, sizeof(p), 0, (struct sockaddr *)&udpPeer, &lengthUDP) < 0){
	  perror("recv_udp:recvfrom");
	  exit(1);
	}
	password[counterp] = p;
	counterp++;
      }
      //printf("password in datagram is .%s.\n", password);
      
      char pwordCorrect;
      //if password mode entered
      if (strcmp(password, "\n") != 0){
	if (argc <= 3 || strcmp(argv[2], "-p") != 0){
	  fprintf(stderr, "Password Mode Required. Please Enter a Password.\n");
	  pwordCorrect = '0';
	  
	  //tell server wrong password was entered
	  if((sendto(udpSock, &pwordCorrect, sizeof(pwordCorrect), 0, (struct sockaddr *)udpServer, sizeof(struct sockaddr_in))) < 0) {
	    perror("inet_wstream:write");
	    exit(1);
	  }
	  
	  exit(0);
	}
	else{
	  //remove \n
	  password[strlen(password)-1] = '\0';
	  //if wrong password entered
	  if (strcmp(argv[3], password) != 0){
	    fprintf(stderr, "Password Entered is Incorrect.\n");
	    
	    pwordCorrect = '0';
	  //tell server wrong password was entered
	    if((sendto(udpSock, &pwordCorrect, sizeof(pwordCorrect), 0, (struct sockaddr *)udpServer, sizeof(struct sockaddr_in))) < 0) {
	      perror("inet_wstream:write");
	      exit(1);
	    }
	  exit(0);
	  }
	}
      }
      
      //tell server correct pword entered
      pwordCorrect = '1';
      if((sendto(udpSock, &pwordCorrect, sizeof(pwordCorrect), 0, (struct sockaddr *)udpServer, sizeof(struct sockaddr_in))) < 0) {
	perror("inet_wstream:write");
	exit(1);
      }
      
      char* msgRec;
      msgRec =  malloc((LINE_MAX* sizeof(char))+1);
      char cha;
      pp = 0;
      printf("PLAYERS CONNECTED TO THE SERVER:\n\n");
      while (cha != '\t'){
	
	if ((recvfrom(udpSock, &cha, sizeof(cha), 0, (struct sockaddr *)&udpPeer, &lengthUDP)) < 0) {
	  perror("recv_udp:recvfrom");
	  exit(1);
	}
	//printf("cha in loop is %c\n", cha);
	msgRec[pp] = cha;
	pp++;
      }
      printf("%s\n", msgRec);
    }
    else{
      fprintf(stderr,"Please enter -q to query the server or no arguments to play.\n");
    }
  }
  
  else{
    //printf("in TCP SOCKET MODe\n");
    //////FOR TCP SOCKET////////////////////////////////////////////////////////////////
    memset( &tcpHints, 0, sizeof(tcpHints));
    tcpHints.ai_family = AF_INET; tcpHints.ai_socktype = SOCK_STREAM;
    tcpHints.ai_flags = AI_NUMERICSERV; tcpHints.ai_protocol = 0;
    tcpHints.ai_canonname = NULL; tcpHints.ai_addr = NULL;
    tcpHints.ai_next = NULL;
    
    //grab host and port
    
    char* hostname;
    hostname =  malloc((LINE_MAX* sizeof(char))+1);
    char* portNum;
    portNum =  malloc((LINE_MAX* sizeof(char))+1);
    
    
    FILE *file;
    file = fopen("hostnport.txt", "r");
    int t = 0;
    int m= 0;
    char oneChar;
    oneChar = fgetc(file);
    
    while (oneChar != ' '){
      hostname[t] = oneChar;
      t++; 
      oneChar = fgetc(file);
    }
    oneChar = fgetc(file);
    while (oneChar != ','){
      portNum[m] = oneChar;
      m++; 
      oneChar = fgetc(file);
    }
    //printf("host n port: %s %s\n", hostname, portNum);
    ///////////////
    
    //first two arguments will have to be read from file
    ecode = getaddrinfo(hostname, portNum, &tcpHints, &tcpAddrlist);
    if (ecode != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ecode));
      exit(1);
    }
    
    tcpServer = (struct sockaddr_in *) tcpAddrlist->ai_addr;
    
    /*
     *  Create the socket.
     */
    if ( (sock = socket( tcpAddrlist->ai_family, tcpAddrlist->ai_socktype, 0 )) < 0 ) {
      perror("inet_wstream:socket");
      exit(1);
    }
    
    if ( connect(sock, (struct sockaddr *)tcpServer, sizeof(struct sockaddr_in)) < 0) {
      perror("inet_wstream:connect");
      exit(1);
    }
    //printf("before read of pword\n");
    int counterp=0; 
    char* password;
    password =  malloc((LINE_MAX* sizeof(char))+1);
    while (read(sock, &password[counterp], 1) == 1){
      // printf("reading..\n");
      if (password[counterp] == '\n'){
	//printf("breaking\n");
	break;
      }
      counterp++;
    }
    
    //printf("password in client is .%s.\n", password);
    
    //if password mode entered
    if (strcmp(password, "\n") != 0){
      //printf("server should be using password\n");
      if (argc <= 2 || strcmp(argv[1], "-p") != 0){
	fprintf(stderr, "Password Mode Required Please Enter a Password.\n");
	pwordCorrect = '0';
	
	//tell server wrong password was entered
	if((write(sock, &pwordCorrect, 1)) < 0) {
	  perror("inet_wstream:write");
	  exit(1);
	}
	
	exit(0);
      }
      else{
	//remove \n
	password[strlen(password)-1] = '\0';
	//if wrong password entered
	if (strcmp(argv[2], password) != 0){
	  fprintf(stderr, "Password Entered is Incorrect.\n");
	  
	  pwordCorrect = '0';
	  //tell server wrong password was entered
	  if((write(sock, &pwordCorrect, 1)) < 0) {
	    perror("inet_wstream:write");
	    exit(1);
	  }
	  exit(0);
	}
      }
    }
    
    //tell server correct pword entered
    pwordCorrect = '1';
    if((write(sock, &pwordCorrect, 1)) < 0) {
	    perror("inet_wstream:write");
	    exit(1);
	  }
    
    
    
    //get handle
    char* handle;
    handle =  malloc((LINE_MAX* sizeof(char))+1);
    printf("What's your Handle?\n");
    fgets(handle, 21, stdin);
    //add a return so the read in nim server knows when to stop
    //strcat(handle, "\n");
    //printf("handle is %s.", handle);
    
    printf("Finding an opponent...\n");
    
    /*
     *  Connect to data socket on the peer at the specified Internet
     *  address.
     */
   // if ( connect(sock, (struct sockaddr *)tcpServer, sizeof(struct sockaddr_in)) < 0) {
    //  perror("inet_wstream:connect");
   //   exit(1);
   // }
    
    
    
    //send handle to server
    int countNum;
    int ee = 0;
    int loopNum = strlen(handle);
    while(ee<loopNum){
      //printf("handle writing is %c\n", handle[ee]);
      if((countNum = write(sock, &handle[ee], 1)) < 0) {
	perror("inet_wstream:write");
	exit(1);
      }
      ee++;
    }
    
    //read in player number
    char c;
    //printf("sdfasd\n");
    read(sock, &c, 1);
    //printf("Player number is %c\n", c);
    //printf("after reading playnum\n");
    //printf("before reading other handle\n");
    
    
    //read other player's handle
    int counter=0; 
    char* otherHand;
    otherHand =  malloc((LINE_MAX* sizeof(char))+1);
    while (read(sock, &otherHand[counter], 1) == 1){
      // printf("reading..\n");
      if (otherHand[counter] == '\n'){
	//printf("breaking\n");
	break;
      }
      counter++;
    }
    otherHand[strlen(otherHand)-1] = '\0';
    //printf("after reading other handle\n");
    //one opponent is found, print board, and other player's handle
    printf("Opponent Found!\n");
    
    //prints board
    int q = 0;
    int i= 0;
    for (i=0;i<4;i++){
      printf("%d| ", i+1);
      for (q=0;q<7;q++){
	printf("%s ", board[i][q]);      
      }
      printf("\n");
    }
    printf(" +---------------\n");
    printf("   1 2 3 4 5 6 7\n");
    //////////
    
    printf("Your Handle: %s", handle);
    printf("Your Opponent: %s\n", otherHand);
    
    
    
    
    
    
    
    //flag for determining first move
    int initial = 1;
    
    
    //change this later for when game is over
    while(1){
      //printf("initial is %d\n", initial);
      //prompt for move
      int rowMove;
      int colMove;
      
      
      //if its the first persons turn and the first move (or not the first move)
      if(!(c=='2' && initial == 1)){
	//printf("in if statement---------------\n");
	while(1){
	  
	  char* initEntry;
	  initEntry =  malloc((LINE_MAX* sizeof(char))+1);
	  
	  printf("What's your move?\n");
	  fgets(initEntry, 21, stdin);
	  if (strlen(initEntry) != 4){
	    printf("Please Enter Valid input.\n");
	    continue;
	  }
	 
	  //if (scanf("%d%c%d", &rowMove, &term, &colMove) != 3 || term != ' '){
	    //printf("Please Enter Valid input\n");
	 // continue;
	//}
	
	rowMove = atoi(&initEntry[0]);
	colMove = atoi(&initEntry[2]);
	
	//printf("rowmove is .%d.\n", rowMove);
	//printf("colmove is .%d.\n", colMove);
	
	if (rowMove == 0 && colMove != 0){
	  printf("Please enter valid input.\n");
	  continue;
	}
	if (rowMove != 0 && colMove == 0){
	  printf("Please enter valid input.\n");
	  continue;
	}
	//check for ints in range
	
	//printf("before checks\n");
	if (rowMove>=0 && rowMove<=4 && colMove>=0 && colMove <= 7){
	  //printf("board spot is %s\n", board[rowMove-1][colMove-1]);
	  if (rowMove == 0 && colMove > 0)
	    
	    printf("Please enter a valid move\n");
	  if (colMove == 0 && rowMove > 0)
	     printf("Please enter a valid move\n");
	  
	  //if the move is not 0 0
	  if (rowMove != 0 && colMove != 0){
	    if( strcmp(board[rowMove-1][colMove-1], "x") == 0)
	      break;
	    else
	      printf("Please enter a valid move\n");
	  }
	  // if the move is 0 0
	  else
	    break;
	}
	else
	  printf("Please enter valid integers in the correct range\n");
      }
      //////////
      
      //printf("after checks\n");
      
      char rowMoveStr[15];
      sprintf(rowMoveStr, "%d", rowMove);
      
      char colMoveStr[15];
      sprintf(colMoveStr, "%d", colMove);
      
      char* sourceStr;
      sourceStr =  malloc((LINE_MAX* sizeof(char))+1);
      //fprintf(stdout, "source string is %s\n", sourceStr);
      strcat(sourceStr, rowMoveStr);
      //fprintf(stdout, "source string is %s\n", sourceStr);
      strcat(sourceStr, " ");
      //fprintf(stdout, "source string is %s\n", sourceStr);
      strcat(sourceStr, colMoveStr);
      
      //left = sizeof(sourceStr);
      
      //printf("outside if row move\n");
      //if player didn't resign
      if(rowMove != 0){
	 //printf("inside if row move\n");
	
	//change board
	int g;
	int b;
	for (g=rowMove-1, b=colMove-1; strcmp(board[g][b], "0") != 0; b++){
	  //printf("g is %d, b is %d\n", g, b);
	  //printf("board[g][b] is %s\n", board[g][b]);
	  board[g][b] = "0";
	  if (b==6)
	    break;
	}
	
	//prints board
	for (i=0;i<4;i++){
	  printf("%d| ", i+1);
	  for (q=0;q<7;q++){
	    printf("%s ", board[i][q]);      
	  }
	  printf("\n");
	}
	printf(" +---------------\n");
	printf("   1 2 3 4 5 6 7\n");
	//////////
      }
      // fprintf(stdout, "source string is %s\n", sourceStr);
      int d = 0;
      
      //printf("source string is %s\n", sourceStr);
      while(sourceStr[d] != '\0'){
	//printf("sourceStr[d] is %c\n", sourceStr[d]);
	if((num = write(sock, &sourceStr[d], 1)) < 0) {
	  perror("inet_wstream:write");
	  exit(1);
	}
	d++;
      }
      
      if (strcmp(sourceStr, "0 0") == 0){
	printf("You Resigned. Game Over\n");
	exit(0);
	
      }
      //Check if the game is over (lost cause you made last move)
      int gameOver = 1;
      int ro, co;
      for(ro = 0; ro < 4; ro++) {
	for(co = 0; co < 7; co++){
	  if (strcmp(board[ro][co], "x") == 0)
	    gameOver = 0;
	}
      }
      if (gameOver == 1){
	printf("You lost!\n");
	break;
      }
      /////////////////////////////
    }
    //printf("inbetween\n");
    
    //read move into move made
    char ch;
    printf("Waiting for other play to make a move...\n");
    char* moveMade;
    moveMade =  malloc((LINE_MAX* sizeof(char))+1);
    read(sock, &ch, 1);
    moveMade[0] = ch;
    read(sock, &ch, 1);
    moveMade[1] = ch;
    read(sock, &ch, 1);
    moveMade[2] = ch;
    
    //printf("move made is %s\n", moveMade);
    
    //place read move into variables so the board can be changed
    char* rowMoveChange;
    rowMoveChange =  malloc((LINE_MAX* sizeof(char))+1);
    char* colMoveChange;
    colMoveChange =  malloc((LINE_MAX* sizeof(char))+1);
    rowMoveChange[0] = moveMade[0];
    colMoveChange[0] = moveMade[2];
    //printf("row move change is %s\n", rowMoveChange);
    //printf("col move change is %s\n", colMoveChange);
    
    int rowMoveChangeInt = atoi(rowMoveChange);
    int colMoveChangeInt = atoi(colMoveChange);
    
    if (rowMoveChangeInt == 0 && colMoveChangeInt == 0){
      printf("Your opponent resigned. Game Over\n");
      exit(0);
    }
    
    //change board
    int g;
    int b;
    for (g=rowMoveChangeInt-1, b=colMoveChangeInt-1; strcmp(board[g][b], "0") != 0; b++){
      //printf("g is %d, b is %d\n", g, b);
      //printf("board[g][b] is %s\n", board[g][b]);
      board[g][b] = "0";
      if (b==6)
	break;
    }
    
    
    //prints board
    for (i=0;i<4;i++){
      printf("%d| ", i+1);
      for (q=0;q<7;q++){
	printf("%s ", board[i][q]);      
      }
      printf("\n");
    }
    printf(" +---------------\n");
    printf("   1 2 3 4 5 6 7\n");
    //////////
    
    
    //Check if the game is over (won cause opponent made last move)
    int gameOverw = 1;
    int ro2, co2;
    for(ro2 = 0; ro2 < 4; ro2++) {
      for(co2 = 0; co2 < 7; co2++){
	if (strcmp(board[ro2][co2], "x") == 0)
	  gameOverw = 0;
      }
    }
    if (gameOverw == 1){
      printf("You won!\n");
      break;
    }
    ///////////
    
    initial = 0;
  }
  
  // printf("UH OH WRITING QQQQQQQQQQQQQQQQQQQQQQQQQQQQq\n");
  //write flag to signify to nms that game is over
  if((countNum = write(sock, "q", 1)) < 0) {
    perror("inet_wstream:write");
    exit(1);
  }
  if((countNum = write(sock, "q", 1)) < 0) {
    perror("inet_wstream:write");
    exit(1);
  }
  ////////////////////////
  
  
  /*strcat(otherHand, "\n");
   *   printf("otherHand to detete to send is %s\n", otherHand);
   *   ee = 0;
   *   loopNum = strlen(otherHand);
   *   while(ee<loopNum){
   *     printf("otherHand to detete {EE} to send is %c\n", otherHand[ee]);
   *     if((countNum = write(sock, &otherHand[ee], 1)) < 0) {
   * perror("inet_wstream:write");
   * exit(1);
   *   }
   *   ee++;s
   * }*/
  /*
   *   ee = 0;
   *   loopNum = strlen(otherHandle);
   *   while(ee<loopNum){
   *     //printf("handle writing is %c\n", handle[ee]);
   *     if((countNum = write(sock, &otherHandle[ee], 1)) < 0) {
   * perror("inet_wstream:write");
   * exit(1);
   * }
   * ee++;
   } **/
  
  
   }
   return 0;
   }