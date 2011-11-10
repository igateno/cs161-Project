#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
#include "Node.h"
#include <stack>
#include <map>
#include <cstdlib>

using namespace std;

int t = 0;
int s = -1;

struct funcCall{
    Node* node;
    int startEdge;
};

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

int seed(){
	int n = 0;
	ifstream s;
	s.open("/dev/urandom", ios::binary);
	if (s.is_open()) {
		s.read((char *)(&n), sizeof(n));
		s.close();
		return n;
	} else {
		cout << "error opening /dev/random" << endl;
	}
}


/*
 * Reads input file and adds each node and it's edges into the graph vector.
 */
void MakeGraph(int n, int m, vector<Node>* graph, vector<Node>* revGraph){
   AddNodes(graph,n);//add nodes to graph
   AddNodes(revGraph, n);
   srand(seed());
   for(int i = 0; i < m; i++){
       int i = rand() % n;
       int j = rand() % n;
       graph->at(i).edges.push_back(&graph->at(j));//add edge to node
       revGraph->at(j).edges.push_back(&revGraph->at(i));
    }
}

void initOutArray(int out[5]){
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    out[4] = 0;
}

void DFS(Node* currNode, vector<Node*>* secondGraph, vector<Node>* graph){
    stack<funcCall> callStack;
    funcCall firstCall;
    firstCall.node = currNode;
    firstCall.startEdge = 0;
    callStack.push(firstCall);

    do{
    nextLoop:
    funcCall* currCall = &(callStack.top());
    currNode = currCall->node;
    currNode->inStack = true;
    for(int i = currCall->startEdge; i < currNode->edges.size(); i++){
    if(!currNode->edges.at(i)->inStack){
            currCall->startEdge = i+1;
            funcCall newCall;
            newCall.node = currNode->edges.at(i);
            newCall.startEdge = 0;
            callStack.push(newCall);
            goto nextLoop;
        }
    }
    secondGraph->push_back(&graph->at((currNode->startingIndex)-1));
    callStack.pop();
    }while(!callStack.empty());
}

void FirstPass(vector<Node>* revGraph, vector<Node>* graph, vector<Node*>* secondGraph){
    for(int i = 0; i < revGraph->size(); i++){
        Node* currNode = &revGraph->at(i);
        if(!currNode->inStack){
            s = i;
            DFS(currNode, secondGraph, graph);
        }
    }
}

void DFS2(Node* currNode){
    stack<funcCall> callStack;
    funcCall firstCall;
    firstCall.node = currNode;
    firstCall.startEdge = 0;
    callStack.push(firstCall);

    do{
    nextLoop:
    funcCall* currCall = &(callStack.top());
    currNode = currCall->node;
    currNode->inStack = true;
    currNode->leader = s;
    for(int i = 0; i < currNode->edges.size(); i++){
         if(!currNode->edges.at(i)->inStack){
            currCall->startEdge = i+1;
            funcCall newCall;
            newCall.node = currNode->edges.at(i);
            newCall.startEdge = 0;
            callStack.push(newCall);
            goto nextLoop;
         }
    }
    callStack.pop();
    }while(!callStack.empty());
}

void SecondPass(vector<Node*>* graph){
    t = 0;
    s = -1;
    for(int i = graph->size()-1; i >=0; i--){
        Node* currNode = graph->at(i);
        if(!currNode->inStack){
            s = i;
            DFS2(currNode);
        }
    }
}

void CountSCCs(vector<Node*>* graph, int out[5]){
    map<int,int> sccs;
    int size;
    for(int i = 0; i < graph->size(); i++){
        sccs[graph->at(i)->leader] = sccs[graph->at(i)->leader]++;
    }
    map<int,int>::iterator it;
    for(it = sccs.begin(); it != sccs.end(); it++){
        size = (*it).second;
        if(size > out[4]){
            AddSCC(out,(*it).second);
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
void findSccs(int n, int m, int out[5])
{
    initOutArray(out);
    vector<Node> graph = vector<Node>();
    vector<Node> revGraph = vector<Node>();
    MakeGraph(n, m, &graph, &revGraph);
    vector<Node*> secondGraph;
    secondGraph.reserve(revGraph.size());
    FirstPass(&revGraph, &graph, &secondGraph);
    SecondPass(&secondGraph);
    CountSCCs(&secondGraph, out);
    /*
    SCCStack* start = NULL;
    SCCStack** stack = &start;
    int count = 1;
    int sccCount = 0;
    for(int i = 0; i < graph.size(); i++) {
        if(graph.at(i).index == 0){
            SCCHelper(&(graph.at(i)), out, &count, stack, &sccCount);
        }
    }*/
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
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    char* outputFile = argv[3];

    findSccs(n, m, sccSizes);
    //cout <<"should be done" << endl;
    // Output the first 5 sccs into a file.
    std::ofstream os;
    os.open(outputFile);
    os << sccSizes[0] << "\t" << sccSizes[1] << "\t" << sccSizes[2] <<
      "\t" << sccSizes[3] << "\t" << sccSizes[4];
    os.close();
    final = clock() - start;
    final = (final * 1000)/CLOCKS_PER_SEC;
    //cout<< "Time: " << final << "ms" << endl;
    return 0;
}

