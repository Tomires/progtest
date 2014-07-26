CC=g++
FL=-ansi -pedantic -Wall -Wextra -static -Wno-long-long -O0 -ggdb -std=c++11 -o havlito5

all: compile doc
compile: cDate.o cDbl.o cInt.o cObj.o cRow.o cVarchar.o edble.o enum.o main.o tableWorks.o
	$(CC) $(FL) cDate.o cDbl.o cInt.o cObj.o cRow.o cVarchar.o edble.o enum.o main.o tableWorks.o -o havlito5

run: 
	./havlito5

clean: 
	rm -rf *.o havlito5 doc

doc: 
	doxygen Doxyfile

cDate.o: 
	$(CC) $(FL) -c src/cDate.cpp -o cDate.o
cDbl.o: 
	$(CC) $(FL) -c src/cDbl.cpp -o cDbl.o
cInt.o: 
	$(CC) $(FL) -c src/cInt.cpp -o cInt.o
cObj.o: 
	$(CC) $(FL) -c src/cObj.cpp -o cObj.o
cRow.o: 
	$(CC) $(FL) -c src/cRow.cpp -o cRow.o
cVarchar.o: 
	$(CC) $(FL) -c src/cVarchar.cpp -o cVarchar.o
edble.o: 
	$(CC) $(FL) -c src/edble.cpp -o edble.o
enum.o: 
	$(CC) $(FL) -c src/enum.cpp -o enum.o
main.o: 
	$(CC) $(FL) -c src/main.cpp -o main.o
tableWorks.o: 
	$(CC) $(FL) -c src/tableWorks.cpp -o tableWorks.o
