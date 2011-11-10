#ifndef _PATH_H
#define _PATH_H

#include <vector>

using namespace std;

class Path {

  private:

    vector<int> nodes;
    int hops;

  public:

    Path(int n);
    void addNode(int n);
    bool operator> (Path &p);
    bool operator< (Path &p);

};

#endif
