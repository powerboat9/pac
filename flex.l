%{
#include "y.tab.h"

static int is_char_lit = 0;

static int int_acc = 0;

static struct {
    char *ptr;
    size_t len;
    size_t size;
} string_builder = {
    .ptr = NULL;
    .len = 0;
    .size = 0;
};

void push_cs_lit(char c) {
    if (is_char_lit) {
        int_acc <<= 8;
        int_acc |= c;
    } else {
        push_string_builder(c);
    }
}

void push_string_builder(char c) {
    if (string_builder.size == 0) {
        string_builder.ptr = (char *) malloc(16);
        string_builder.size = 16;
    }
    if (string_builder.len == string_builder.size) {
        string_builder.size *= 2;
        string_builder.ptr = (char *) realloc(string_builder.ptr, string_builder.size);
    }
    string_builder.ptr[string_builder.len++] = c;
}

void clean_string_builder() {
    if (string_builder.ptr != NULL) {
        free(string_builder.ptr);
        string_builder.ptr = NULL;
        string_builder.len = 0;
        string_builder.size = 0;
    }
}

char *finish_string_builder() {
    char *ret;
    if (string_builder.ptr == NULL) {
        char *ret = (char *) malloc(1);
        *ret = '\0';
    } else {
        if (string_builder.size == string_builder.len) {
            ret = (char *) realloc(string_builder.ptr, string_builder.size + 1);
        } else {
            ret = string_builder.ptr;
        }
        ret[string_builder.len] = '\0';
        string_builder.ptr = NULL;
        string_builder.len = 0;
        string_builder.size = 0;
    }
    return ret;
}

%}

%pointer

%union {
    unsigned long i_ul;
    long i_l;
    unsigned int i_u;
    int i;
    char *s;
}

%x comment
%x string
%x charlit

%%
"/*"		BEGIN(comment)
<comment>"*/"	BEGIN(INITIAL)
<comment>.	/* ignore */

<string,charlit>\\n	{push_cs_lit('\n');}
<string,charlit>\\t	{push_cs_lit('\t');}
<string,charlit>\\v	{push_cs_lit('\v');}
<string,charlit>\\b	{push_cs_lit('\b');}
<string,charlit>\\r	{push_cs_lit('\r');}
<string,charlit>\\f	{push_cs_lit('\f');}
<string,charlit>\\a	{push_cs_lit('\a');}
<string,charlit>\\\\	{push_cs_lit('\\');}
<string,charlit>\\\?	{push_cs_lit('\?');}
<string,charlit>\\\'	{push_cs_lit('\'');}
<string,charlit>\\\"	{push_cs_lit('"');}
<string,charlit>\\[0-7]{1,3}		{push_cs_lit((char) strtol(yytext + 1, NULL, 8));}
<string,charlit>\\x[0-9A-Fa-f]+		{push_cs_lit((char) strtol(yytext + 2, NULL, 16));}
<string,charlit>.	{push_cs_lit(*yytext);}

\"		BEGIN(string)
<string>\"	{
    yyval.s = finish_string_builder();
    return SLIT;
}

\'		{
    is_char_lit = 1;
    BEGIN(charlit)
}
<charlit>\'	{
    yyval.i = int_acc;
    BEGIN(INITIAL)
}

"auto"		{return TOKEN_AUTO;}
"double"	{return TOKEN_DOUBLE;}
"int"		{return TOKEN_INT;}
"struct"	{return TOKEN_STRUCT;}
"break"		{return TOKEN_BREAK;}
"else"		{return TOKEN_ELSE;}
"long"		{return TOKEN_LONG;}
"switch"	{return TOKEN_SWITCH;}
"case"		{return TOKEN_CASE;}
"enum"		{return TOKEN_ENUM;}
"register"	{return TOKEN_REGISTER;}
"typedef"	{return TOKEN_TYPEDEF;}
"char"		{return TOKEN_CHAR;}
"extern"	{return TOKEN_EXTERN;}
"return"	{return TOKEN_RETURN;}
"union"		{return TOKEN_UNION;}
"const"		{return TOKEN_CONST;}
"float"		{return TOKEN_FLOAT;}
"short"		{return TOKEN_SHORT;}
"unsigned"	{return TOKEN_UNSIGNED;}
"continue"	{return TOKEN_CONTINUE;}
"for"		{return TOKEN_FOR;}
"signed"	{return TOKEN_SIGNED;}
"void"		{return TOKEN_VOID;}
"default"	{return TOKEN_DEFAULT;}
"goto"		{return TOKEN_GOTO;}
"sizeof"	{return TOKEN_SIZEOF;}
"volatile"	{return TOKEN_VOLATILE;}
"do"		{return TOKEN_DO;}
"if"		{return TOKEN_IF;}
"static"	{return TOKEN_STATIC;}
"while"		{return TOKEN_WHILE;}
%%
