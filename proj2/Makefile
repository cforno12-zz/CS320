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
	rm -f $(EXEC) $(OBJ)

compress:
	cd ../
	mv ./proj2 ./cforno1
	tar -czvf cforno1.tar.gz cforno1/

test: ./traces
	./$(EXEC) ./traces/trace2.txt output.txt
	cat output.txt
	cat ./correct_outputs/trace2_output.txt
