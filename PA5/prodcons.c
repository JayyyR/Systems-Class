 
/* The classic producer-consumer example.
   Illustrates mutexes and conditions.
   All integers between 0 and 9999 should be printed exactly twice,
   once to the right of the arrow and once to the left. */

#include <stdio.h>
#include <pthread.h>

#define BUFFER_SIZE 16

/* Circular buffer of integers. */

struct prodcons
{
  int buffer[BUFFER_SIZE];	/* the actual data */
  pthread_mutex_t lock;		/* mutex ensuring exclusive access to buffer */
  int readpos, writepos;	/* positions for reading and writing */
  pthread_cond_t notempty;	/* signaled when buffer is not empty */
  pthread_cond_t notfull;	/* signaled when buffer is not full */
};

/* Initialize a buffer */
static void
init (struct prodcons *b)
{
  pthread_mutex_init (&b->lock, NULL);
  pthread_cond_init (&b->notempty, NULL);
  pthread_cond_init (&b->notfull, NULL);
  b->readpos = 0;
  b->writepos = 0;
}

/* Store an integer in the buffer */
static void
put (struct prodcons *b, int data)
{
  //printf("before locking in put\n");
  printf("BEFORE LOCKED BY PUT=============\n");
  pthread_mutex_lock (&b->lock);
  printf("AFTER LOCKED BY PUT=============\n");
  /* Wait until buffer is not full */
  //printf("before not full signal put\n");
  while ((b->writepos + 1) % BUFFER_SIZE == b->readpos)
    {
      printf("notfull\n");
      pthread_cond_wait (&b->notfull, &b->lock);
      /* pthread_cond_wait reacquired b->lock before returning */
    }
    //printf("after not full signal put\n");
  /* Write the data and advance write pointer */
  b->buffer[b->writepos] = data;
  b->writepos++;
  if (b->writepos >= BUFFER_SIZE)
    b->writepos = 0;
  /* Signal that the buffer is now not empty */
  //printf("before sending signal in put\n");
  pthread_cond_signal (&b->notempty);
  //printf("after sending signal in put\n");
 // printf("before unlocking in put\n");
  printf("BEFORE UNLOCKED BY PUT============\n");
  pthread_mutex_unlock (&b->lock);
  printf("AFTER UNLOCKED BY PUT============\n");
}

/* Read and remove an integer from the buffer */
static int
get (struct prodcons *b)
{
  int data;
  //printf("before locking in get\n");
  printf("BEFORE LOCKED BY GET-----------\n");
  pthread_mutex_lock (&b->lock);
  printf("AFTER LOCKED BY GET-------------\n");
  /* Wait until buffer is not empty */
 // printf("before not empty signal get\n");
  while (b->writepos == b->readpos)
    {
      printf("notempty\n");
      pthread_cond_wait (&b->notempty, &b->lock);
    }
    //printf("after not empty signal get\n");
  /* Read the data and advance read pointer */
  data = b->buffer[b->readpos];
  b->readpos++;
  if (b->readpos >= BUFFER_SIZE)
    b->readpos = 0;
  /* Signal that the buffer is now not full */
  //printf(" before sending signal in get\n");
  pthread_cond_signal (&b->notfull);
  //printf("after sending signal in get\n");
  //printf("before unlocking in get\n");
  printf("BEFORE UNLOCKED BY GET-------\n");
  pthread_mutex_unlock (&b->lock);
  printf("AFTER UNLOCKED BY GET--------\n");
  return data;
}

/* A test program: one thread inserts integers from 1 to 10000,
   the other reads them and prints them. */

#define OVER (-1)

struct prodcons buffer;

static void *
producer (void *data)
{
  int n;
  for (n = 0; n < 10000; n++)
    {
     // printf ("%d --->\n", n);
      put (&buffer, n);
    }
  put (&buffer, OVER);
  return NULL;
}

static void *
consumer (void *data)
{
  int d;
  while (1)
    {
      d = get (&buffer);
      if (d == OVER)
	break;
      //printf ("---> %d\n", d);
    }
  return NULL;
}

int
main (void)
{
  pthread_t th_a, th_b;
  void *retval;

  init (&buffer);
  /* Create the threads */
  pthread_create (&th_a, NULL, producer, 0);
  pthread_create (&th_b, NULL, consumer, 0);
  /* Wait until producer and consumer finish. */
  pthread_join (th_a, &retval);
  pthread_join (th_b, &retval);
  return 0;
}