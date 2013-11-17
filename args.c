/* All Rights Reversed - No Rights Reserved */

#include "args.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PROGRAM_NAME "fast-grep"
#define PROGRAM_VERSION "0.1"
#define PROGRAM_NAME_VERSION PROGRAM_NAME "-" PROGRAM_VERSION

/* Filled in by parse_args */
struct opts opt;

/* Program documentation. */
static char *prog_doc =
   PROGRAM_NAME_VERSION
   "\nFast, multi-threaded grep.\n\n"
   "USAGE\n\n" PROGRAM_NAME
   " [OPTIONS] "
   "<needle> <haystack>\n\n"
   "MANDATORY ARGUMENTS\n\n"
   "needle   - string to search for\n"
   "haystack - name of file to search (for needle string)\n\n"
   "OPTIONS\n\n"
   "-v <invert> - skip lines containing this \"invert\" string\n\n"
   "EXAMPLE\n\n"
   "Print all lines in the file cred containing the string\n"
   "'.se-', but skip lines containing the string '-|-|--':\n\n"
   "./" PROGRAM_NAME " -v '-|-|--' '.se-' cred\n\n";

void print_usage(void)
{
   printf("%s", prog_doc);
}

int parse_args(int argc, char **argv)
{
   int c;

   /* Default values. */
   opt.vstring = NULL;

   /* Parse optional arguments */
   opterr = 0;
   while ((c = getopt (argc, argv, "hv:")) != -1) {

      switch (c) {

      case 'v':
         opt.vstring = optarg;
         opt.vlen = strlen(opt.vstring);
         break;

      case 'h':
         /* Just print help */
         return 0;


      case '?':
         fprintf(stderr, "Error: unknown option -%c\n\n", optopt);
         return 0;

      default:
         return 0;
      }
   }

   if (argc != optind + 2) {
      puts("Error: wrong number of mandatory arguments");
      return 0;
   }

   /* Mandatory arguments */
   opt.needle = argv[optind++];
   opt.needlen = strlen(opt.needle);
   opt.filename = argv[optind];

   return 1;
}


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
