#include "Path.h"

using namespace std;

Path::Path(int n)
{
  nodes = vector<int>();
  nodes.push_back(n);
  hops = 1;
}

void Path::addNode(int n)
{
  nodes.push_back(n);
  hops++;
}

bool Path::operator> (Path &p)
{
  return hops > p.hops;
}

bool Path::operator< (Path &p)
{
  return hops < p.hops;
}
