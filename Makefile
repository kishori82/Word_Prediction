# The location of the expat directory
#CPP=g++  -Wall  -fPIC -g  -lm
CPP=gcc  -Wall  -fPIC -g 
LIB=-lm
OPT=

XP_OBJ= main.o utils.o

CFLAGS = $(OPT) $(LIB) 

ngram:  $(XP_OBJ) 
	$(CPP) $(XP_OBJ) $(CFLAGS) -o ngram

main.o: main.c  utils.h
	$(CPP) $(CFLAGS) -c -o main.o  main.c

utils.o: utils.c  utils.h
	$(CPP) $(CFLAGS) -c -o utils.o  utils.c 


clean:	
	rm -rf $(GRAPH_OBJ)
	rm -rf  main 
	rm -rf $(XP_OBJ)
	rm  -rf $(XP_OBJ_AUX)
#rm outline
#rm line

tags : $(shell find . -regextype posix-egrep -regex '.*\.(c|C|h|pl)')
	ctags-exuberant $^
    
# Tue Apr 12 22:21:12 PDT 2011
# Tue Apr 12 22:22:30 PDT 2011
