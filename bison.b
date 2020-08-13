%{

#define I_FORCE_UNSIGNED 1
#define I_FORCE_LONG 2

struct int_lit {
    unsigned long v;
    int flags;
}

#define F_FLOAT 0
#define F_DOUBLE 1
#define F_LDOUBLE 2

struct float_int {
    long double v;
    int flags;
}

%}

%token	KEY_AUTO	"auto"
%token	KEY_DOUBLE	"double"
%token	KEY_INT		"int"
%token	KEY_STRUCT	"struct"
%token	KEY_BREAK	"break"
%token	KEY_ELSE	"else"
%token	KEY_LONG	"long"
%token	KEY_SWITCH	"switch"
%token	KEY_CASE	"case"
%token	KEY_ENUM	"enum"
%token	KEY_REGISTER	"register"
%token	KEY_TYPEDEF	"typedef"
%token	KEY_CHAR	"char"
%token	KEY_EXTERN	"extern"
%token	KEY_RETURN	"return"
%token	KEY_UNION	"union"
%token	KEY_CONST	"const"
%token	KEY_FLOAT	"float"
%token	KEY_SHORT	"short"
%token	KEY_UNSIGNED	"unsigned"
%token	KEY_CONTINUE	"continue"
%token	KEY_FOR		"for"
%token	KEY_SIGNED	"signed"
%token	KEY_VOID	"void"
%token	KEY_DEFAULT	"default"
%token	KEY_GOTO	"goto"
%token	KEY_SIZEOF	"sizeof"
%token	KEY_VOLATILE	"volatile"
%token	KEY_DO		"do"
%token	KEY_IF		"if"
%token	KEY_STATIC	"static"
%token	KEY_WHILE	"while"

%token	TOKEN_IDENT
%token	TOKEN_STR
%token	TOKEN_CHAR
%token	TOKEN_NUM
%token	TOKEN_FNUM

%union {
    struct int_lit num;
    struct float_lit fnum;
    char *s;
}

%%

%%
