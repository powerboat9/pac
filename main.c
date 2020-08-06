#include <stdlib.h>
#include <stdio.h>

#define KEYWORD_AUTO		0
#define KEYWORD_BREAK		1
#define KEYWORD_CASE		2
#define KEYWORD_CHAR		3
#define KEYWORD_CONST		4
#define KEYWORD_CONTINUE	5
#define KEYWORD_DEFAULT		6
#define KEYWORD_DO		7
#define KEYWORD_DOUBLE		8
#define KEYWORD_ELSE		9
#define KEYWORD_ENUM		10
#define KEYWORD_EXTERN		11
#define KEYWORD_FLOAT		12
#define KEYWORD_FOR		13
#define KEYWORD_GOTO		14
#define KEYWORD_IF		15
#define KEYWORD_INT		16
#define KEYWORD_LONG		17
#define KEYWORD_REGISTER	18
#define KEYWORD_RETURN		19
#define KEYWORD_SHORT		20
#define KEYWORD_SIGNED		21
#define KEYWORD_SIZEOF		22
#define KEYWORD_STATIC		23
#define KEYWORD_STRUCT		24
#define KEYWORD_SWITCH		25
#define KEYWORD_TYPEDEF		26
#define KEYWORD_UNION		27
#define KEYWORD_UNSIGNED	28
#define KEYWORD_VOID		29
#define KEYWORD_VOLATILE	30
#define KEYWORD_WHILE		31

int starts_with(const char *s, const char *prefix) {
    size_t i = 0;
    while (prefix[i] != '\x00') {
        if (prefix[i] != s[i]) {
            return 0;
        }
    }
    return 1;
}

union TokenReturn {
    char *

int tokenise(const char **data, const)
