/* All Rights Reversed - No Rights Reserved */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#include "args.h"
#include "print.h"
#include "queue.h"

typedef struct chunk {
   char *start;
   size_t len;
} chunk_t;

/* Memory mapping */
static char *map = NULL;
static struct stat mapstat;

void free_map(void)
{
   if (map) {
      munmap(map, mapstat.st_size);
      map = NULL;
   }
}

int init_map(const char *filename)
{
   int ret = 0;
   int fd = open(filename, O_RDONLY);

   if (fd < 0) {
      perror(filename);
      goto out;
   }

   if (fstat(fd, &mapstat) < 0) {
      perror(filename);
      goto out2;
   }

   map = mmap(NULL, mapstat.st_size, PROT_READ, MAP_SHARED, fd, 0);
   if (map == MAP_FAILED) {
      perror(filename);
      goto out2;
   }

   /* Free map automatically when program exits */
   atexit(free_map);
   ret = 1;

 out2:

   close(fd);

 out:

   return ret;
}

void *run_chunk(void *data)
{
   char *line;
   char *next;
   size_t linelen;
   size_t offset;
   chunk_t *chunk = (chunk_t *)data;

   if (opt.debug) {
      fprintf(stderr, "DBG: %ld, %ld\n", (long int)(chunk->start - map), (long int)chunk->len);
   }
   line = chunk->start;
   offset = 0;

   while (offset < chunk->len) {
      next = strchr(line, '\n');
      if(!next) {
         break;
      }
      /* Add 1 to linelen to include newline */
      linelen = next - line + 1;

      /* Print line if it contains needle */
      if (memmem(line, linelen - 1, opt.needle, opt.needlen)) {
         /* and does *not* contain vstring (if present) */
         if ((!opt.vstring) || (!memmem(line, linelen - 1, opt.vstring, opt.vlen))) {
            q_push(line, (int)linelen);
         }
      }

      offset += linelen;
      line = next + 1;
   }

   return NULL;
}


#define MAX_CHUNKS 16

int main(int argc, char *argv[])
{
   char *next, *end;
   pthread_t threads[MAX_CHUNKS];
   chunk_t chunk[MAX_CHUNKS];
   int num_cpus, i;

   if (!parse_args(argc, argv)) {
      print_usage();
      return 1;
   }

   if (!init_map(opt.filename)) {
      return 1;
   }

   if (opt.single) {
      num_cpus = 1;
   } else {
      /* Keep num_cpus in range 1 - MAX_CHUNKS */
      num_cpus = (int)sysconf(_SC_NPROCESSORS_ONLN);
      if (num_cpus < 2) {
         num_cpus = 1;
      } else if (num_cpus > MAX_CHUNKS) {
         num_cpus = MAX_CHUNKS;
      }
   }

   /* Calculate start address and length of each chunk */
   chunk[0].start = map;
   chunk[0].len = (size_t)mapstat.st_size / num_cpus;
   for (i = 1; i < num_cpus; i++) {
      /* Increase len to include next newline */
      end = chunk[i - 1].start + chunk[i - 1].len;
      if (end - map >= mapstat.st_size) {
         /* Past last line (few lines in file), set 1 chunk */
         chunk[0].len = (size_t)mapstat.st_size;
         num_cpus = 1;
         break;
      }
      next = strchr(end, '\n');
      chunk[i - 1].len += next - end;
      chunk[i].start = next + 1;
      chunk[i].len = (size_t)mapstat.st_size / num_cpus;

      if (chunk[i].start - map >= mapstat.st_size) {
         /* Past last line */
         chunk[0].len = (size_t)mapstat.st_size;
         num_cpus = 1;
         break;
      }
   }

   /* Adjust last len with help of file size */
   if (num_cpus > 1) {
      size_t abs_start = chunk[i].start - map;
      chunk[i].len = mapstat.st_size - abs_start;
   }

   if (opt.debug) {
      fprintf(stderr, "DBG: num threads %d\n", num_cpus);
   }

   /* Start print thread */
   spawn_print_thread();

   /* Spawn num_cpus threads chewing on one chunk each */
   for (i = 0; i < num_cpus; i++) {
      pthread_create(&threads[i], NULL, &run_chunk, (void *)&chunk[i]);
   }

   /* Join child threads */
   for (i = 0; i < num_cpus; i++) {
      pthread_join(threads[i], NULL);
   }

   /* Cleanup */
   join_print_thread();

   return 0;
}


/**
 * Local Variables:
 * mode: c
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
