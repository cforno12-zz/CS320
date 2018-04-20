SRC=driver.cpp caches.cpp cache.h
OBJ=driver.o caches.o
EXEC=cache-sim
CXX=g++
FLAGS= -c -I=. -Wall

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
