#include "Node.h"

using namespace std;

Node::Node() {
    edges = vector<Node*>(); 
    startingIndex = 0;   
    index = 0;
    leader = 0;
    inStack = false;
}
