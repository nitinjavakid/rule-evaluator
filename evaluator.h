#ifndef EVALUATOR_H
#define EVALUATOR_H
typedef enum sym_rec_type
{
  var,
  func
} sym_rec_type_t;

typedef struct sym_rec
{
  sym_rec_type_t type;
  union {
    int var;
    int (*func0)();
    int (*func1)(int);
    int (*func2)(int, int);
    int (*func3)(int, int, int);
  } u;
} sym_rec_t;

#endif
