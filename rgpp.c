#define _GNU_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>



//Joseph Acosta
//Program 2
//CS 415

//structs
struct node {
  char* data;
  struct node *next;
};

struct list {
  struct node *start;
  struct node *end;
};

//function declarations
int initList(struct list *sList);
int push(struct list *sList, char* data);
char* grab(struct list *sList);
int emptyCheck(struct list *sList);
int pop(struct list *sList);
int print(struct list *sList);
struct node* clone(struct node *sNode);
int grabGrep();
int openFile();
int wordModeTest();


  

//globals
int lineMode = 0;
int wordMode = 0;
int bannerMode = 0;
int numberedMode = 0;
char* wordModeWord;
int matchCount = 0;
int wordCount = 0;
//define linkedList
  struct list fileList;
  struct list cloneList;





int main(int argc, char *argv[]){
  
  if (argc == 1){
    printf("Error.\n");
    exit(1);
  }
  
  //check what mode user wants
  if (strcmp(argv[1], "-l") == 0){
    lineMode = 1;}
  else if (strcmp(argv[1], "-w") == 0){
    wordMode = 1;}
  else{
    printf("Please enter valid input, first argument must be '-l' or '-w'\n");
    exit(1);
  }
  //set input arguments correctly
  if (lineMode == 1){
    
    if (argc == 3){
      if (strcmp(argv[2], "-b") == 0)
	bannerMode = 1;
      else if (strcmp(argv[2], "-n") == 0)
	numberedMode = 1;
      else{
	printf("Please enter valid input\n");
	exit(1);
      }
    }
    
    if (argc == 4){
      if (strcmp(argv[2], "-b") == 0 && strcmp(argv[3], "-n") == 0){
	bannerMode = 1;
	numberedMode = 1;
      }
      else if (strcmp(argv[2], "-n") == 0 && strcmp(argv[3], "-b") == 0){
	numberedMode = 1;
	bannerMode = 1;
      }
      else{
	printf("Please enter valid input\n");
	exit(1);
      }
    }
    if (argc>4){
      printf("Too many arguments. Please enter valid input\n");
      exit(1);
    }
  }
  
  if (wordMode == 1){
    
    //set wordModeWord to the argument
    //you are malloc-ing the size of a char multiplied by the length of the input word
    //plus 1 for the null byte
    wordModeWord = malloc((sizeof(char)*strlen(argv[2]))+1);
    strcpy(wordModeWord, argv[2]);

    if (argc == 4){
      if (strcmp(argv[3], "-b") == 0)
	bannerMode = 1;
      else if (strcmp(argv[3], "-n") == 0)
	numberedMode = 1;
      else{
	printf("Please enter valid input\n");
	exit(1);
      }
    }
    
    if (argc == 5){
      if (strcmp(argv[3], "-b") == 0 && strcmp(argv[4], "-n") == 0){
	bannerMode = 1;
	numberedMode = 1;
      }
      else if (strcmp(argv[3], "-n") == 0 && strcmp(argv[4], "-b") == 0){
	numberedMode = 1;
	bannerMode = 1;
      }
      else{
	printf("Please enter valid input\n");
	exit(1);
      }
   }
   if (argc>5){
      printf("Too many arguments. Please enter valid input\n");
      exit(1);
    }
  }
  
  grabGrep();
  
  if (bannerMode == 1 && lineMode == 1)
    printf("THERE ARE %d LINES THAT MATCH\n", matchCount);
  //this will count the matching words
  if (wordMode == 1)
    wordModeTest();

  if (wordMode && bannerMode == 1)
    printf("THERE ARE %d MATCHING WORDS\n", wordCount);
  
  
  openFile();
  return 0;
  
}

int initList(struct list *sList){
  struct node *p;
  p = malloc(sizeof(struct node));
  p->data = "start";
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
  return NULL;
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
  while(p != NULL) {
    printf("%s\n", p->data);
    p=p->next;
  }
  return 0;
}

struct node* clone(struct node *sNode){
  if(sNode != NULL){
    
    struct node *result;
    result = malloc(sizeof(struct node));
    result->data = sNode->data;
    result->next = clone(sNode->next);

    return result;
  }
  return NULL;
}

int grabGrep(){
  
    initList(&fileList);
  
    
  //create char* for grepData and allocate enough space for it
  char* grepData;
  grepData = malloc((LINE_MAX* sizeof(char))+1);
  //loop through all of stdin
  
  while (fgets(grepData, LINE_MAX, stdin)){
    
    //count the number of chars in filename plust line number
    int charCount = 0;
    int colonCount = 0;
    int j;
    
    for(j = 0; colonCount < 2; j++) {
      charCount++;
      if (grepData[j] == ':')
	colonCount++;
      if (grepData[j] == '\0')
      {
	printf("Error in grep input. Please enter valid input.\n");
	exit(1);
      }
    }
    

    
    
    //create string of charCount size that will be just the file name and line number of grep data
    char* fileName;
    fileName = malloc(((sizeof(char))*charCount)+1);
    
    //copy the filename and line number over to fileName
    int k;
    int q;
    for(k = charCount, q=0; k > 1; k--, q++) {

      fileName[q] = grepData[q];
    }
    
    
    //push the file names over to my linked list
    matchCount++;
    push(&fileList, fileName);
    
    //erase grepData and malloc again to make space for next line
    free(grepData);
    grepData = malloc((LINE_MAX* sizeof(char))+1);
  }
  
  pop(&fileList);
  
  return 0;
}

int openFile(){
  
  //define a file that was previously opened and future file
  char* prevFile;
    prevFile = malloc(2408* sizeof(char)+1);
   
    char* prevLineNum;
    prevLineNum = malloc(2408* sizeof(char)+1);
  int lineCounter = 0;
  int count = 1;
  //while my linked list ain't empty
  while(emptyCheck(&fileList) == 0){

    char* futureFileToOpen;
    futureFileToOpen = malloc((LINE_MAX* sizeof(char))+1);
    
    char* futureListEntry;
    futureListEntry = malloc(2408* sizeof(char)+1);
    
    char* listEntry;
    listEntry = malloc(2408* sizeof(char)+1);
    listEntry = grab(&fileList);
    
    //determine size to malloc these strings DONT NEED
    int b = 0;
    int fileCount = 0;
    for(b = 0; listEntry[b] != ':'; b++) {
      fileCount++;
    }
    
    //move past first :
    b++;
    int z = 0;
    int lineCount = 0;
    for(z = 0; listEntry[b] != '\0'; z++) {
      lineCount++;
      b++;
    }
    
    //declare variables for files and line numbers allocated correctly!
    char* fileToOpen;
    fileToOpen = malloc((LINE_MAX* sizeof(char))+1);
    char* lineNumber;
    lineNumber = malloc((LINE_MAX* sizeof(char))+1);
    
    //copy filetoopen
    int i = 0;
    while (listEntry[i]!= ':'){
      fileToOpen[i] = listEntry[i];
      i++;
    }
    //skip over the first :
    i++;
    //copy lineentry
    int j=0;
    
    
    while (listEntry[i]!= '\0'){

      lineNumber[j] = listEntry[i];

      i++;
      j++;
    }
    
    //declare file variables
    char* oneLine;
    oneLine = malloc((LINE_MAX* sizeof(char))+1);
    FILE *file;
    
    //if we're working with a new file, open it
    //and set line counter to the appropriate line number
    if(strcmp(fileToOpen, prevFile) != 0){
      //if the previous file isn't empty, close it
	if (strlen(prevFile) != 0)
	  fclose(file);
	 
	file = fopen(fileToOpen, "r");
	//firstEntry = 0;
	lineCounter = atoi(lineNumber);
	printf("\n=========================%s\n", fileToOpen);
	count = 1;
  }
  //if its anotehr instance in teh same file,
  //set linecounter to teh linenumber minus the linenumber from the last instance
    else{
      lineCounter = atoi(lineNumber) - atoi(prevLineNum);
      //printf("line counter is %d\n", lineCounter);
    }
    
    //set a variable g to linecounter to print all lines before the match
    int g = lineCounter;
    //print all lines before the match
    while (g>1){
      fgets(oneLine, LINE_MAX, file);
      
      if(numberedMode == 1)
	printf("        %d: %s", count, oneLine);
      else
	printf("        %s", oneLine);
      g--;
      count++;
    }
    //print the match
    fgets(oneLine, LINE_MAX, file);
    
    
    //Count how many instances of the word on the matching line for -w
    
    if (wordMode == 1){

      int countPerLine = 0;
      char* wordString;
      wordString = malloc((LINE_MAX* sizeof(char))+1);
      
      wordString = strcasestr(oneLine, wordModeWord);
      while ((wordString = strcasestr(wordString, wordModeWord))){
	
	wordString += strlen(wordModeWord);
	
	countPerLine++;
      }

      char* testString;
      testString = malloc((LINE_MAX* sizeof(char))+1);
      testString = strcasestr(oneLine, wordModeWord);
      
      while (countPerLine > 0){
	char* copyString;
	copyString = malloc((LINE_MAX* sizeof(char))+1);
	//test string is the string starting with first match
	testString = strcasestr(testString, wordModeWord);

	//define length of full line minus area up to first match
	int x= strlen(oneLine) - strlen(testString);

	//copy oneline up to first match
	strncpy(copyString, oneLine, x);

	//add flag
	strcat(copyString, "\e[7m");
	//copy the word
	strncat(copyString, oneLine+x,strlen(wordModeWord));
	
	//add ending flag
	strcat(copyString, "\e[0m");

	//copy the rest
	strcat(copyString, oneLine + x+strlen(wordModeWord));
	
	strcpy(oneLine, copyString);

	testString += 4+strlen(wordModeWord);

	countPerLine--;
	
      }

    }

    
    
    if(numberedMode == 1)
      printf("MATCH!! %d: %s", count, oneLine);
    else
      printf("MATCH!! %s", oneLine);
    count++;
    //copy file to prevFile and prev line num
    strcpy(prevFile, fileToOpen);
    strcpy(prevLineNum, lineNumber);
    //pop the entry off my linked list
    pop(&fileList);
    
    //find future file that will be opened
    if(emptyCheck(&fileList) == 0){
      
      futureListEntry = grab(&fileList);
      
      
      int v = 0;
      while (futureListEntry[v]!= ':'){
	futureFileToOpen[v] = futureListEntry[v];
	v++;
      }
      //printf("future file is %s\n", futureFileToOpen);
    }
    
    //if we're moving to a new file or our filelist is empty print rest of current file 
    if(strcmp(fileToOpen, futureFileToOpen) != 0){

      while (fgets(oneLine, LINE_MAX, file)){
	if (numberedMode == 1)
	  printf("        %d: %s", count, oneLine);
	else
	  printf("        %s", oneLine);
	count++;
      }
      printf("\n");
    }
  }
  
  return 0;
}

int wordModeTest(){
  
  //create clone so original file list doesn't get affected
  initList(&cloneList);
  cloneList.start = clone(fileList.start);
  
  int lineCounter = 0;
  int count = 1;
  //define a file that was previously opened and future file
  char* prevFile;
  prevFile = malloc(2408* sizeof(char)+1);
  
  char* prevLineNum;
  prevLineNum = malloc(2408* sizeof(char)+1);
  
  
  //while my linked list ain't empty
  while(emptyCheck(&cloneList) == 0){
    
    
    
    char* futureFileToOpen;
    futureFileToOpen = malloc((LINE_MAX* sizeof(char))+1);
    
    char* futureListEntry;
    futureListEntry = malloc(2408* sizeof(char)+1);
    
    char* listEntry;
    listEntry = malloc(2408* sizeof(char)+1);
    listEntry = grab(&cloneList);
    
    
    
    //declare variables for files and line numbers allocated correctly!
    char* fileToOpen;
    fileToOpen = malloc((LINE_MAX* sizeof(char))+1);
    char* lineNumber;
    lineNumber = malloc((LINE_MAX* sizeof(char))+1);
    
    
    
    //copy filetoopen
    int i = 0;
    while (listEntry[i]!= ':'){
      fileToOpen[i] = listEntry[i];
      i++;
    }
    i++;
    int j=0;
    
    
    while (listEntry[i]!= '\0'){
      
      lineNumber[j] = listEntry[i];
      i++;
      j++;
    }
    

    //declare file variables
    char* oneLine;
    oneLine = malloc((LINE_MAX* sizeof(char))+1);
    FILE *file;
    
    //if we're working with a new file, open it
    //and set line counter to the appropriate line number
    if(strcmp(fileToOpen, prevFile) != 0){
      //if its not the first entry

      file = fopen(fileToOpen, "r");
      //firstEntry = 0;
      lineCounter = atoi(lineNumber);
      count = 1;
    }
    //if its anotehr instance in teh same file,
    //set linecounter to teh linenumber minus the linenumber from the last instance
    else{
      lineCounter = atoi(lineNumber) - atoi(prevLineNum);
    }

    //set a variable g to linecounter to print all lines before the match
    int g = lineCounter;
    //print all lines before the match
    while (g>1){
      fgets(oneLine, LINE_MAX, file);
      g--;
      count++;
    }
    //print the match
    fgets(oneLine, LINE_MAX, file);
    
    
    //Count how many instances of the word on the matching line for -w
    
    if (wordMode == 1){
      char* testString;
      testString = malloc((LINE_MAX* sizeof(char))+1);
      testString = strcasestr(oneLine, wordModeWord);
      if (testString == NULL){
	printf("Your words don't match. Please enter valid input\n");
	exit(1);
      }
      while ((testString = strcasestr(testString, wordModeWord))){

	testString += strlen(wordModeWord);
	wordCount++;
      }
    }
    
    
    //copy file to prevFile and prev line num
    strcpy(prevFile, fileToOpen);
    strcpy(prevLineNum, lineNumber);
    //pop the entry off my linked list
    
    pop(&cloneList);

    //find future file that will be opened
    if(emptyCheck(&cloneList) == 0){
      
      futureListEntry = grab(&cloneList);
      
      
      int v = 0;
      while (futureListEntry[v]!= ':'){
	futureFileToOpen[v] = futureListEntry[v];
	v++;
      }
    }
     
  }
  
  return 0;
  
}
