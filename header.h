

#include <stdio.h>
#include<string.h>
#include<ctype.h>
#include <stdbool.h>
#include<stdlib.h>
#include<time.h>

#define WORD_LEN  30
#define MAX_WORDS 1000
#define MAX_SUGGEST 50
#define WORD_DIST_LIMIT 1
#define INITIAL_VAL 0

extern int correct_spell, wrong_spell;
extern struct timespec start, stop;


int wagner_fischer(const char*, const char*);
void spell_suggest(FILE*, const  char*);

void normalize_word(char *) ;
void to_lower(char*);
char* upper_case(char *);

void word_analyze(FILE *, FILE *);