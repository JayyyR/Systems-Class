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
#include <signal.h>
#include <sys/wait.h>
//Joseph Acosta
//totalsize.c


//structs
struct node {
  char* data;
  struct node *next;
};

struct list {
  struct node *start;
  struct node *end;
};

//Declarations
int initList(struct list *sList);
int push(struct list *sList, char* data);
char* grab(struct list *sList);
int emptyCheck(struct list *sList);
int pop(struct list *sList);
int print(struct list *sList);

//globals
struct list idList;
int idListSize = 0;
int once = 0;
int first = 1;


int main(int argc, char *argv[]){
  
  initList(&idList);
  //printf("here\n");
  struct stat info;
  char* inputData;
  inputData = malloc ((LINE_MAX* sizeof(char))+1);
  
  char* units;
  units = malloc ((LINE_MAX* sizeof(char))+1);
  
  char* mompid;
  mompid = malloc ((LINE_MAX* sizeof(char))+1);
  
  char* sleepTime;
  sleepTime = malloc ((LINE_MAX* sizeof(char))+1);
  
  //grab file names
  //int end = 0;
  
  //use fgetc to grab file names to output to stdout
  //make sure stdout doesn't have newlines and is separated by whitespace
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
 
  int j;
  int charCount = 0;
  for (j=0; inputData[j] != '\0'; j++){
    charCount++;
  }

  //while loop to separate file names on white space
  int i = 0;
  long totalBytes = 0;
  while(i<charCount){
    
    char* fileName;
    fileName = malloc ((LINE_MAX* sizeof(char))+1);
    
    sleepTime = getenv("TSTALL");

    
    if (sleepTime != NULL){
      
 
      int validInt = 1;
      
      
      //check if valid integer
      int c;
      int tStallCount = 0;
      
      
      //count chars in sleepTime
      for (c=0; sleepTime[c] !='\0'; c++){
	
	tStallCount++;
      }
  
      if (!sleepTime){
	
	validInt = 0;
	break;
      }
      
      //Check for non-digit chars in the rest of the stirng.
      //loop through sleep time to make sure each char is a digit
      int v = 0;
      while (v<tStallCount)
      {
	if (!isdigit(sleepTime[v])){
	 
	  validInt = 0;
	  break;
	}
	else
	  v++;
      }
    
      
      
      //if it is a positive integer, sleep for tha tmany seconds
      if (validInt == 1){
	//printf("here\n");
	int sleepInt = atoi(sleepTime);
	
	sleep(sleepInt);
      }
      //dont sleep
      else{}
	
    }
    
    int q;
    for (q=0; !isspace(inputData[i]) && i<charCount; q++, i++){
      fileName[q] = inputData[i];
      
    }
    i++;
    
    //Calculate Times
    if (stat(fileName, &info) != 0) { 
     
    }
    
    if (S_ISREG(info.st_mode)){
      char* deviceID;
      deviceID = malloc ((LINE_MAX* sizeof(char))+1);
      sprintf(deviceID, "%d/%d/%ld", major(info.st_dev),minor(info.st_dev),info.st_ino);
     
      //calculate how long ago in days hte file was accessed
      long bytes = info.st_size; 
      
      //create flag to know whether or not you should add to totalbytes
      int addFlag = 1;
      //run through linked list, make sure id's aren't the same
      //as the current file you're checking
     
      int g = 0;
     
      
      
      if (first!=1){
	//pop off zero
	if (once == 0){
	 
	  pop(&idList);
	  once++;}
	  
	  while (g<idListSize){
	  
	    if (strcmp(deviceID, grab(&idList))==0){
	     
	      addFlag = 0;
	    }
	    push(&idList, grab(&idList));
	    pop(&idList);
	    
	    g++;
	  }
	  if (addFlag == 1){
	   
	    totalBytes += bytes;
	  }
	 
      }
      //if its the first file add it to the bytes
      if(first==1){
	totalBytes += bytes;
      }
      //push next file onto list, increase list size, first flag set to 0
      push(&idList, deviceID);
      idListSize++;
      first = 0;
      
    }
  }
  
  //check the units to be outputted
  units = getenv("UNITS");
  //set process ID to TMOM and grab it
 
  mompid = getenv("TMOM");
  if (mompid != NULL){
    int intpid = atoi(mompid);
    
    
    
    //if tmom is a positive int then send kill signal
    if (intpid>0){
      //wait();
      //fprintf(stderr, "here\n");
      sleep(3); kill(intpid,SIGUSR1);
    }
  }
 
  if (units != NULL){
    if((strcmp(units, "k") == 0) || (strcmp(units, "K") == 0)){
      totalBytes = totalBytes/1000;
      printf("%ldkB\n", totalBytes);
    
    }
    else{
      printf("%ld\n", totalBytes);
     
    }
  }
  else{
    printf("%ld\n", totalBytes);
    
  }
  return 0;
}


int initList(struct list *sList){
  struct node *p;
  p = malloc(sizeof(struct node));
  p->data = (long) 0;
  p->next = NULL;
  sList->start = p;
  sList->end = p;
  
  return 0;
}

int push(struct list *sList, char* data){
  struct node *p;
  p = malloc(sizeof(struct node));
  p->data = data;
  sList ->end->next = p;
  
  
  sList->end = p;
  
  
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