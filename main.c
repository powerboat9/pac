#define SYMBOL_TABLE_LEN (512L * 1024L * 1024L)

char symbol_table[SYMBOL_TABLE_LEN];
size_t symbol_pos = 0;

void symbol_push_char(char c) {
    if (symbol_pos == SYMBOL_TABLE_LEN) {
        fprintf(stderr, "SYM ALLOC FAIL");
        exit(-1);
    } else {
        symbol_table[symbol_pos++] = c;
    }
}

struct token {
    union {
        void *ptr;
        unsigned long data;
    }
    char id;
}

void accept_token(struct token tt) {
}

unsigned long num_store;
int force_long;
int force_str;

#define FLOAT_LIT 0
#define INT_LIT 1
#define STR_LIT 2
#define CHAR_LIT 3

enum parse_state {
    Slash
    MultiComment,
    MultiCommentStar,
    Comment,
    

void accept_input()
