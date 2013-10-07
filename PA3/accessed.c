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

//Joseph Acosta
//accessed.c

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
int num;
struct list idList;
int idListSize = 0;
int once = 0;
int first = 1;

int main(int argc, char *argv[]){
  initList(&idList);
  
  if (argc != 2)
  {
    fprintf(stderr, "Error. Please enter one argument\n");
    exit(1);
  }
  num = atoi(argv[1]);
  struct stat info;
  
  char* inputData;
  inputData = malloc ((LINE_MAX* sizeof(char))+1);
  
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

  //count number of chars in inputData
  int j;
  int charCount = 0;
  for (j=0; inputData[j] != '\0'; j++){
    charCount++;
    
  }

  //while loop to separate file names on white space
  int i = 0;

  while(i<charCount){
    
    char* fileName;
    fileName = malloc ((LINE_MAX* sizeof(char))+1);
    
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
      double secondsAgo = time(NULL) - info.st_atime; 

      double daysAgo = secondsAgo/60/60/24;

      int addFlag = 1;
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

	    if (num > 0){

	      //if it was not accessed for num days
	      if (daysAgo > num){
		//fprintf(stderr, "HEEHER\n");
		printf("%s\n", fileName);}
	    }
	    //if num is negative
	    if (num<0){

	      //if it was accessed within num
	      if (daysAgo <= (num*-1))
		printf("%s\n", fileName);
	    }
	    //if num is 0
	    if (num==0)
	    {
	      fprintf(stderr, "Please enter a valid argument\n");
	      exit(1);
	    }
	  }

      }
      if (first == 1){
	if (num > 0){

	      //if it was not accessed for num days
	      if (daysAgo > num){

		printf("%s\n", fileName);}
	    }
	    //if num is negative
	    if (num<0){
	      //if it was accessed within num
	      if (daysAgo <= (num*-1))
		printf("%s\n", fileName);
	    }
	    //if num is 0
	    if (num==0)
	    {
	      fprintf(stderr, "Please enter a valid argument\n");
	      exit(1);
	    }
	
      }
      push(&idList, deviceID);
      idListSize++;
      first = 0;
      
      //if the num inputted was positive
      
    }
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
