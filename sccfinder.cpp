#include <fstream>
#include <iostream>
#include <vector>
#include "Node.h"

using namespace std;

struct SCCStack{
    int node;
    int index;
    SCCStack* next;
};

void PrintStack(SCCStack** stack){
    SCCStack* curr = *stack;
    cout << "\nprinting stack" << endl;
    while(curr != NULL){
         cout << "node:" << curr << " num:" << curr->node << "index:" << curr->index << endl;
         curr = curr->next;
    }
}

bool NodeInStack(SCCStack** stack, Node* node){
    //cout<< "Checking Node in Stack." << endl;
    //cout<<"Finding " << node->startingIndex << endl;
    //PrintStack(stack);
    SCCStack* prev = *stack;
    SCCStack* curr = *stack;
    while(curr != NULL){
        if(node->startingIndex == curr->node){
            //cout << "found it!" << endl;
            return true;
        }
        prev = curr;
        curr = curr->next;    
    }
        //cout << "not found" << endl;
        return false;
}

int CountStackSize(SCCStack** stack, Node* endingNode){
    //PrintStack(stack);
    bool loop = false;
    int count = 0;
    SCCStack* curr = *stack;
    SCCStack* deletePtr = *stack;
    //cout << "starting " << endingNode->startingIndex << endl;
    //cout << endl;
    while(curr != NULL && !loop){
        if(endingNode->startingIndex == curr->node){
            loop = true;
        }
        count++;
        deletePtr = curr;
        curr = curr->next;
        delete(deletePtr);
    }
    *stack = curr;
    //PrintStack(stack);
    return count;
}

void AddSCC(int out[5], int size){
    int index = -1;
    for(int i = 3; i >= 0; i--){
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

void PrintGraph(vector<Node>* graph){
    for(int i = 0; i < graph->size(); i++){
        cout << graph->at(i).startingIndex;
        for(int j = 0; j < graph->at(i).edges.size(); j++){
            cout << graph->at(i).edges.at(j)->startingIndex;
        }
        cout << endl;
    }
}

void AddNodes(vector<Node>* graph, int numNodes){
    for(int i = 1; i <= numNodes; i++){
        Node newNode = Node();
        newNode.startingIndex = i;
        graph->push_back(newNode);
    }
}

void ReadGraph(char* inputFile, vector<Node>* graph){
    int number;
    int edgeStart;
    int edgeEnd;
    ifstream reader;
    reader.open(inputFile);
    if(reader.is_open()) {
        reader >> number;
        graph->reserve(number);
        AddNodes(graph,number);
        reader >> number;
        int numEdges = number;
        while(reader >> edgeStart) {
            reader >> edgeEnd;
            edgeStart--;
            edgeEnd--;
            graph->at(edgeStart).edges.push_back(&graph->at(edgeEnd));
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

void SCCHelper(Node* v, int out[5], int* count, SCCStack** stack, int* sccCount){
    v->index = *count;
    v->link = *count;
    (*count)++;
    SCCStack* newElem = new SCCStack();
    newElem->node = v->startingIndex;
    newElem->index = v->index;
    SCCStack* oldTop = *stack;
    *stack = newElem;
    newElem->next = oldTop; 
    
    for(int i = 0; i < v->edges.size(); i++){
        Node* vPrime = v->edges.at(i);
        if(vPrime->index == 0){
            SCCHelper(vPrime, out, count, stack, sccCount);
            if(v->link > vPrime->link){
                v->link = vPrime->link;
            }
        }else if(NodeInStack(stack, vPrime)) {
            if(v->link > vPrime->index){
                v->link = vPrime->index;
            }
        }
    }
   // cout << "Checking: link:" << v->link << " index:" << v->index << " startingIndex:" << v->startingIndex << endl;
   // PrintStack(stack);
    if(v->link == v->index){
        int sccSize = CountStackSize(stack, v);
        if(sccSize > out[5]){
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
    PrintGraph(&graph);
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
    return 0;
}

