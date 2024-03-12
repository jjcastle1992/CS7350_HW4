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

    // Question-based methods (For all edges in Q1 - Q3, weights are 1)
    void q1Graph(int); // creates graph per q1 spec (accepts # verts and makes an undirected complete graph).
    void q2Graph(int, bool); // Create a graph per q2 spec (Accept # Verts, and undirected bool and makes a Cycle graph)
    void q3Graph(int, int); // Graphs a graph with V vertices and E edges between random pairs of vertices (using edge
    // count as basis of blend of directed/undirected edges).

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
