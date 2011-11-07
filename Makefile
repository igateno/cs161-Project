
sccfinder: sccfinder.cpp
	g++ -lrt -o sccfinder sccfinder.cpp Node.cpp Node.h

clean:
	rm -f sccfinder sccfinder.o
