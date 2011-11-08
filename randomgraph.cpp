#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
  if (argc != 4) {
    cout << "Wrong number of arguments" << endl;
    return 1;
  }

  int n = atoi(argv[1]);
  int m = atoi(argv[2]);
  char* outputFile = argv[3];

  vector< vector<bool> > graph;
  for (int i = 0; i < n; i++) {
    vector<bool> v;
    for (int j = 0; j < n; j++) {
      v.push_back(false);
    }
    graph.push_back(v);
  }

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
  return 0;
}
