
sccfinder: sccfinder.cpp
	g++ -O2 -lrt -o sccfinder sccfinder.cpp Node.cpp Node.h

clean:
	rm -f sccfinder sccfinder.o
