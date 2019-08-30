CC= g++ # Compiler to use
OPTIONS= -std=c++0x -O0 -g -Wall # -g for debug, -O2 for optimise and -Wall additional messages
INCLUDES= -I./ # Directory for header file
OBJS= Player.o Board.o Reward.o main.o # List of objects to be build
#.PHONY: all clean # To declare all, clean are not files
	 
all: ${OBJS}
	@echo "Building.." # To print "Building.." message
	@echo ${CC} ${OPTIONS} ${INCLUDES} ${OBJS} -o ttt
	${CC} ${OPTIONS} ${INCLUDES} ${OBJS} -o ttt 
	./ttt
		 
%.o: %.cpp  # % pattern wildcard matching
	@echo "Building file .. $*.cpp " # To print "Building.." message
	${CC} ${OPTIONS} ${INCLUDES} -c $*.cpp 

list:
	@echo $(shell ls) # To print output of command 'ls'
	     
clean:
	@echo "Cleaning up.."
	-rm -rf *.o # - prefix for ignoring errors and continue execution
	-rm ttt
