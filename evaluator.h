#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <vector>
#include <inttypes.h>

typedef struct sym_rec
{
  int type;
  union {
    int64_t var;
    int64_t (*func0)();
    int64_t (*func1)(int64_t);
    int64_t (*func2)(int64_t, int64_t);
    int64_t (*func3)(int64_t, int64_t, int64_t);
  } u;

  sym_rec();
} sym_rec_t;

#endif
