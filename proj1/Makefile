SOURCE=driver.cpp
OBJECT=driver.o
EXEC=run
CXX:= g++
CXXFLAGS:= -c -Wall -I=.

#------------------------------------------------

all: link
link: compile $(OBJECT)
	$(CXX) -o $(EXEC) $(OBJECT)
compile: $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE)
clean:
	rm -f *.o $(EXEC)
compress: clean
	tar -czvf Prog1Forno_cforno1.tgz ../proj1/*
test: ./traces/*
	./$(EXEC) ./traces/short_trace1.txt output.txt
