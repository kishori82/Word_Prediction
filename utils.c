#include <stdio.h>   /*All c program must have this*/
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "utils.h"

char * free_word(char *word) {
    if(word!=NULL) free(word);
    return(NULL);
}

char **free_words(char **words, unsigned int num) {
    int i;
    for(i=0; i< num; i++ ) {
       if(words[i]!=NULL) free(words[i]);
    }
    if(words!=NULL) free(words);
    return(NULL);
}

short int hasalphanumeric(char *tok) {
    while( tok!=NULL && *tok !='\0' ) {
        if(isalnum(*tok) )  return(1);
        ++tok;
    }
    return(0);

}
CORPUS *get_corpus( char *fname){          /*Program entry point*/
     FILE* fp;       /*Declare file pointer variable*/
     char buf[bufsize], tempbuf[bufsize], *line, *tok;
     CORPUS *readcorpus;
     short int numwords;
     short int numlines;
     short int wcount, lcount;
     
     /*If file doesn't exist or filetype isn't allowed exit and*/
     /*error message & return (1) control to the OS*/
     if ((fp = fopen(fname,"rt")) == NULL){ 
             fprintf(stderr,"Error opening file: %s\n",fname);
             return NULL;
      }
      
     /*Read into the buffer contents within thr file stream*/
     readcorpus=(CORPUS *)malloc(sizeof(CORPUS));
    
          /* count the number of lines */
     numlines=0; 
     while(fgets(buf, bufsize, fp) != NULL){
       numlines++;
     }
     rewind(fp);

     readcorpus->n=numlines;
     readcorpus->lines=(LINE *) malloc(readcorpus->n*sizeof(LINE));

     lcount=0;
     while(fgets(buf, bufsize, fp) != NULL){
          strncpy(tempbuf, buf, strlen(buf));
          line=strtok(tempbuf,"\n");
         
          /* count the number of words */
          tok=strtok(line," ");
          numwords=0;
          while( tok !=NULL ) {
             if(hasalphanumeric(tok)) numwords++;
             tok=strtok(NULL," ");
#ifdef DEBUG
             if(hasalphanumeric(tok)) 
                printf("Token = <<%s>>\n", tok);
#endif
          }
          
          readcorpus->lines[lcount].n = numwords;
          readcorpus->lines[lcount].words = (char **)malloc(numwords*sizeof(char *));
           
         /* start filling the words in the current line */
          wcount=0;
          strncpy(tempbuf, buf, strlen(buf));
          line=strtok(tempbuf,"\n");
          tok=strtok(line," ");
         
          while( tok!=NULL)  {
             readcorpus->lines[lcount].words[wcount]=(char *)malloc((strlen(tok))*sizeof(char));
             strncpy( readcorpus->lines[lcount].words[wcount], tok, strlen(tok));
#ifdef DEBUG
             printf("%s   %d\n", readcorpus->lines[lcount].words[wcount],  strlen(tok));
#endif
             tok=strtok(NULL," ");
             wcount++;
          }
          lcount++;
           
     }/*Continue until EOF is encoutered*/
     fclose(fp); /*Close file*/
     return readcorpus; /*Executed without errors*/
}/*End main*/





/* This is the comparison function used for sorting and searching. */

int hash_cmp(const  KEY_VALUE_PAIR *c1, const KEY_VALUE_PAIR *c2)
{
  return strcmp (c1->key, c2->key);
}

/* Print information about a key value pair. */
void print_key_value (const KEY_VALUE_PAIR *c)
{
  printf ("%s, the %lf\n", c->key, c->value);
}


/* Do the lookup into the sorted array. */
int find_key_value(HASH *hash, const char *key)
{
  int i;
  //result = bsearch(&target, hash, count, sizeof (KEY_VALUE_PAIR), hash_cmp);
  for(i=0 ; i<  hash->size; i++ ) {
      if(strcmp(key, hash->keyvalues[i].key) == 0 ) return(i);

  }
  return (-1);
}


int insert_key_value(HASH *hash, KEY_VALUE_PAIR keyvalue) {
     int loc;
     string_tolower(keyvalue.key);
     string_remove_non_alphanumeric(keyvalue.key);

     loc = find_key_value(hash, keyvalue.key);

     if( loc >= 0 )  {
          (hash->keyvalues)[loc].value++;
     }
     else {
          hash->keyvalues[hash->size].key =(char * )malloc((strlen(keyvalue.key)+1)*sizeof(char));
          strncpy(hash->keyvalues[hash->size].key, keyvalue.key, strlen(keyvalue.key));
          hash->keyvalues[hash->size].key[strlen(keyvalue.key)] ='\0';
          //strcpy(hash->keyvalues[hash->size].key, keyvalue.key);
#ifdef DEBUG
          printf("%s(%d)  %s(%d)\n", keyvalue.key, strlen(keyvalue.key), hash->keyvalues[hash->size].key, strlen(hash->keyvalues[hash->size].key));
#endif
          hash->keyvalues[hash->size].value= 1;
          hash->size++;
     }
     
     return 1;
}

void string_remove_non_alphanumeric( char *str) {
   char *s, *t;
   s=str;
   t=str;

   while(*s!='\0') {
       if( (*s >= '0' && *s <='9') || (*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <='Z') || (*s=='_')) {
           *t=*s;
            t++;
       }
       s++;
   }
   *t='\0';
}



void string_tolower( char *str) {
   short int i;
   for(i=0; i< strlen(str); i++ ) {
       str[i] = tolower(str[i]);
   }
}


HASH * create_hash() {
     HASH *hash;
     int i;
     hash = (HASH *) malloc( sizeof(HASH));
     hash->keyvalues=(KEY_VALUE_PAIR *)malloc( MAX_NGRAMS*sizeof(KEY_VALUE_PAIR)); 
     hash->size =0;
     for(i=0; i< MAX_NGRAMS; i++ ) {
          hash->keyvalues[i].value = 0;
     }
     return hash; 
}


HASH * create_ngram_hash( CORPUS *readcorpus) {
     unsigned short nlines, nwords;
     KEY_VALUE_PAIR keyval;
     HASH *hash=create_hash(); 
     unsigned short i,j;

     nlines = readcorpus->n;
     for( i=0; i< nlines; i++) {
        //printf("%d has %d words\n", i, readcorpus->lines[i].n);
        nwords =  readcorpus->lines[i].n;
        for(j=0; j< nwords-1; j++) {
              keyval.key = (char *)malloc((strlen(readcorpus->lines[i].words[j])+strlen(readcorpus->lines[i].words[j+1])+1)*sizeof(char));
              sprintf(keyval.key, "%s_%s", readcorpus->lines[i].words[j], readcorpus->lines[i].words[j+1]);
             // printf("%s\n",keyval.key);
              insert_key_value( hash, keyval);
#ifdef DEBUG0
              printf("%s_%s \n", readcorpus->lines[i].words[j], readcorpus->lines[i].words[j+1]);
#endif
        }

   }
   return hash;

}

char **get_words_array(CORPUS *corpus, int line) {
    line = line-1;
    char **words;
    int i;
    words = (char **)malloc(sizeof(get_num_words(corpus, line))*sizeof(char *));
    for(i=0; i< get_num_words(corpus, line); i++ ) {
      //  printf("Number of words lin the line = %d\n", get_num_words(corpus, line));
       // printf("Allocating for = %s\n", corpus->lines[line].words[i]);
        //printf("Allocating bytes = %d\n", strlen(corpus->lines[line].words[i]));
        words[i]= (char *)malloc(strlen(corpus->lines[line].words[i])*sizeof(char));
        strncpy(words[i],corpus->lines[line].words[i], strlen(corpus->lines[line].words[i]));
        words[i][strlen(corpus->lines[line].words[i])]='\0';
    }
    return words;
}

int  get_num_words(CORPUS *corpus, int line) {
     if( corpus->n < line ) return 0;
     return( corpus->n);

}

double get_sentence_score(HASH *hash, char **wordsarray, int numwords ) {
    return( 1 - exp(-numwords));

}

void print_words(char **words, int num) {
    int i;
    for(i=0; i< num;  i++ ) {
        printf("%s ", words[i]);
    }
}


char *predict_word( HASH *hash, char **words, int num) {
    CANDIDATES *cands;
    char *predword;
    short int sel_cand_num;

    predword= (char *)malloc(sizeof(char));
    if( num < 1 ) { predword[0] = '\0'; }

    char *precword = (char *)malloc(1+strlen(words[num-1])*sizeof(char));
    
    sprintf(precword,"%s_",words[num-1]);
    string_tolower(precword);

    cands = get_candidates(precword,  hash, words,  num) ;

    sel_cand_num=  rand()%(cands->n);

    if(cands->n > 0 ) {
        predword= (char *)malloc( (strlen(cands->words[sel_cand_num])+1)*sizeof(char));
        strncpy(predword,cands->words[sel_cand_num], strlen(cands->words[sel_cand_num]));
        predword[strlen(cands->words[sel_cand_num])]='\0';
    }

    //srand(time(NULL));
    //if(cands->n > 0 )
   // printf(" <<%s>> ", cands->words[rand()%cands->n]);
    //printf("\n");
    return( predword);

   
}

char **predict_words(HASH *hash, char **words, int num, int num_word_predict) {
    CANDIDATES *cands;
    char **predword;
    int i;
    short int sel_cand_num;
    if( num < 1 ) { return(NULL); }

    char *precword = (char *)malloc(1+strlen(words[num-1])*sizeof(char));
    sprintf(precword,"%s_",words[num-1]);
    string_tolower(precword);

    cands = get_candidates(precword,  hash, words,  num) ;

    if(cands->n <= 0 ) {
       return(NULL);
    }

    predword = (char **) malloc(num_word_predict*sizeof(char *));

    srand(time(NULL));
    for(i=0; i< num_word_predict; i++) {
       sel_cand_num=  rand()%(cands->n);
      //  if(cands->n > 0 ) printf(" <<%s>> ", cands->words[sel_cand_num]);
        predword[i]= (char *)malloc( (strlen(cands->words[sel_cand_num])+1)*sizeof(char));
        strncpy(predword[i],cands->words[sel_cand_num], strlen(cands->words[sel_cand_num]));
        predword[i][strlen(cands->words[sel_cand_num])]='\0';
    }
   
   // printf("\n");
    return( predword);

   
}


CANDIDATES *get_candidates(char *precword,  HASH *hash, char **words, int num) {
    CANDIDATES *cands;
    int numcands=0;
    int i,j;
    for(i=0; i< hash->size;  i++ ) {
      //  printf("%s   %s   %d\n ",precword, hash->keyvalues[i].key, strncmp(hash->keyvalues[i].key, precword, strlen(precword)));
        if( strncmp(hash->keyvalues[i].key, precword, strlen(precword))==0) {
            numcands = numcands+ hash->keyvalues[i].value;     
        }
    }
    cands=(CANDIDATES *)malloc( sizeof(CANDIDATES) );
    cands->n= numcands;
    cands->words = (char **) malloc(cands->n*sizeof(char *));
    numcands = 0;
    for(i=0; i< hash->size;  i++ ) {
        if( strncmp(hash->keyvalues[i].key, precword, strlen(precword))==0) {
            for(j=0; j< hash->keyvalues[i].value; j++ ) {
                cands->words[numcands] = (char *) malloc( (strlen(hash->keyvalues[i].key) - strlen(precword))*sizeof(char));     
                sprintf(cands->words[numcands],"%s", hash->keyvalues[i].key + strlen(precword));
     //           printf("%s", cands.words[numcands]);
                numcands++;
            }
           
        }
    }

    return(cands);
   
}



