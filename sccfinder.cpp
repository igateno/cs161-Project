#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
#include "Node.h"

using namespace std;

/*
 * Struct to be used as a stack nodes.
 * Linked list allows use of popping and pushing (inserting/removing at front of list)
 * as well as traversal through the stack. 
 */
struct SCCStack{
    Node* node;
    SCCStack* next;
};

/*
 * Debugging function that prints out stack linked list.
 */
void PrintStack(SCCStack** stack){
    SCCStack* curr = *stack;
    cout << "\nprinting stack" << endl;
    while(curr != NULL){
         cout << "node:" << curr << " num:" << curr->node << endl;
         curr = curr->next;
    }
}

/*
 * Count the number of nodes in the current SCC.
 * endingNode is the leader of the current SCC, we pop every node currently in
 * the stack before and including the leader. These nodes make up the current SCC.
 */
int CountSCCSize(SCCStack** stack, Node* endingNode){
    bool stop = false; //if we hit the leader
    int count = 0;
    SCCStack* curr = *stack;
    SCCStack* deletePtr = *stack;
    while(curr != NULL && !stop){
        if(endingNode->startingIndex == curr->node->startingIndex){
            stop = true;
        }
        count++;
        deletePtr = curr;//delete any nodes we pop from the stack.
        curr = curr->next;
        deletePtr->node->inStack = false;
        delete(deletePtr);
    }
    *stack = curr; //stack should not point to leftover nodes.
    return count;
}

/*
 * Adds an SCC size to the list of top 5 largest SCCs.
 */
void AddSCC(int out[5], int size){
    int index = -1;
    //first we find the index where the count belongs
    for(int i = 4; i >= 0; i--){
        if(size > out[i]){
            index = i;
        }else{
            break;
        }
    }
    
    for(int j = 4; j > index; j--){
        out[j] = out[j-1];
    }
    out[index] = size;
}

/*
 * Creates a vector of numNodes nodes. This is our graph.
 */
void AddNodes(vector<Node>* graph, int numNodes){
    for(int i = 1; i <= numNodes; i++){
        Node newNode = Node();
        newNode.startingIndex = i;
        graph->push_back(newNode);
    }
}

/*
 * Debugging function that prints out each node number followed by each 
 * node it has a directed edge towards.
 */
void PrintGraph(vector<Node>* graph){
    for(int i = 0; i < graph->size(); i++){
        cout << graph->at(i).startingIndex;
        for(int j = 0; j < graph->at(i).edges.size(); j++){
            cout << " " << graph->at(i).edges.at(j)->startingIndex;
        }
        cout << endl;
    }
}

/*
 * Reads input file and adds each node and it's edges into the graph vector.
 */
void ReadGraph(char* inputFile, vector<Node>* graph){
    int number; // stores the first 2 numbers in input file (number of node and edges)
    int edgeStart;
    int edgeEnd;
    ifstream reader;
    reader.open(inputFile);
    if(reader.is_open()) {
        reader >> number;
        graph->reserve(number);
        AddNodes(graph,number);//add nodes to graph
        reader >> number;
        int numEdges = number;
        //add edges to graph
        while(reader >> edgeStart) {
            reader >> edgeEnd;
            edgeStart--;//vector starts at 0 and nodes start at 1.
            edgeEnd--;
            graph->at(edgeStart).edges.push_back(&graph->at(edgeEnd));//add edge to node
        }
    }else {
         cout << "error: could not open file" << inputFile << endl;
    }
}

void initOutArray(int out[5]){
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    out[4] = 0;
}

/* 
 * Recursive function that searches graph for SCCS.
 */
void SCCHelper(Node* v, int out[5], int* count, SCCStack** stack, int* sccCount){
    v->index = *count; //current count of nodes traversed
    v->leader = *count;
    (*count)++;
    //add current node to the stack.
    SCCStack* newElem = new SCCStack();
    newElem->node = v;
    v->inStack = true;//tell the node that it has been added to the stack.
    SCCStack* oldTop = *stack;
    *stack = newElem;
    newElem->next = oldTop; 
    
    
    //Depth first search through the nodes connected to our current nore.
    //If a node has not yet been visited we recurse on it.
    for(int i = 0; i < v->edges.size(); i++){
        Node* vPrime = v->edges.at(i);
        if(vPrime->index == 0){//has not been visited.
            SCCHelper(vPrime, out, count, stack, sccCount);
            if(v->leader > vPrime->leader){//determine which node is closer to the leader.
                v->leader = vPrime->leader;
            }
        }else if(vPrime->inStack) {//node is part of current SCC.
            if(v->leader > vPrime->index){//check to see which is closer to leader.
                v->leader = vPrime->index;
            }
        }
    }
    
    //When we find an SCC leader we want to count all the nodes in this SCC.
    if(v->leader == v->index){//if the current node is a leader of its SCC.
        int sccSize = CountSCCSize(stack, v);
        if(sccSize > out[4]){//record this SCC size if it is among largest 5.
            AddSCC(out, sccSize);
        }
    }
        
}

/**
 * Given an input file (inputFile) and an integer array (out) of size 5, fills
 * the 5 largest SCC sizes into (out) in decreasing order. In the case where
 * there are fewer than 5 components, you should fill in 0 for the remaining
 * values in (out). For example, if there are only 2 components of size 100 and
 * 50, you should fill:
 * out[0] = 100
 * out[1] = 50
 * out[2] = 0
 * out[3] = 0
 * out[4] = 0
 */
void findSccs(char* inputFile, int out[5])
{
    vector<Node> graph= vector<Node>();
    ReadGraph(inputFile, &graph);
    initOutArray(out);
    SCCStack* start = NULL;
    SCCStack** stack = &start;
    int count = 1;
    int sccCount = 0;
    for(int i = 0; i < graph.size(); i++) {
        if(graph.at(i).index == 0){
            SCCHelper(&(graph.at(i)), out, &count, stack, &sccCount);
        }
    }
}

/*
 * sccfinder should be your main class. If you decide to code in C, you can
 * rename this file to sccfinder.c. We only want your binary to be named
 * sccfinder and we want "make sccfinder" to build sccfinder.
 * Main takes two arguments: 1) input file and 2) output file.
 * You should fill in the findSccs function.
 * Warning: Don't change the part of main that outputs the result of findSccs.
 * It outputs in the correct format.
 */
int main(int argc, char* argv[])
{
    clock_t start, final = 0;
    start = clock();
    int sccSizes[5];
    char* inputFile = argv[1];
    char* outputFile = argv[2];

    findSccs(inputFile, sccSizes);

    // Output the first 5 sccs into a file.
    std::ofstream os;
    os.open(outputFile);
    os << sccSizes[0] << "\t" << sccSizes[1] << "\t" << sccSizes[2] <<
      "\t" << sccSizes[3] << "\t" << sccSizes[4];
    os.close();
    final = clock() - start;
    final = (final * 1000)/CLOCKS_PER_SEC;
    cout<< "Time: " << final << "ms" << endl;
    return 0;
}

