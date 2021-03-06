#define SIG_ERR			-1
#define SIG_DONE		-2

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

#define TOKEN_ADD
#define TOKEN_SUB
#define TOKEN_MUL
#define TOKEN_DIV
#define TOKEN_-

struct parse_state {
}

// Handles control lines, line count, tokenisation, etc.

#define CTRL_LINESTART
#define CTRL_PARSING_IDENT
#define CTRL_PARSING_NDEC
#define CTRL_PARSING_NZERO
#define CTRL_PARSING_NOCT
#define CTRL_PARSING_NHEX
#define CTRL_PARSING_HAD_PLUS
#define CTRL_PARSING_HAD_MINUS
#define CTRL_PARSING_HAD_STAR
#define CTRL_PARSING_HAD_

struct ctrl_state {
    struct parse_state next;
    long line;
    long next_line;
    int state;
    union {
    } state_data;
}

void start_ctrl(struct ctrl_state *state) {
}

void feed_ctrl(struct ctrl_state *state, char c) {
}

void end_ctrl(struct ctrl_state *state) {
}

// Handles line splicing

struct splice_state {
    struct ctrl_state next;
    int holding_back;
}

void start_splice(struct splice_state *state) {
    state->holding_back = 0;
    start_ctrl(&state->next);
}

void feed_splice(struct splice_state *state, char c) {
    // Handles line splicing continuation
    if (splice_state->holding_back) {
        switch (c) {
            default:
                feed_ctrl(&state->next, '\\');
                feed_ctrl(&state->next, c);
            case '\n':
                state->holding_back = 0;
                return;
            case '\\':
                feed_ctrl(&state->next, '\\');
        }
    } else {
        if (c == '\\') {
            splice_state->holding_back = 1;
        } else {
            feed_ctrl(&state->next, c);
        }
    }
}

void end_splice(struct splice_state *state) {
    if (state->holding_back) {
        feed_ctrl(&state->next, '\\');
    }
    end_ctrl(&state->next);
}

// Handle trigraph sequences

struct trig_state {
    struct splice_state next;
    int cnt;
}

void start_trig(struct trig_state *state) {
    state->cnt = 0;
    start_splice(&state->next);
}

void feed_trig(struct trig_state *state, char c) {
    switch (state->cnt) {
        case 0:
            if (c == '?') {
                state->cnt = 1;
            } else {
                feed_splice(&state->next, c);
            }
            return;
        case 1:
            if (c == '?') {
                state->cnt = 2;
            } else {
                feed_splice(&state->next, '?');
                feed_splice(&state->next, c);
                state->cnt = 0;
            }
            return;
        case 2:
            switch (c) {
                case '=':
                    feed_splice(&state->next, '#');
                    state->cnt = 0;
                    return;
                case '/':
                    feed_splice(&state->next, '\\');
                    state->cnt = 0;
                    return;
                case '\'':
                    feed_splice(&state->next, '^');
                    state->cnt = 0;
                    return;
                case '(':
                    feed_splice(&state->next, '[');
                    state->cnt = 0;
                    return;
                case ')':
                    feed_splice(&state->next, ']');
                    state->cnt = 0;
                    return;
                case '!':
                    feed_splice(&state->next, '|');
                    state->cnt = 0;
                    return;
                case '<':
                    feed_splice(&state->next, '{');
                    state->cnt = 0;
                    return;
                case '>':
                    feed_splice(&state->next, '}');
                    state->cnt = 0;
                    return;
                case '-':
                    feed_splice(&state->next, '~');
                    state->cnt = 0;
                    return;
                case '?':
                    feed_splice(&state->next, '?');
                    return;
                default:
                    feed_splice(&state->next, '?');
                    feed_splice(&state->next, '?');
                    feed_splice(&state->next, c);
                    state->cnt = 0;
                    return;
            }
    }
}

void end_trig(struct trig_state *state) {
    switch (state->cnt) {
        case 1:
            feed_splice(&state->next, '?');
        case 2:
            feed_splice(&state->next, '?');
    }
    end_splice(&state->next);
}

// Handle input

void parse_input(char *buf, size_t len) {
    struct trig_state state;
    start_trig(&state);
    for (size_t i = 0; i < len; i++) {
        feed_trig(&state, buf[i]);
    }
    end_trig(&state);
}
