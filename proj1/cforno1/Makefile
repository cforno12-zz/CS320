SOURCE=predictor.cpp branches.cpp
OBJECT=predictor.o branches.o
EXEC=predictors
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
test01: ./traces/*
	./$(EXEC) ./traces/long_trace1.txt output.txt
test02: ./traces/*
	./$(EXEC) ./traces/long_trace2.txt output.txt
test03: ./traces/*
	./$(EXEC) ./traces/long_trace3.txt output.txt
test04: ./traces/*
	./$(EXEC) ./traces/short_trace1.txt output.txt
test05: ./traces/*
	./$(EXEC) ./traces/short_trace2.txt output.txt
test06: ./traces/*
	./$(EXEC) ./traces/short_trace3.txt output.txt
