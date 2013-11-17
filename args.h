#ifndef __ARGS_H
#define __ARGS_H

/* All Rights Reversed - No Rights Reserved */

struct opts
{
   /* Mandatory */
   char *filename;
   char *needle;
   int needlen;

   /* Optional */
   char *vstring;
   int vlen;
   int single;
   int debug;
};

/* Parsed options will be here */
extern struct opts opt;

extern int parse_args(int argc, char **argv);
extern void print_usage(void);


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */

#endif /* __ARGS_H */
