#include <stdio.h>   /*All c program must have this*/
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#define   DEBUG
#undef   DEBUG


#define DEBUG1



int main() {
   CORPUS *readcorpus;
   char *word, **words;

   readcorpus= get_corpus("training_set.txt");
   HASH *hash;
   hash = create_ngram_hash(readcorpus);

#ifdef  DEBUG1
   int i;
   for(i=0; i< hash->size; i++) {
     printf("%s -> %lf\n", hash->keyvalues[i].key, hash->keyvalues[i].value);
   }
   printf("\n");
#endif


   char **wordsarray; 
   int numwords;


   printf("Reading the test sentences ......" );
   CORPUS *testcorpus = get_corpus("test_set.txt");
   printf("done\n");

  // double sentencescore = get_sentence_score(hash, wordsarray, numwords);
   
  // printf("Num words = %d\n", numwords);
 //  printf("Sentence score = %lf\n", sentencescore);
  
  // exit(0);
  /* let's test sentence one in the test set fiile */
//   printf("Predicting for the 1st sentences ......" );
   printf("1. Testing the 1st sentence ......\n" );
   wordsarray= get_words_array(testcorpus, 1); 
   //printf("done\n");


   //printf("Number of words ......" );
   numwords=get_num_words(testcorpus, 1); 
  // printf("done\n");
   word = predict_word(hash, wordsarray, numwords );
   print_words(wordsarray, numwords);
   printf("<<%s>>\n", word);

   wordsarray=free_words(wordsarray, numwords);
   word=free_word(word);
 //  printf("done\n");
   
  /* let's test sentence two in the test set fiile */
   printf("2. Testing the 2nd sentence ......\n" );
   wordsarray= get_words_array(testcorpus, 2); 
   numwords=get_num_words(testcorpus, 2); 
   word= predict_word(hash, wordsarray, numwords );
   print_words(wordsarray, numwords);
   printf("<<%s>>\n", word);

   word=free_word(word);
   wordsarray=free_words(wordsarray, numwords);


   printf("3. Getting multiple words....(ramdomized selection based on statistical distribution)\n");
   wordsarray= get_words_array(testcorpus, 1); 
   numwords=get_num_words(testcorpus, 2); 
   print_words(wordsarray, numwords);
   int num_pred_words= 10;
   words=predict_words(hash, wordsarray, numwords, num_pred_words);

   printf("<<%s", words[0]);
   for(i=1; i< num_pred_words-1; i++)
   printf("/%s", words[i]);
   printf("/%s>>\n", words[num_pred_words-1]);

   words=free_words(words, 3);
   wordsarray=free_words(wordsarray, numwords);
   //printf("%s\n", words[1]);
   
   //printf("\n");
   return(0);
}
