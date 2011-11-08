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
  for (int i = 0; i < n; i++) {
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
  for (int i = 0; i < n; i++) {
    delete(graph[i]);
  }
  delete(graph);
}

void outputRandomGraph(int n, int m, char* outputFile)
{
  bool** graph = allocateGraph(n);

  std::ofstream os;
  os.open(outputFile);
  os << n << endl;
  os << m << endl;

  srand(time(NULL));
  int count = 0;
  while(count < m) {
    int i = rand() % n;
    int j = rand() % n;
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
  outputRandomGraph(n, m, outputFile);

  return 0;
}
