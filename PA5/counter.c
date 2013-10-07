#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>


//struct for the buffer
struct readcount{
  char** buffer; //buffer of correct size
  pthread_mutex_t lock;
  int readpos, writepos;
  pthread_cond_t notempty;
  pthread_cond_t notfull;
};

//structs for final lists
struct node {
  char* data;
  int freq;
  struct node *next;
  struct node*prev;
};

struct list{
  struct node *start;
  struct node *end;
};


int numLines;		//buffersize
int maxCounters;
long fileDelay;
long threadDelay;
FILE* file;
int listCount = 0;
struct readcount buffy;	//our buffer
struct list oddWords;
struct list evenWords;
struct timespec fileDel;
struct timespec threadDel;
int EOFflag = 0;	//flag declaring read should stop
int linesCounted = 0;
char* finalLine;	//final line that lets count know to stop
int counterStop = 0;	//flag for counter to stop
int bufferFullCount = 0;	//number of times buffer was full
int bufferFull = 0;		//flag that indicates new counters hould be created
pthread_t th_a, th_b, newThread;
int oneFile = 1; //set to one so read will open first file
pthread_t* threadNames; //array of all new threads
int threadCount = 0;
int fileCount = 0;
char** fileNames; //array for filenames
void* retval;
char* threadName ="a";
char newName = 'b';
int numLinesSet = 0;
int maxCountersSet = 0;
int fileDelaySet = 0;
int threadDelaySet = 0;

//declarations of functions
void init (struct readcount *buf, int numLines);
static void reader (struct readcount *buf);
static void counter (struct readcount *buf, char* name);
static void * read (void* data);
static void * count (void* data);
int initList(struct list *sList, char* data);
int print(struct list *sList);
void insertWord(struct list *sList, char * oneWord);
void callThread(char* name);


int main(int argc, char *argv[]){
  
  //doesnt have all neccessary command line args
  if (argc < 10){
    printf("Error. Please enter correct arguments\n");
    exit(1);
  }
  
  //grab correct variables
  int u;
  for (u = 1; u<argc; u++){
    
    if (strcmp(argv[u], "-b") == 0){
      numLines = atoi(argv[u+1]);
      if (numLines <= 0){
	fprintf(stderr, "Error. Please enter positive integer for numlines\n");
	exit(1);
      }
      numLinesSet = 1;
    }
    
    if (strcmp(argv[u], "-t") == 0){
      maxCounters = atoi(argv[u+1]);
      if (maxCounters > 26 || maxCounters < 1){
	fprintf(stderr, "Error. Please enter a positive integer less than 27 for maxcounters\n");
	exit(1);
      }
      maxCountersSet = 1;
    }
    if (strcmp(argv[u], "-d") == 0){
      fileDelay = atoi(argv[u+1]);
      if (fileDelay < 0){
	fprintf(stderr, "Error. filedelay must be zero or greater.\n");
	exit(1);
      }
      fileDelaySet = 1;
    }
    if (strcmp(argv[u], "-D") == 0){
      threadDelay = atoi(argv[u+1]);
      if (threadDelay < 0){
	fprintf(stderr, "Error. threaddelay must be zero or greater.\n");
	exit(1);
      }
      threadDelaySet = 1;
    }
  }
  
  //check if all necessary arguments were set
  if (numLinesSet == 0 || maxCountersSet == 0 || fileDelaySet == 0 || threadDelaySet == 0){
    printf("Error. Please enter correct arguments\n");
    exit(1);
  }
  
  //store correct time values milliseconds
  if (fileDelay <= 999)
    fileDel.tv_nsec = fileDelay*1000000;
  else
    fileDel.tv_sec = fileDelay/1000;
  //store correct time values in milliseonds
    if (threadDelay <= 999)
      threadDel.tv_nsec = threadDelay*1000000;
    else
      threadDel.tv_sec = threadDelay/1000;
    
    
    //malloc space for all threadnames and filenames
      threadNames = malloc(sizeof(pthread_t)*maxCounters+1);
      fileNames = malloc(sizeof(char)*argc);
      
      //number of files entered
      int numFiles = argc - 9;
      
      //initialize lists
      initList(&oddWords, "0");
      initList(&evenWords, "0");
      
      //stick file names in file array
      while(numFiles > 0){
	char* fileToOpen = argv[8+numFiles];
	fileNames[fileCount] = fileToOpen;
	fileCount++;
	numFiles--;
      }
      
      EOFflag = 0;
      counterStop = 0;
      
      finalLine = malloc((LINE_MAX* sizeof(char))+1);
      
      finalLine = "\n";
      
      //initialize buffer
      init(&buffy, numLines);
      
      //if the buffersize is 1, setnumlines to 2 to fix empty checks
      if (numLines == 1){
	numLines++;
      }

      //create threads
      pthread_create (&th_a, NULL, read, 0);
      pthread_create (&th_b, NULL, count, (void*) threadName);
      
      //wait til they finish
      pthread_join (th_a, &retval);
      pthread_join (th_b, &retval);
      
      //join all threads that were created
      int y;
      for (y = 0; y < threadCount; y++){
	pthread_join (threadNames[y], &retval);
      }

      //get rid of the zeros at the front of the lists
      oddWords.start = oddWords.start->next;
      evenWords.start = evenWords.start->next;
      
      printf("\n");
      if (oddWords.start != NULL){
	printf("ODD WORDS\n");
	printf("---------\n");
	print(&oddWords);
      }
      else{
	printf("ODD WORDS\n");
	printf("---------\n");
	printf("No Odd Words Found\n\n");
      }
      if (evenWords.start != NULL){
	printf("\n");
	printf("EVEN WORDS\n");
	printf("----------\n");
	print(&evenWords);
      }
      else{
	printf("EVEN WORDS\n");
	printf("---------\n");
	printf("No Even Words Found\n");
      }
      
      return 0;
}

void callThread(char* name){
  threadName = name;
  pthread_t newThread;
  threadNames[threadCount] = newThread;
  threadCount++;
  
  maxCounters--;
  pthread_create (&newThread, NULL, count, (void*) threadName);
}

static void reader (struct readcount *buf){

  pthread_mutex_lock(&buf->lock);

  //wait until buffer is not full
  while ((buf->writepos+1) % numLines == buf->readpos){
    pthread_cond_wait (&buf->notfull, &buf->lock);
  }

  char* oneLine;
  oneLine = malloc((LINE_MAX* sizeof(char))+1);
  
  //if its the first time entering or other file has been closed
  if (oneFile == 1){
    
    //open file, output to stderr if can't be opened
    while(1){

      file = fopen(fileNames[fileCount-1], "r");
      if (file == NULL){
	fprintf(stderr, "ERROR: The file %s could not be opened. Continuing with rest of files\n", fileNames[fileCount-1]);
	fileCount--;
	if (fileCount <= 0){
	  break;
	}
	continue;
      }
      else
	break;
    }

    
    
  }
  if (fileCount != 0)
  {
    oneFile = 0;
    
    fgets(oneLine, LINE_MAX, file);
    
    //if end of one file
    if (feof(file)){
      oneFile = 1;
      fclose(file);
      fileCount--;
    }
    
    //if end of all files
    if (fileCount == 0){
      EOFflag = 1;
    }
    
    //if the last line, set it as a global so counter knows when to stop
    if (EOFflag == 1){
      finalLine = oneLine;
    }
    
    //write line from file to buffer and advance pointer
    buf->buffer[buf->writepos] = oneLine;
    buf->writepos++;
    if (buf->writepos >= numLines){
      buf->writepos = 0;
      bufferFullCount ++;
      bufferFull = 1;
      if (maxCounters > 1){
	char* str = malloc(sizeof(char));
	str[0] = newName;
	callThread(str);
	newName++;
      }
    }

    //signal that the buffer now isn't empty
    pthread_cond_signal (&buf->notempty);

    pthread_mutex_unlock (&buf->lock);
  }
  //final file could not be opened
  else{
    EOFflag = 1;
    pthread_cond_signal (&buf->notempty);
    counterStop = 1;
    pthread_mutex_unlock (&buf->lock);
  }
}

static void counter (struct readcount *buf, char* name)
{
  //local val so we know which thread stopped it
  int thisStop = 0;
  char* data;
  data = malloc((LINE_MAX* sizeof(char))+1);
  
  pthread_mutex_lock (&buf->lock);
  
  while (buf->writepos == buf->readpos)
  {
    if (counterStop == 1){
      break;
    }
    pthread_cond_wait (&buf->notempty, &buf->lock);
    
  }
  //when remainging threads get here after counts are done, they should skip all this
  if (counterStop == 0){

    /* Read the data from buffer and advance read pointer */
    data = buf->buffer[buf->readpos];
    nanosleep(&threadDel, NULL);

    if (strcmp(data, finalLine) == 0 && EOFflag == 1){
      counterStop = 1;
      thisStop = 1;
    }
    
    //add a \n to data for EOF checks
    strcat(data,"\n");
    
    //loop through data to split words
    char c = '\0';
    int j = 0;
    while (c != '\n' && strcmp(data, "") != 0){
      //printf("in outer while\n");
      int k = 0;
      char* oneWord;
      oneWord = malloc((LINE_MAX* sizeof(char))+1);
      
      
      c = data[j];
      while (c != ' ' && c != '\n' && c != '\t' && c != '\v' && c != '\f'){
	oneWord[k] = c;
	j++;
	k++;
	c = data[j];
      }
      j++;
      
      //if oneWord isn't empty:
      if (strcmp(oneWord, "") != 0){
	//check if odd or even and insert into list alphabetically
	if (strlen(oneWord) % 2 == 0){
	  insertWord(&evenWords, oneWord);
	}
	else{
	  insertWord(&oddWords, oneWord);
	}
	//print counter name
	printf("%s", name);
	fflush(stdout);
      }      
    }
  }
  buf->readpos++;
  if (buf->readpos >= numLines)
    buf->readpos = 0;
  /* Signal that the buffer is now not full */
  pthread_cond_signal (&buf->notfull);
  
  pthread_mutex_unlock (&buf->lock);
  
  //cancel all remaning threads
  if (thisStop == 1){
    int y;
    for (y = 0; y < threadCount; y++){
      pthread_cancel(threadNames[y]);
    }
    pthread_cancel(th_b);
    pthread_cancel(th_a);
  }
  
}

static void * read (void* data)
{
  
  while (EOFflag == 0)
  {
    reader (&buffy);
    nanosleep(&fileDel, NULL);
    linesCounted++;
  }
  return NULL;
}

static void * count (void* data)
{
  char* name = (char*) data;
  while (counterStop == 0)
  {
    counter (&buffy, name);
  }
  return NULL;
}

//initialize the buffer
void init (struct readcount *buf, int numLines)
{
  buf->buffer = malloc(sizeof(char*)*numLines+1);
  pthread_mutex_init (&buf->lock, NULL);
  pthread_cond_init (&buf->notempty, NULL);
  pthread_cond_init (&buf->notfull, NULL);
  buf->readpos = 0;
  buf->writepos = 0;
}

//initalize list
int initList(struct list *sList, char* data){
  struct node *p;
  p = malloc(sizeof(struct node));
  p->data = data;
  p->freq = 1;
  p->next = NULL;
  p->prev = NULL;
  sList->start = p;
  sList->end = p;
  
  return 0;
}

int print(struct list *sList)
{
  struct node *p = sList->start;
  while(p != sList->end) {
    fprintf(stderr, "%s  %d\n", p->data, p->freq);
    p=p->next;
  }
  fprintf(stderr, "%s  %d\n", p->data, p->freq);
  
  return 0;
}

void insertWord(struct list *sList, char* oneWord)
{ 
  struct node *t = sList->start;
  //get past the 0
  if (t != sList->end)
    t = t->next;
  //while we're not at end of linked list and oneWord is greater than current word
    while (t != sList->end && (strcmp(oneWord, t->data) > 0)){
      t = t->next;
    }
    //if oneword is equal to current word, increase freq
    if (strcmp(oneWord, t->data) == 0){
      t->freq = t->freq + 1; 
    }
    //if at the end of the list
    else if (t == sList->end){
      if (strcmp(oneWord, t->data) > 0 || t == sList->start){
	struct node *new;
	new = malloc(sizeof(struct node));
	new->data = oneWord;
	new->freq = 1;
	new->next = NULL;
	new->prev = t;
	t->next = new;
	sList->end = new;
	listCount++;
      }
      //if less than, add it before
      else{
	struct node *new;
	new = malloc(sizeof(struct node));
	new->data = oneWord;
	new->freq = 1;
	new->next = t;
	new->prev = t->prev;
	t->prev->next = new;
	t->prev = new;
	listCount++;
      }
    }
    //otherwise add it in
    else{
      struct node *new;
      new = malloc(sizeof(struct node));
      new->data = oneWord;
      new->freq = 1;
      new->next = t;
      new->prev = t->prev;
      t->prev->next = new;
      t->prev = new;
      listCount++;
    }
    
}