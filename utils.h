#include <stdio.h>   /*All c program must have this*/
#include <string.h>
#include <math.h>
#define bufsize 1024 /*A defined integer for our buffer size*/
#define MAX_NGRAMS 1024 /*A defined integer for our buffer size*/

typedef struct candidates {
    int n;
    char **words;
} CANDIDATES ;


typedef struct line {
    int n;
    char **words;
} LINE ;

typedef struct corpus {
    int n;
    LINE *lines;
} CORPUS ;


CORPUS *get_corpus(char *fname);

 
/* Define an array of critters to sort. */
 
typedef struct keyvalue
 {
     char *key;
     double value;
 } KEY_VALUE_PAIR;
  
typedef struct HASH_TABLE { 
      KEY_VALUE_PAIR *keyvalues;
      int size;
} HASH;

int hash_cmp(const KEY_VALUE_PAIR *a, const KEY_VALUE_PAIR *b );
void print_key_value( const KEY_VALUE_PAIR *);
int find_key_value(HASH *h,  const char *key);


HASH *create_ngram_hash(CORPUS *readcorpus);
HASH * create_hash();

void string_tolower(char *str);
void string_remove_non_alphanumeric(char *str);

char **get_words_array(CORPUS *, int);
int get_num_words(CORPUS *, int);


double get_sentence_score(HASH *, char **, int);

void print_words(char **, int);

char *predict_word(HASH *, char **, int);
char **predict_words(HASH *, char **, int, int);

char * free_word(char *);
char **free_words(char **, unsigned int);

CANDIDATES *get_candidates(char *predword,  HASH *hash, char **words, int num) ;
