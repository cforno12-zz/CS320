SOURCE=predictor.cpp branches.cpp
OBJECT=predictor.o branches.o
EXEC=run
CXX:= g++
CXXFLAGS:= -c -Wall -I=.

#------------------------------------------------

all: link test
link: compile $(OBJECT)
	$(CXX) -o $(EXEC) $(OBJECT)
compile: $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE)
clean:
	rm -f *.o $(EXEC)
compress: clean
	tar -czvf Prog1Forno_cforno1.tgz ../proj1/*
test: ./traces/*
	./$(EXEC) ./traces/long_trace1.txt output.txt
	echo CORRECT OUTPUT FOR LONG TRACE1
	cat ./correct_outputs/long_trace1.txt
	./$(EXEC) ./traces/long_trace2.txt output.txt
	echo CORRECT OUTPUT FOR LONG TRACE2
	cat ./correct_outputs/long_trace2.txt
	./$(EXEC) ./traces/long_trace3.txt output.txt
	echo CORRECT OUTPUT FOR LONG TRACE3
	cat ./correct_outputs/long_trace3.txt
	./$(EXEC) ./traces/short_trace1.txt output.txt
	echo CORRECT OUTPUT FOR SHORT TRACE1
	cat ./correct_outputs/short_trace1.txt
	./$(EXEC) ./traces/short_trace2.txt output.txt
	echo CORRECT OUTPUT FOR SHORT TRACE2
	cat ./correct_outputs/short_trace2.txt
	./$(EXEC) ./traces/short_trace3.txt output.txt
	echo CORRECT OUTPUT FOR SHORT TRACE3
	cat ./correct_outputs/short_trace3.txt
