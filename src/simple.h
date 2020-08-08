#ifndef SIMPLE_PARSE_HEADER
#define SIMPLE_PARSE_HEADER

struct simple_state {
    // feed
    int holding_back;
    // trig
    short trig_state;
    char cache[2];
    // debug data
    long line_num;
    long next_line;
}

void set_next_line(struct simple_state *state, long next_line) {
    state->next_line = next_line;
}

void init_char_state(struct simple_state *state);

// Returns PARSE_WAIT, PARSE_INVCHAR, or char
int feed_char(struct simple_state *state, char c);

// Returns PARSE_WAIT if requesting feed or char
int pull_char(struct simple_state *state);

// Returns PARSE_WAIT if empty or char
// Means that chars will no longer be fed
int drain_char(struct simple_state *state);

char *read_file(char *filename, )

#endif
