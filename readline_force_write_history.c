#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/history.h>
#include <wordexp.h>
#include <fcntl.h>
#include <unistd.h>


#define DEFAULT_HISTORY "~/.history"

static void (*using_history_orig) (void) = NULL;
static void (*add_history_orig) (const char*) = NULL;
static const char* history_file = NULL;
static int init_done = 0;

void using_history (void) {
  if (!init_done) {
    using_history_orig = dlsym (RTLD_NEXT, "using_history");
    if (using_history_orig == NULL) {
      printf ("cannot find using_history\n");
      exit (127);
    }

    history_file = getenv ("HISTORY_FILE");
    if (history_file == NULL)
      history_file = DEFAULT_HISTORY;

    // Touch the history file
    wordexp_t exp_result;
    int fd;
    wordexp (history_file, &exp_result, 0);
    history_file = exp_result.we_wordv[0];
    // exp_result should be freed, but why bother.

    if ((fd = open (history_file, O_CREAT, S_IRUSR | S_IWUSR)) < 0 || close (fd) < 0) {
      fprintf (stderr, "cannot use history file %s\n", history_file);
      exit (127);
    }
  }

  using_history_orig ();

  if (!init_done) {
    read_history (history_file);
    init_done = 1;
  }
}

void add_history (const char* string) {
  if (add_history_orig == NULL)
    add_history_orig = dlsym (RTLD_NEXT, "add_history");
  add_history_orig (string);
  if (!init_done)
    return;
  append_history (1, history_file);
}
