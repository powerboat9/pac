#include <stdlib.h>
#include <stdio.h>

#include "simple.h"

void init_char_state(struct simple_state *state) {
    state->holding_back = 0;
    state->cnt = 0;
    state->line_num = 1;
    state->next_line = -1;
}

int trig_feed_char(struct simple_state *state, char c) {
    switch (state->trig_state) {
        case 0:
            if (c == '?') {
                state->trig_state = 1;
                return PARSE_WAIT;
            } else {
                return (int) c;
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

// 0 on success and no affect on errno, -1 on error and sets errno
int run_subs(char *buf, size_t len) {
    size_t i = 0;
    2
