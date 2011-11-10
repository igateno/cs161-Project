#include <fstream>
#include <iostream>
#include <vector>
#include <new>

using namespace std;

struct node {
	int u;
	int v;
};
typedef struct node node_t;

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

bool nodeInGraph(int u, int v, vector<node_t> graph)
{
	for (int i = 0; i < graph.size(); i++) {
		node_t node = graph[i];
		if (node.u == u && node.v == v)
			return true;
	}
	return false;
}

void outputRandomGraph(int n, int m, char* outputFile)
{
  vector<node_t> graph;

  std::ofstream os;
  os.open(outputFile);
  os << n << endl;
  os << m << endl;

  srand(seed());
  int count = 0;
  while(count < m) {
    int i = rand() % n+1;
    int j = rand() % n+1;
    if (!nodeInGraph(i, j, graph)) {
      os << i << " " << j << endl;
			node_t node;
			node.u = i;
			node.v = j;
      graph.push_back(node);
      count++;
    }
  }

  os.close();

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
