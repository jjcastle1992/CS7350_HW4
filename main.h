/*********************
Name: James Castle
CS 7350 HW4 Graph Project
Purpose: Interface for my driver file that will be testing my graph object.
**********************/

#ifndef HW4_CODE_MAIN_H
#define HW4_CODE_MAIN_H

#include <iostream>
#include <string>
#include <sstream> // for file parsing
#include <bits/stdc++.h>
#include <chrono>
#include <cstdlib> // For function calls in randomRangeGen
#include <fstream> // For file writing/reading

struct Node {
    int nodeID;
    Node *nextNode;
    Node *prevNode; // to get the node previous weight
    int nextNodeWeight;
    int numDegrees; // to detect Euler tours at head of each list element
};

class Graph {
public:
    //Constructor
    Graph();

    //Destructor - not going to worry about for now

    //Getters <- breaking encapsulation by not using for attributes, but not trying to be robust.
//    int numNodes();
//    int numEdges();

    // Setters  <- breaking encapsulation by not using for attributes, but not trying to be robust
    void setVerbose(bool); // Set debugging prints on or off

    //Vertex & Edge Add/Removal
    void addNode(int); // Takes in a Node ID
    void addEdge(int, int, int); //Takes in starting nodeID, destination Node ID, and weight,
    void addUndirectedEdge(int, int, int); // Calls addEdge twice - one for each direction

    // Printing and utilities
    void displayGraph(); // Displays the entire graph in adjacency list format.
    void clearGraph(); // Deletes Adjacency Matrix
    void writeToFile(const std::string&); // writes adjacency list data to file per spec
    void readFromFile(const std::string&); // reads adjacency list data to file per spec
    static bool sortcol(const std::vector<int>&, const std::vector<int>&);

    // MST methods
    void primsMinSpan(); // Prims MST algorithm that provides a minimum spanning tree.

    // Graph Creation Methods
    void createCompleteGraph(int); // creates a complete undirected graph w/ V verts with random edge weights between (1 - 100).
    void createCycleGraph(int, bool); // Create a cycle graph w/ v Verts and either directed/undirected with random edge weights (1 - 100)


private:
    int findNode (int); // looks in our adjacency list to find a node. Accepts an int node ID, and a bool for verbose
    std::vector<Node*> adjacencyList; // Our graph in adjacency list format
    int nodeCount; // counts number of nodes in graph (should be == to size of vector)
    int edgeCount; // Counts each unique edge between nodes (undirected edges get counted once)
    int maxDegree; // Counts the maximum degree of any vertex in the graph
    std::vector<int> oddDegreeNodes; // List containing all nodes of odd degree (Determines Euler Tour Truth)
    bool verbose; // turns on/off debug prints
};

int randomRangeGen(int, int);

#endif //HW4_CODE_MAIN_H
