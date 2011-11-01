
sccfinder: sccfinder.cpp
	g++ -lrt -o sccfinder sccfinder.cpp

clean:
	rm -f sccfinder sccfinder.o
