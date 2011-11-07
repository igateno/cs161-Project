/*
 *
 * Node Class will represent each vertex
 *
 */

#ifndef _NODE_H
#define _NODE_H

#include <vector>

using namespace std;

class Node {

public:
    vector <Node*> edges;
    unsigned int startingIndex;
    unsigned int index;
    unsigned int link;

    Node();
private:

};

#endif 
