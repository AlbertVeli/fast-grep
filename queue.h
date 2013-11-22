#ifndef __QUEUE_H
#define __QUEUE_H

/* All Rights Reversed - No Rights Reserved */

typedef struct myq {
   /* struct myq *next; */
   struct myq *prev;
   char *msg;
   int msglen;
   int busy;
   int index;
} myq_t;

void q_init(void);
void q_release(void);
void q_push(char *str, int len);
char *q_pop(int *len);
void wait_until_q_not_empty(void);
/* void q_print(void); */


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */

#endif /* __QUEUE_H */
