PROJECT = mysync
HEADERS = $(PROJECT).h
OBJ     = mysync.o globals.o patternmatch.o filemaking.o directorychecking.o

C11     = cc -std=c11
CFLAGS  = -Wall -Werror

$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ) -lm

%.o : %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)
