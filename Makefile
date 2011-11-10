
sccfinder: sccfinder.cpp
	g++ -O2 -lrt -o sccfinder sccfinder.cpp Node.cpp Node.h
	g++ -O2 -lrt -o randomgraph randomgraph.cpp

clean:
	rm -f sccfinder sccfinder.o randomgraph randomgraph.o
