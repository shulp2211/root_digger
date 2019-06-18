#ifndef __RD_DEBUG__
#define __RD_DEBUG__

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <execinfo.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

const clock_t CLOCK_START = clock();
extern bool __PROGRESS_BAR_FLAG__;
extern bool __VERBOSE__;

#define DEBUG_IF_FLAG 1

#define EMIT_DEBUG_FLAG __VERBOSE__

#define print_clock                                                            \
  do {                                                                         \
    if (DEBUG_IF_FLAG && EMIT_DEBUG_FLAG) {                                    \
      fprintf(stderr, "[%f] ",                                                 \
              ((double)clock() - CLOCK_START) / CLOCKS_PER_SEC);               \
    }                                                                          \
  } while (0)

#define debug_print(fmt, ...)                                                  \
  do {                                                                         \
    if (DEBUG_IF_FLAG && EMIT_DEBUG_FLAG) {                                    \
      print_clock;                                                             \
      fprintf(stderr, "[%s:%s:%d]: " fmt "\n", __FILE__, __func__, __LINE__,   \
              __VA_ARGS__);                                                    \
    }                                                                          \
  } while (0)

#define debug_string(x)                                                        \
  do {                                                                         \
    debug_print("%s", x);                                                      \
  } while (0)

#define print_trace()                                                          \
  do {                                                                         \
    if (DEBUG_IF_FLAG) {                                                       \
      void *callstack[128];                                                    \
      int frames = backtrace(callstack, 128);                                  \
      char **bt_symbols = backtrace_symbols(callstack, frames);                \
      print_clock;                                                             \
      fprintf(stderr, "BACKTRACE AT %s:%d:%s():\n", __FILE__, __LINE__,        \
              __func__);                                                       \
      for (int i = 0; i < frames; ++i) {                                       \
        print_clock;                                                           \
        fprintf(stderr, "%s\n", bt_symbols[i]);                                \
      }                                                                        \
    }                                                                          \
  } while (0)

#define assert_string(cond, comment)                                           \
  do {                                                                         \
    if (DEBUG_IF_FLAG && EMIT_DEBUG_FLAG) {                                    \
      {                                                                        \
        if (!(cond)) {                                                         \
          print_clock;                                                         \
          fprintf(                                                             \
              stderr,                                                          \
              "assertion \"%s\" failed: file: %s, line: %d, comment: %s\n",    \
              #cond, __FILE__, __LINE__, comment);                             \
          abort();                                                             \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  } while (0)

#define turn_on_progress()                                                     \
  do {                                                                         \
    __PROGRESS_BAR_FLAG__ = true;                                              \
  } while (0)

#define turn_off_progress()                                                    \
  do {                                                                         \
    __PROGRESS_BAR_FLAG__ = false;                                             \
  } while (0)

#define toggle_progress()                                                      \
  do {                                                                         \
    __PROGRESS_BAR_FLAG__ = !__PROGRESS_BAR_FLAG__;                            \
  } while (0)

#define print_progress(done, total)                                            \
  do {                                                                         \
    if (__PROGRESS_BAR_FLAG__) {                                               \
      struct winsize w;                                                        \
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);                                    \
      print_progress_cols(done, total, w.ws_col);                              \
    }                                                                          \
  } while (0)

#define print_progress_cols(done, total, cols)                                 \
  do {                                                                         \
    if (__PROGRESS_BAR_FLAG__) {                                               \
      size_t adj_cols = cols - 5;                                              \
      if (done == 0)                                                           \
        adj_cols--;                                                            \
      size_t total_padding = 0;                                                \
      size_t tmp = total;                                                      \
      while (tmp != 0) {                                                       \
        tmp /= 10;                                                             \
        adj_cols--;                                                            \
        total_padding++;                                                       \
      }                                                                        \
      size_t done_padding = total_padding;                                     \
      tmp = done;                                                              \
      while (tmp != 0) {                                                       \
        tmp /= 10;                                                             \
        adj_cols--;                                                            \
        done_padding--;                                                        \
      }                                                                        \
      if (done == 0)                                                           \
        done_padding = total_padding - 1;                                      \
      adj_cols -= done_padding;                                                \
      size_t done_cols = (done * adj_cols / total);                            \
      size_t left_cols = adj_cols - done_cols;                                 \
      printf("\r");                                                            \
      fprintf(stdout, "[");                                                    \
      fprintf(stdout, "\e[32m");                                               \
      for (size_t i = 0; i < done_cols; ++i) {                                 \
        fprintf(stdout, "=");                                                  \
      }                                                                        \
      fprintf(stdout, "\e[31m");                                               \
      for (size_t i = 0; i < left_cols; ++i) {                                 \
        fprintf(stdout, "-");                                                  \
      }                                                                        \
      fprintf(stdout, "\e[0m][");                                              \
      for (size_t _i_ = 0; _i_ < done_padding; ++_i_) {                        \
        fprintf(stdout, " ");                                                  \
      }                                                                        \
      fprintf(stdout, "\e[33m%lu\e[0m/\e[33m%lu\e[0m]", done, total);          \
      fprintf(stdout, "\e[0m");                                                \
      fflush(stdout);                                                          \
    }                                                                          \
  } while (0)

#define finish_progress()                                                      \
  do {                                                                         \
    if (__PROGRESS_BAR_FLAG__) {                                               \
      fprintf(stdout, "\n");                                                   \
      fflush(stdout);                                                          \
    }                                                                          \
  } while (0)

#endif