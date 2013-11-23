/* All Rights Reversed - No Rights Reserved */

/* Simple queue implementation.
 * Used for calling write() in a
 * separate thread, consuming items
 * from the queue.
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "queue.h"

#define MAXQ (1024*8)
static myq_t q[MAXQ];
static int head = -1;
static int tail = -1;
static pthread_mutex_t q_mutex;
static pthread_cond_t q_cond;
static int time_to_exit = 0;
static int q_active = 0;

void q_init(void)
{
   int i;
   myq_t *qp;
   for (i = 0; i < MAXQ; i++) {
      qp = &q[i];
      memset(qp, 0, sizeof(myq_t));
   }

   pthread_mutex_init(&q_mutex, NULL);
   pthread_cond_init(&q_cond, NULL);
   q_active = 1;
}

void q_release(void)
{
   time_to_exit = 1;
   q_active = 0;
   pthread_cond_broadcast(&q_cond);
   pthread_cond_destroy(&q_cond);
   pthread_mutex_destroy(&q_mutex);
}

void q_push(char *str, int len)
{
   myq_t *qp = &q[0];
   int i;

   pthread_mutex_lock(&q_mutex);

   /* Find first free memory slot */
   for (i = 0; i < MAXQ; i++) {
      if (!qp->busy) {
         break;
      }
      qp++;
   }

   if (i == MAXQ) {
      /* q completely full :(
       * if this happens, increase MAXQ and recompile.
       */
      pthread_mutex_unlock(&q_mutex);
      fprintf(stderr, "Warning: queue full, dropping item %.*s", len, str);
      return;
   }

   /* Must set all fields in qp. Might be reused. */
   qp->index = i;
   qp->msg = str;
   qp->msglen = len;
   qp->busy = 1;
   qp->prev = NULL;
   if (head == -1) {
      /* list is empty */
      head = i;
      tail = i;
      pthread_cond_broadcast(&q_cond);
      /* qp->next = NULL; */
   } else {
      /* Insert item at head */
      myq_t *hp = &q[head];
      hp->prev = qp;
      /* qp->next = hp; */
      head = i;
   }

   pthread_mutex_unlock(&q_mutex);
}

char *q_pop(int *len)
{
   myq_t *pq, *tq;
   char *rets = NULL;

   if (!q_active) {
      return NULL;
   }

   pthread_mutex_lock(&q_mutex);

   if (tail == -1) {
      pthread_mutex_unlock(&q_mutex);
      return NULL;
   }

   /* pop tail */
   tq = &q[tail];
   if (!tq->prev) {
      /* Last item */
      head = -1;
      tail = -1;
   } else {
      pq = tq->prev;
      /* pq->next = NULL; */
      tail = pq->index;
   }
   tq->busy = 0;
   rets = tq->msg;
   *len = tq->msglen;

   pthread_mutex_unlock(&q_mutex);

   return rets;
}

void wait_until_q_not_empty(void)
{
   pthread_mutex_lock(&q_mutex);
   while ((!time_to_exit) && (tail == -1)) {
      /* printf("DBG: Waiting for cond\n"); */
      pthread_cond_wait(&q_cond, &q_mutex);
   }
   pthread_mutex_unlock(&q_mutex);
}

/* /\* print q to stdout (for debug) *\/ */
/* void q_print(void) */
/* { */
/*    myq_t *qp; */

/*    if (head == -1) { */
/*       printf("q empty\n"); */
/*       return; */
/*    } */

/*    qp = &q[head]; */
/*    while (qp) { */
/*       printf("%.*s", qp->msglen, qp->msg); */
/*       qp = qp->next; */
/*    } */
/* } */

/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
