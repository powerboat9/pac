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

int read_num_flags() {
    char c = input();
    if (c == "U" || c == "u") {
        c = input();
        if (c == "L" || c == "l") {
            return I_FORCE_UNSIGNED | I_FORCE_LONG;
        } else {
            unput(c);
            return I_FORCE_UNSIGNED;
        }
    } else {
        unput(c);
        return 0;
    }
}

%}

WSPACE		[ \n\t\r]

%pointer

%x comment
%x string
%x charlit

%%
"/*"		BEGIN(comment)
<comment>"*/"	BEGIN(INITIAL)
<comment>.	/* ignore */
<comment><<EOF>>	{
    error("unterminated comment");
    yyterminate();
}

"//"[^\n]*	/* ignore */

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
    BEGIN(INITIAL)
    return TOKEN_STR;
}
<string><<EOF>>	{
    error("unterminated string");
    yyterminate();
}

\'		{
    is_char_lit = 1;
    BEGIN(charlit)
}
<charlit>\'	{
    is_char_lit = 0;
    yyval.num = int_acc;
    BEGIN(INITIAL)
    return TOKEN_CHAR;
}
<charlit><<EOF>>	{
    error("unterminated character literal");
    yyterminate();
}

"auto"		{return KEY_AUTO;}
"double"	{return KEY_DOUBLE;}
"int"		{return KEY_INT;}
"struct"	{return KEY_STRUCT;}
"break"		{return KEY_BREAK;}
"else"		{return KEY_ELSE;}
"long"		{return KEY_LONG;}
"switch"	{return KEY_SWITCH;}
"case"		{return KEY_CASE;}
"enum"		{return KEY_ENUM;}
"register"	{return KEY_REGISTER;}
"typedef"	{return KEY_TYPEDEF;}
"char"		{return KEY_CHAR;}
"extern"	{return KEY_EXTERN;}
"return"	{return KEY_RETURN;}
"union"		{return KEY_UNION;}
"const"		{return KEY_CONST;}
"float"		{return KEY_FLOAT;}
"short"		{return KEY_SHORT;}
"unsigned"	{return KEY_UNSIGNED;}
"continue"	{return KEY_CONTINUE;}
"for"		{return KEY_FOR;}
"signed"	{return KEY_SIGNED;}
"void"		{return KEY_VOID;}
"default"	{return KEY_DEFAULT;}
"goto"		{return KEY_GOTO;}
"sizeof"	{return KEY_SIZEOF;}
"volatile"	{return KEY_VOLATILE;}
"do"		{return KEY_DO;}
"if"		{return KEY_IF;}
"static"	{return KEY_STATIC;}
"while"		{return KEY_WHILE;}

[A-Za-z_][A-Za-z0-9_]	{
    yyval.s = strdup(yytext);
    return TOKEN_IDENT;
}

0[xX][0-9a-fA-F]+[uU]?[lL]?	{
    char *end;
    yyval.num.v = strtoul(yytext, &end, 16);
    yyval.num.flags = read_num_flags();
    return TOKEN_NUM;
}

0[0-7]*		{
    yyval.num.v = strtoul(yytext, NULL, 8);
    yyval.num.flags = read_num_flags();
    return TOKEN_NUM;
}

[1-9][0-9]*	{
    yyval.num.v = strtoul(yytext, NULL, 10);
    yyval.num.flags = read_num_flags();
    return TOKEN_NUM;
}

[1-9][0-9]*\.
%%
