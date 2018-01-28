#include <ctype.h>
#include <cstring>
#include <string>
#include <unordered_map>
#include <evaluator.h>
#include <parser.hpp>

std::unordered_map<std::string, sym_rec_t> s_symbols;

sym_rec::sym_rec()
  : type(VAR),
    u{0}
{
}

int
yylex(void)
{
  int c;
  while((c = getchar()) == ' ' || c == '\t')
    continue;

  if(isdigit(c))
  {
    ungetc(c, stdin);
    scanf("%" PRId64, &yylval);
    return NUM;
  }
  else if(isalpha(c))
  {
    char op[50];
    ungetc(c, stdin);
    scanf("%49[a-zA-Z0-9_]", op);
    op[49] = '\0';
    if(strcasecmp(op, "and") == 0)
    {
      return AND;
    }
    else if(strcasecmp(op, "or") == 0)
    {
      return OR;
    }
    else if(strcasecmp(op, "not") == 0)
    {
      return NOT;
    }
    else
    {
      sym_rec_t &s = s_symbols[op];
      *((sym_rec_t **) &yylval) = &(s);
      return s.type;
    }
  }
  else if(c != EOF && ((c == '>') || (c == '<') || (c == '=')))
  {
    int c1 = getchar();
    if(c1 == EOF)
    {
      return c;
    }

    switch(c)
    {
    case '=':
    {
      if(c1 == '=')
      {
        return EQ;
      }
      else
      {
        ungetc(c1, stdin);
        return c;
      }
    }
    case '<':
    {
      if(c1 == '=')
      {
        return LE;
      }
      else
      {
        ungetc(c1, stdin);
        return LT;
      }
    }
    case '>':
    {
      if(c1 == '=')
      {
        return GE;
      }
      else
      {
        ungetc(c1, stdin);
        return GT;
      }
    }
    default:
      ungetc(c1,stdin);
    }
  }

  if(c == EOF)
    return 0;

  return c;
}

void yyerror(const char *err)
{
  printf(err);
}

#define FUNC0(name, f)                    \
  {                                       \
    sym_rec_t &_s = s_symbols[name];      \
    _s.type = FUNC;                       \
    _s.u.func0 = f;                       \
  }

#define FUNC1(name, f)                    \
  {                                       \
    sym_rec_t &_s = s_symbols[name];      \
    _s.type = FUNC;                       \
    _s.u.func1 = f;                       \
  }

#define FUNC2(name, f)                    \
  {                                       \
    sym_rec_t &_s = s_symbols[name];      \
    _s.type = FUNC;                       \
    _s.u.func2 = f;                       \
  }

#define FUNC3(name, f)                    \
  {                                       \
    sym_rec_t &_s = s_symbols[name];      \
    _s.type = FUNC;                       \
    _s.u.func3 = f;                       \
  }

int main(int argc, const char **argv)
{
  for(int idx = 1; idx < argc; ++idx)
  {
    char name[50];
    int64_t value;
    sscanf(argv[idx], "%49[^=]=%" PRId64, name, &value);
    name[49] = '\0';
    sym_rec_t &symbol = s_symbols[name];
    symbol.type = VAR;
    symbol.u.var = value;
  }

  FUNC0("date_time", []() -> int64_t {
      return time(NULL);
  });

  FUNC1("date_hour", [](int64_t time) -> int64_t {
      time_t rawtime = time;
      struct tm *ptm;
      ptm = gmtime(&rawtime);
      return ptm->tm_hour;
  });

  FUNC1("date_min", [](int64_t time) -> int64_t {
      time_t rawtime = time;
      struct tm *ptm;
      ptm = gmtime(&rawtime);
      return ptm->tm_min;
  });

  FUNC1("date_second", [](int64_t time) -> int64_t {
      time_t rawtime = time;
      struct tm *ptm;
      ptm = gmtime(&rawtime);
      return ptm->tm_sec;
  });

  FUNC1("date_month_day", [](int64_t time) -> int64_t {
      time_t rawtime = time;
      struct tm *ptm;
      ptm = gmtime(&rawtime);
      return ptm->tm_mday;
  });

  FUNC1("date_week_day", [](int64_t time) -> int64_t {
      time_t rawtime = time;
      struct tm *ptm;
      ptm = gmtime(&rawtime);
      return ptm->tm_wday;
  });

  FUNC1("date_year_day", [](int64_t time) -> int64_t {
      time_t rawtime = time;
      struct tm *ptm;
      ptm = gmtime(&rawtime);
      return ptm->tm_yday;
  });

  FUNC1("date_month", [](int64_t time) -> int64_t {
      time_t rawtime = time;
      struct tm *ptm;
      ptm = gmtime(&rawtime);
      return ptm->tm_mon;
  });

  FUNC2("date_offset", [](int64_t hours, int64_t mins) {
      return 60*hours*60 + mins * 60;
  });

  return yyparse();
}
