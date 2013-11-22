#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "queue.h"

static pthread_t print_thread;
static int exit_print_thread = 0;

static void *run_print_thread(void *data __attribute__ ((unused)))
{
   char *str;
   int len;

   while (!exit_print_thread) {
      wait_until_q_not_empty();
      do {
         str = q_pop(&len);
         if (str) {
            int nwritten = 0;
            int nleft = len;
            do {
               str += nwritten;
               nwritten = write(1, str, nleft);
               nleft -= nwritten;
            } while (nleft > 0);
         }
      } while (str);
   }

   return NULL;
}

void spawn_print_thread(void)
{
   q_init();
   pthread_create(&print_thread, NULL, &run_print_thread, NULL);
}

void join_print_thread(void)
{
   exit_print_thread = 1;
   q_release();
   pthread_join(print_thread, NULL);
}


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
