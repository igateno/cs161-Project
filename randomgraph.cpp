#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
  if (argc != 3) {
    cout << "Wrong number of arguments" << endl;
    return 1;
  }

  int numNodes = atoi(argv[1]);
  int numEdges = atoi(argv[2]);
  char* outputFile = argv[3];

  std::ofstream os;
  os.open(outputFile);
  os << "Nodes: " << numNodes << endl;
  os << "Edges: " << numEdges << endl;
  os.close();
  return 0;
}
