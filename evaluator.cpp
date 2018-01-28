#include <ctype.h>
#include <cstring>
#include <string>
#include <unordered_map>
#include <evaluator.h>
#include <parser.hpp>

std::unordered_map<std::string, sym_rec_t> s_symbols;

int
yylex(void)
{
  int c;
  while((c = getchar()) == ' ' || c == '\t')
    continue;

  if(isdigit(c))
  {
    ungetc(c, stdin);
    scanf("%d", &yylval);
    return NUM;
  }
  else if(isalpha(c))
  {
    char op[50];
    ungetc(c, stdin);
    scanf("%49[a-zA-Z0-9]", op);
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
      *((sym_rec_t **) &yylval) = &(s_symbols[op]);
      return VAR;
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

int main(int argc, const char **argv)
{
  for(int idx = 1; idx < argc; ++idx)
  {
    char name[50];
    int value;
    sscanf(argv[idx], "%49[^=]=%d", name, &value);
    name[49] = '\0';
    sym_rec_t &symbol = s_symbols[name];
    symbol.type = var;
    symbol.u.var = value;
  }

  return yyparse();
}
