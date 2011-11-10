
sccfinder: sccfinder.cpp
	g++ -O2 -lrt -o sccfinder sccfinder.cpp Node.cpp Node.h
	g++ -O2 -lrt -o randomgraph randomgraph.cpp
	g++ -O2 -lrt -o randomSCC randomSCC.cpp Node.cpp Node.h
clean:
	rm -f sccfinder sccfinder.o randomgraph randomgraph.o randomSCC randomSCC.o
