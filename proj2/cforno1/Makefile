SRC=driver.cpp caches.cpp
OBJ=driver.o caches.o
EXEC=cache-sim
CXX=g++
FLAGS= -c -I=. -Wall -g

all: link

link: compile $(OBJ)
	$(CXX) -o $(EXEC) $(OBJ)

compile: $(SRC)
	$(CXX) $(FLAGS) $(SRC)

clean:
	rm -rf $(EXEC) $(OBJ) *.dSYM

compress:
	cd ../
	mv ./proj2 ./cforno1
	tar -czvf cforno1.tar.gz cforno1/

test: ./traces
	./$(EXEC) ./traces/trace2.txt output.txt
	cat output.txt
	cat ./correct_outputs/trace2_output.txt

test_all: ./traces
	./$(EXEC) ./traces/trace1.txt output1.txt
	./$(EXEC) ./traces/trace2.txt output2.txt
	./$(EXEC) ./traces/trace3.txt output3.txt


valgrind:
	valgrind --leak-check=yes ./cache-sim ./traces/trace2.txt output.txt
