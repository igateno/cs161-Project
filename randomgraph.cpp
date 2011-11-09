#include <fstream>
#include <iostream>
#include <vector>
#include <new>

using namespace std;

bool** allocateGraph(int n)
{
  bool** graph = new (nothrow) bool*[n];
  if (graph == NULL) {
    cout << "Memory allocation error" << endl;
    exit(1);
  }
  for (int i = 1; i <= n; i++) {
    graph[i] = new (nothrow) bool[n];
    if (graph[i] == NULL) {
      cout << "Memory allocation error" << endl;
      exit(1);
    }
  }
  return graph;
}

void deallocateGraph(bool** graph, int n)
{
  for (int i = 1; i <= n; i++) {
    delete(graph[i]);
  }
  delete(graph);
}

/*
 * This function reads in /dev/random to seed the
 * random number generator with a better entropy source
 * than time(NULL)
 */
int seed()
{
	int n = 0;
	ifstream s;
	s.open("/dev/urandom", ios::binary);
	if (s.is_open()) {
		s.read((char *)(&n), sizeof(n));
		s.close();
		return n;
	} else {
		cout << "error opening /dev/random" << endl;
	}
}

void outputRandomGraph(int n, int m, char* outputFile)
{
  bool** graph = allocateGraph(n);

  std::ofstream os;
  os.open(outputFile);
  os << n << endl;
  os << m << endl;

  srand(seed());
  int count = 0;
  while(count < m) {
    int i = rand() % n+1;
    int j = rand() % n+1;
    if (!graph[i][j]) {
      os << i << " " << j << endl;
      graph[i][j] = true;
      count++;
    }
  }

  os.close();

  deallocateGraph(graph, n);
}

int main(int argc, char* argv[])
{
  if (argc != 4) {
    cout << "Wrong number of arguments" << endl;
    return 1;
  }

  int n = atoi(argv[1]);
  int m = atoi(argv[2]);
  char* outputFile = argv[3];

	if (n > RAND_MAX) {
		cout << "I can't generate numbers that large" << endl;
		return 1;
	}

  outputRandomGraph(n, m, outputFile);

  return 0;
}
