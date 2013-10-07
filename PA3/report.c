#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h> 
#include <unistd.h>
#include <errno.h> 
#include <sys/sysmacros.h> 
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>


//Joseph Acosta
//report.c
void usr1handler();

int report2accessor[2];
int report2accessorNeg[2];
int accessor2totalsize[2];
int accessor2totalsizeNeg[2];
int totalsize2report[2];
int totalsize2reportNeg[2];
int kflag = 0;
int flag=1;
int wrongDirect = 0;
int tempnum;

char* num;
char* negnum;

void usr1handler(){
  flag=0;}

int main(int argc, char *argv[]){
  num = malloc ((LINE_MAX* sizeof(char))+1);

  //grab args from command line
  if (argc == 1)
  {
    fprintf(stderr, "Error. Please enter arguments\n");
    exit(1);
  }
  num = argv[1];
  int tempnum = atoi(num);
  
  if (tempnum == 0){
    fprintf(stderr, "Error. Please enter a valid arguments\n");
    exit(1);
  }
  
  //3 arguments
  if (argc==3){
    if (strcmp(argv[2], "-k") == 0 || strcmp(argv[2], "-K") == 0 ){
      kflag = 1;
      putenv("UNITS=k");
    }
    else{
      fprintf(stderr, "Error. Please enter valid arguments\n");
      exit(1);
    }
  }
  
  //4 arguments just -d
  if (argc==4){
    
    if (strcmp(argv[2], "-d") == 0 || strcmp(argv[2], "-D") == 0){
      
      int m;
      for(m = 0; m < strlen(argv[3]); m++)
      {
	if(isdigit(argv[3][m]) == 0)
	  break;
      }
      if(m != strlen(argv[3])){
	fprintf(stderr, "Please enter valid integer value following -d\n");
	exit(1);
      }
      
      char* newStall;
      newStall = malloc ((LINE_MAX* sizeof(char))+1);
      strcpy(newStall,"TSTALL=");

      strcat(newStall, argv[3]);
      

      putenv(newStall);
    }
    else{
      fprintf(stderr, "Error. Please enter valid arguments\n");
      exit(1); 
    }
    
  }
  
  
  if (argc==5){
    
    if (strcmp(argv[2], "-k") == 0 || strcmp(argv[2], "-K") == 0){
      kflag = 1;
      putenv("UNITS=k");
    }
    if (strcmp(argv[4], "-k") == 0 || strcmp(argv[4], "-K") == 0){
      kflag = 1;
      putenv("UNITS=k");
    }
    if (strcmp(argv[2], "-d") == 0 || strcmp(argv[2], "-D") == 0){
      
      
      int m;
      for(m = 0; m < strlen(argv[3]); m++)
      {
	if(isdigit(argv[3][m]) == 0)
	  break;
      }
      if(m != strlen(argv[3])){
	fprintf(stderr, "Please enter valid integer value following -d\n");
	exit(1);
      }
      char* newStall;
      newStall = malloc ((LINE_MAX* sizeof(char))+1);
      strcpy(newStall,"TSTALL=");

      strcat(newStall, argv[3]);
      

      putenv(newStall);
    }
    if (strcmp(argv[3], "-d") == 0 || strcmp(argv[3], "-D") == 0){
      
      int m;
      for(m = 0; m < strlen(argv[4]); m++)
      {
	if(isdigit(argv[4][m]) == 0)
	  break;
      }
      if(m != strlen(argv[4])){
	fprintf(stderr, "Please enter valid integer value following -d\n");
	exit(1);
      }
      
      char* newStall;
      newStall = malloc ((LINE_MAX* sizeof(char))+1);
      strcpy(newStall,"TSTALL=");

      strcat(newStall, argv[4]);
      

      putenv(newStall);
    }
    
  }
  
  //set TMOM to current PID
  int processID = getpid();
  char* procID;
  procID = malloc ((LINE_MAX* sizeof(char))+1);
  sprintf(procID, "%d", processID);
  char* newTMOM;
  newTMOM = malloc ((LINE_MAX* sizeof(char))+1);
  strcpy(newTMOM,"TMOM=");

  strcat(newTMOM, procID);
  putenv(newTMOM);
  
  
  
  //define pipe
  pipe(report2accessor);
  pipe(report2accessorNeg);
  pipe(accessor2totalsize);
  pipe(accessor2totalsizeNeg);
  pipe(totalsize2report);
  pipe(totalsize2reportNeg);
  
  //defined data for writing stdin to programs
  char* inputData;
  inputData = malloc ((LINE_MAX* sizeof(char))+1);
  //fgets(inputData, LINE_MAX, stdin);
  
  
  //first child feeds positive num to accessed
  if (fork() == 0) {

    
    dup2(report2accessor[0], 0);
    
    //dup standard output of this environment to pipe for totalsize
    dup2(accessor2totalsize[1],1);
    
    close(accessor2totalsizeNeg[0]);
    close(accessor2totalsizeNeg[1]); 
    close(report2accessorNeg[1]);
    close(report2accessorNeg[0]);  
    close(totalsize2reportNeg[0]);
    close(totalsize2reportNeg[1]);
    
    
    //close everything you dont need
    close(accessor2totalsize[0]);
    close(accessor2totalsize[1]); 
    close(report2accessor[1]);
    close(report2accessor[0]); 
    close(totalsize2report[0]);
    close(totalsize2report[1]);
    
    
    if (execl("./accessed", "./accessed", num, NULL) == -1)
    {fprintf(stderr, "Error: In incorrect directory1\n");   wrongDirect = 1;  exit(1);
    }
    
    
  }
  //second child feeds negative num to accessed
  if (fork() == 0) {

    negnum = malloc ((LINE_MAX* sizeof(char))+1);
    
    int intnum = atoi(num);
    intnum = intnum *-1;
    sprintf(negnum, "%d", intnum);

    
    
    dup2(report2accessorNeg[0], 0);
    //dup standard output of this environment to pipe for totalsize
    dup2(accessor2totalsizeNeg[1],1);
    
    
    close(accessor2totalsize[1]);
    close(accessor2totalsize[0]); 
    close(report2accessor[1]);
    close(report2accessor[0]);
    close(totalsize2report[0]);
    close(totalsize2report[1]);
    
    
    close(accessor2totalsizeNeg[0]);
    close(accessor2totalsizeNeg[1]); 
    close(report2accessorNeg[1]);
    close(report2accessorNeg[0]);  
    close(totalsize2reportNeg[0]);
    close(totalsize2reportNeg[1]);
    if (execl("./accessed", "./accessed", negnum, NULL)== -1){
      fprintf(stderr, "Error: In incorrect directory2\n");   wrongDirect = 1; exit(1);} 

  }
  //3rd child for accessed to totalsize
  if (fork() == 0) {

    dup2(accessor2totalsize[0], 0);
    //dup standard output of this environment to pipe for report
    dup2(totalsize2report[1],1);
    
    
    close(accessor2totalsizeNeg[0]);
    close(accessor2totalsizeNeg[1]); 
    close(report2accessorNeg[1]);
    close(report2accessorNeg[0]);  
    close(totalsize2reportNeg[0]);
    close(totalsize2reportNeg[1]);
    
    
    close(accessor2totalsize[1]);
    close(accessor2totalsize[0]); 
    close(report2accessor[1]);
    close(report2accessor[0]);
    close(totalsize2report[0]);
    close(totalsize2report[1]); 

    if (execl("./totalsize", "./totalsize", num, NULL)== -1){
      fprintf(stderr, "Error: In incorrect directory\n");   wrongDirect = 1; exit(1);}

  }
  //4th child for neg accessed to totalsize
  if (fork() == 0) {

    
    dup2(accessor2totalsizeNeg[0], 0);
    dup2(totalsize2reportNeg[1],1);
    
    

    close(accessor2totalsize[1]);
    close(accessor2totalsize[0]); 
    close(report2accessor[1]);
    close(report2accessor[0]);
    close(totalsize2report[0]);
    close(totalsize2report[1]); 
    
    
    //close stuff you don't need
    close(accessor2totalsizeNeg[1]);
    close(accessor2totalsizeNeg[0]); 
    close(report2accessorNeg[1]);
    close(report2accessorNeg[0]);
    close(totalsize2reportNeg[0]);
    close(totalsize2reportNeg[1]); 
    if (execl("./totalsize", "./totalsize", num, NULL) == -1){
      fprintf(stderr, "Error: In incorrect directory4\n");   wrongDirect = 1; exit(1);}
  }
  //parent
  else{
    
    int e = 0;
  char oneLet;
  oneLet = fgetc(stdin);
  while (oneLet != EOF){

    if (oneLet != '\n'){
      inputData[e] = oneLet;
      e++;
    }
    else{
      inputData[e] = ' ';
      e++;
    }

    
    oneLet = fgetc(stdin);
  }

  
  write(report2accessor[1], inputData, (LINE_MAX* sizeof(char))+1);
  write(report2accessorNeg[1], inputData, (LINE_MAX* sizeof(char))+1);
    
    //close everything you don't need
    close(totalsize2report[1]);
    close(accessor2totalsize[1]);
    close(accessor2totalsize[0]);
    close(report2accessor[1]);
    close(report2accessor[0]);
    close(totalsize2reportNeg[1]);
    close(accessor2totalsizeNeg[1]);
    close(accessor2totalsizeNeg[0]);
    close(report2accessorNeg[1]);
    close(report2accessorNeg[0]);
    
    signal(SIGUSR1, usr1handler);
    while(flag){
      sleep(1);
      write(1, "*", 1);
    }
    fprintf(stdout, "\n");
    
    
    char* posData;
    posData = malloc ((LINE_MAX* sizeof(char))+1);
    read(totalsize2report[0], posData, (LINE_MAX* sizeof(char))+1);
;
    
    char* newposData;
    newposData = malloc ((LINE_MAX* sizeof(char))+1);
    //remove newline
    int u = 0;
    while(posData[u] != '\n'){
      newposData[u] = posData[u];
      u++;
    }

    char* negData;
    negData = malloc ((LINE_MAX* sizeof(char))+1);
    read(totalsize2reportNeg[0], negData, (LINE_MAX* sizeof(char))+1);
    
    char* newnegData;
    newnegData = malloc ((LINE_MAX* sizeof(char))+1);
    //remove newline
    int f = 0;
    while(negData[f] != '\n'){
      newnegData[f] = negData[f];
      f++;
    }
     
   
    
    
    
    if (kflag == 0)
      fprintf(stdout, "A total of %s bytes are in regular files not accessed for %s days\n", newposData, num);
    else
      fprintf(stdout, "A total of %s are in regular files not accessed for %s days\n", newposData, num);
    fprintf(stdout, "----------------------------------------------------------------------\n");
    if (kflag==0)
      fprintf(stdout, "A total of %s bytes are in regular files accessed within %s days\n", newnegData, num);
    else
      fprintf(stdout, "A total of %s are in regular files accessed within %s days\n", newnegData, num);
  }
  
  return 0;
}