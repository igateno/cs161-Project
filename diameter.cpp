#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <queue>
#include <set>

#define INFINITY -1

using namespace std;

class Path {

  public:

    vector<int> nodes;
    int hops;

    Path(int n)
    {
      nodes = vector<int>();
      nodes.push_back(n);
      hops = 1;
    }

    void addNode(int n)
    {
      nodes.push_back(n);
      hops++;
    }

};

bool operator> (const Path &p1, const Path &p2)
{
  return p1.hops > p2.hops;
}

bool operator< (const Path &p1, const Path &p2)
{
  return p2.hops < p2.hops;
}

int populateMap(char* filename, map<int, vector<int> >* graph)
{
  int n, m, u, v = 0;

  ifstream f;
  f.open(filename);
  if (f.is_open()) {
    f >> n;
    f >> m;
    while (f >> u) {
      f >> v;
      (*graph)[u].push_back(v);
    }
  } else {
    cout << "Failed to open file" << endl;
    exit(1);
  }

  return n;
}

int smallestDistance(map<int, int>* dist) {
  map<int, int>::iterator iter;
  int distance = INFINITY;
  int key = 0;
  for (iter = (*dist).begin(); iter != (*dist).end(); iter++) {
    if ((*iter).second != INFINITY) {
      if (distance == INFINITY || distance > (*iter).second) {
        distance = (*iter).second;
        key = (*iter).first;
      }
    }
  }
  return key;
}

int longestShortestPath(int start, int n, map<int, vector<int> >* graph)
{
  priority_queue<Path, vector<Path>, less<vector<Path>::value_type> > pQueue;
  set<int> Q;
  for (int i = 1; i <= n; i++) {
    if (i != start) {
      Q.insert(i);
    }
  }
  pQueue.push(Path(start));
  while (Q.size() != 0) {
    break;
  }
  return 0;
}

int main(int argc, char* argv[])
{
  if (argc != 2) {
    cout << "This program takes 1 argument" << endl;
    return 1;
  }

  char* filename = argv[1];
  map<int, vector<int> > graph;
  int n = populateMap(filename, &graph);

  int diameter = 0;
  for (int i = 1; i <= n; i++) {
    int temp = longestShortestPath(i, n, &graph);
    if (temp > diameter) {
      diameter = temp;
    }
  }

  cout << "Diameter: " << diameter << endl;

  return 0;
}
