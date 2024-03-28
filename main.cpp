/*********************
Name: James Castle
CS 7350 HW4 Graph Project
Purpose: Implementation for my driver file that will be testing my graph object.
**********************/

#include "main.h"

// Concepts borrowed from
// https://www.programiz.com/dsa/graph-adjacency-list
// https://www.softwaretestinghelp.com/graph-implementation-cpp/

// Constructor
Graph::Graph() {
    nodeCount = 0;
    edgeCount = 0;
    maxDegree = 0;
    verbose = false;
    this->oddDegreeNodes;
}

void Graph::setVerbose(bool beVerbose) {
    this->verbose = beVerbose;
}

void Graph::addNode(int id) {
    Node *newNode = new Node;
    newNode->nodeID = id;
    newNode->nextNode = nullptr;
    newNode->prevNode = nullptr;
    newNode->nextNodeWeight = -1; // Signals there are no connected nodes
    newNode->numDegrees = 0;
    if(this->adjacencyList.empty()){
        this->adjacencyList.push_back(newNode);
    }
    if(this->adjacencyList.size() < (id + 1)) {
        this->adjacencyList.resize(id + 1);
    }
//    adjacencyList.push_back(newNode);
    this->adjacencyList[id] = newNode;
    this->nodeCount++;
}

void Graph::addEdge(int startId, int destId, int edgeWeight = 1) {
    //Check to see if the start and dest nodes are not the same
    if(startId != destId){
        // validate that start and destination nodes exist
        startId = findNode(startId);
        destId = findNode(destId);
        bool preExistingEdge = false;
        int localMaxDegree = 0;  //Prep to check if max degree is greater

        if((startId != -1) && (destId != -1)) {
            // Validate edge does not already exist
            //only check start LL for connection to dest
            Node *headNode = adjacencyList[startId];
            Node *currentNode = adjacencyList[startId];
            Node *temp = adjacencyList[destId];
            Node *newEdge = new Node;
            newEdge->nodeID = temp->nodeID;
            newEdge->nextNode = nullptr;
            newEdge->prevNode = nullptr;
            newEdge->nextNodeWeight = -1;

            while(currentNode->nextNode != nullptr) {
                if(currentNode->nextNode->nodeID == destId){
                    if(this->verbose){
                        std::cout << "Edge Already exists" << startId << " -> " << destId << std::endl;
                    }
                    preExistingEdge = true;
                }
                currentNode = currentNode->nextNode;
                localMaxDegree++;
            }
            if (!preExistingEdge) { // if the edge does not exist, add it.
                currentNode->nextNode = newEdge;
                newEdge->nextNode = nullptr;
                newEdge->prevNode = currentNode;
                currentNode->nextNodeWeight = edgeWeight;
                localMaxDegree++;
                headNode->numDegrees += 1;
                if(headNode->numDegrees % 2 != 0){ // Checks to see if the node has odd degrees
                    //Odd number of degrees, push on oddDegreeNodes list
                    this->oddDegreeNodes.push_back(headNode->nodeID);
                }
                else {
                    // Find and remove ID from oddDegreeNodes list;
                    // Used this source: https://www.shiksha.com/online-courses/articles
                    // /erasing-elements-from-a-vector-in-c/
                    for(int i = 0; i < this->oddDegreeNodes.size(); i++){
                        if(this->oddDegreeNodes[i] == headNode->nodeID){
                            this->oddDegreeNodes.erase(this->oddDegreeNodes.begin() + i);
                            break;
                        }
                    }
                }

                // check for previous directed edge (as undirected edge should be counted once)
                Node *undirCheck = this->adjacencyList[destId];
                bool undirected = false;
                if (undirCheck != nullptr){
                    while(undirCheck->nextNode != nullptr){
                        if(undirCheck->nextNode->nodeID == startId) {
                            undirected = true;
                        }
                        undirCheck = undirCheck->nextNode;
                    }
                }
                if(!undirected) {
                    this->edgeCount++;
                }
            }
        }
        else{
            std::cout << "ERROR: One or more passed in edges are invalid" << std::endl;
        }
        if(localMaxDegree > this->maxDegree) {
            this->maxDegree = localMaxDegree;
        }
    }
}

void Graph::addUndirectedEdge(int startId, int destId, int weight) {
    addEdge(startId, destId, weight);
    addEdge(destId, startId, weight);
}

int Graph::findNode(int targetNodeId) {
    int nodeId = -1; // indicates not found
    //Determine if graph is empty
    if(adjacencyList.empty()){
        if(verbose){
            std::cout << "Graph Empty" << std::endl;
        }
    }
    else{
        for (int i = 0; i < adjacencyList.size(); i++) {
            if(adjacencyList[i] == nullptr){
                continue;
            }
            int currentNodeId = adjacencyList[i]->nodeID;
            if (targetNodeId == currentNodeId) {
                nodeId = currentNodeId;
            }
        }
    }
    if(verbose){
        if(nodeId == -1) {
            std::cout << "node not found" << std::endl;
        }
        else {
            std::cout << "node found" << std::endl;
        }
    }

    return nodeId;
}
void Graph::displayGraph() {
    std::vector<Node*> graphToPrint = this->adjacencyList;
    std::cout << "\nList of nodes in our adjacency list" << std::endl;
    for (int i = 0; i < graphToPrint.size(); i++ ) {
        int degrees = 0;
        bool firstPrint = true;
        // Print node at Root
        std::cout << "Node "<< graphToPrint[i]->nodeID << ": ";
        Node *currentNode = graphToPrint[i];
        while(currentNode->nextNode != nullptr){
            if(firstPrint){
                std::cout << graphToPrint[i]->nodeID;
            }
            currentNode = currentNode->nextNode;
            std::cout << " --> " << currentNode->nodeID << "(" << currentNode->prevNode->nextNodeWeight << ")";
            degrees++;
            firstPrint = false;
        }
        if(degrees == 0) {
            std::cout << "EMPTY";
        }
        std::cout << std::endl;
    }
    std::cout << "# nodes: " << this->nodeCount << "\n# edges: " << this->edgeCount << "\nMax Degrees: " <<
    this->maxDegree << "\nEuler Tour: " << (this->oddDegreeNodes.empty() ? "true":"false") << std::endl;
    std::cout << "Graph adjacency list printed" << std::endl;
}

void Graph::clearGraph() {
    this->adjacencyList.clear();
    this->nodeCount = 0;
    this->edgeCount = 0;
}

void Graph::writeToFile(const std::string& filename) {
    std::vector<Node*> graphToWrite = this->adjacencyList;
    std::ofstream outfile;
//    std::cout << filename;
    outfile.open(filename);

    if(outfile.fail()){ // File could not be opened
        std::cerr << "ERROR: File could not be opened" << std::endl;
        std::exit(1);
    }
    outfile << "Start Node, Destination Node, Edge Weight" << std::endl;

    Node *currentNode;
    for(int i = 0; i < this->adjacencyList.size(); i++){
        // Check to see if node has an edge and write out
        int startId = -1;
        currentNode = this->adjacencyList[i];
        startId = currentNode->nodeID;

        while(currentNode->nextNode != nullptr){
            int destId = -1;
            int weight = -1;
            destId = currentNode->nextNode->nodeID;
            weight = currentNode->nextNodeWeight;
            // If all three are valid (not -1), then write to file
            if ((startId != -1) && (destId != -1) && (weight != -1)){
                outfile << startId << "," << destId << "," << weight << std::endl;
            }
            currentNode = currentNode->nextNode;
        }
    }

    outfile.close();
}

void Graph::readFromFile(const std::string & filename) {
    std::vector<Node*> graphToRead = this->adjacencyList;
    std::ifstream infile;
//    std::cout << filename;
    infile.open(filename);
    if(!infile.is_open()){ // File could not be opened
        std::cerr << "ERROR: File could not be opened" << std::endl;
        std::exit(1);
    }

    // Discard header line
    std::string header;
    std::getline(infile, header);

    // Read the file line by line
    std::string line;
    while(std::getline(infile, line)) {
        // define a stringstreeam to parse each line
        std::istringstream inputString(line);

        // Parse each line into a field
        std::string field;
        int colNum = 0;
        int startEdge = -1;
        int destEdge = -1;
        int weight = -1;

        while(std::getline(inputString, field, ',')) { // parse string into fields
            int item = std::stoi(field);
            if(colNum == 0) {startEdge = item;}
            else if(colNum == 1){destEdge = item;}
            else if(colNum == 2) {weight = item;}
            else{
                std::cout << "ERROR: Illegal column [" << colNum << "] - (not 0, 1, or 2)." << std::endl;
            }
            colNum++;
        }

        //Check if the start and dest node exists
        int startID = findNode(startEdge); // item should equal found id if found, -1 if not found
        int destID = findNode(destEdge); // item should equal found id if found, -1 if not found
        // Add start node and dest nodes if it does not exist
        if(startID == -1){
            addNode(startEdge);
        }
        if(destID == -1){
            addNode(destEdge);
        }

        // Then add the edge
        addEdge(startEdge, destEdge, weight);
    }

    infile.close();
}

void Graph::createCompleteGraph(int numVerts) {
    // Creates a complete undirected graph with passed in number of vertices with edges of weight 1
    // This means each vertex must have a connection (in both direction) to every other node

    //Create adjacency list
    for (int i = 0; i < numVerts; i++) {
        this->addNode(i);
    }

    // Create the undirected edges
    for (int j = 0; j < numVerts; j++) {
        for(int k = 0; k < numVerts; k++){
            int weight = randomRangeGen(100, 1);  // Give a random weight from 1 to 100
            this->addUndirectedEdge(j,k, weight);
        }
    }
}

void Graph::createCycleGraph(int numVerts, bool unDirected) {
    // Creates a graph with V vertices in Cycle format (where each node connects with 2 nodes),
    // Start and end nodes point to each other
    // In my format, I will have each node in the adjacency list point to the node previous and node next, except
    // head and tail nodes who's previous and next will be the the tail node and the head node respectively.
    // Since it does not specify undirected or directed, I'll create the option for both

    // Create adjacency list
    for (int i = 0; i < numVerts; i++) {
        this->addNode(i);
    }
    // Add Edges
    // Check if Undirected
    if(unDirected){
        for (int i = 0; i < numVerts; i++) {
            // if tail (end of list), set next to [0], and previous to [V - 1]
            int weight = randomRangeGen(100, 1);  // Give a random weight from 1 to 100
            if(i == (numVerts - 1)){this->addUndirectedEdge(i, 0, weight);}

                // else, set previous to [V - 1] and next to [V + 1]
            else {
                this->addUndirectedEdge(i, (i + 1), weight);
            }
        }
    }
    else{
        for (int i = 0; i < numVerts; i++) {
            // if tail (end of list), set next to [0], and previous to [V - 1]
            int weight = randomRangeGen(100, 1);  // Give a random weight from 1 to 100
            if(i == (numVerts - 1)) {
                this->addEdge(i, 0, weight);
            }
                // else, set previous to [V - 1] and next to [V + 1]
            else{
                this->addEdge(i, (i + 1), weight);
            }
        }
    }
}

void Graph::createRandomGraph(int numVerts, int numEdges) {
    /* Accepts a number of verts and a number of edges and randomly creates edges between 2 random vertices (that may or
may not be undirected). The graph may not be connected, and can have anywhere from 0 nodes with 0 edges to V vertices
 with (v * (v - 1))/2 edges <- I.e. a complete graph.
 */

    // Create adjacency list
    for (int i = 0; i < numVerts; i++) {
        this->addNode(i);
    }

    // Edge Creation and validation
    int maxEdges = ((numVerts * (numVerts - 1)) / 2);
    if(numEdges > maxEdges){
        std::cout << "Number of requested edges (" << numEdges << " ) exceeds total maximum possible edges ("
                  << maxEdges << "). Replacing edge request total with MaxEdges of: " << maxEdges << std::endl;
        numEdges = maxEdges;
    }

    while (this->edgeCount < numEdges){
        // Use Random to pick a start vert and dest vert between 0 and V - 1 where V - 1 is the last element in the
        // adj list
        int startNodeId = -1;
        int endNodeId = -1;
        do {
            startNodeId = randomRangeGen((numVerts - 1), 0);
            endNodeId = randomRangeGen((numVerts - 1), 0);
        }
        while(startNodeId == endNodeId);  // Ensure nodes are different
        this->addEdge(startNodeId, endNodeId);
    }
}

bool Graph::sortcol(const std::vector<int> &v1, const std::vector<int> &v2) {
    // From https://www.geeksforgeeks.org/sorting-2d-vector-in-c-set-2-in-descending-order-by-row-and-column/
    return v1[2] < v2[2]; // sort by weight (smallest to largest)
}

void Graph::primsMinSpan() {
    // Inspired by https://www.freecodecamp.org/news/prims-algorithm-explained-with-pseudocode/
    std::vector<Node*> graph;
    std::vector<std::vector<int>> mst; // [start_id, dest_id, weight]
    std::vector<std::vector<int>> edgeSet;  // [start_id, dest_id, weight]
    int totalWeight = 0;
    graph = this->adjacencyList;
    int numNodes = 0;
    int lowestWeightIdx; // To track the current lowest weight in the NodesToVisit list
    int minWeight = 999999;
    Node *currentNode;
    Node *originNode;

    std::cout << '\n' << "*****Prim's Minimum Spanning Tree*****" << std::endl;

    // Select the first vertex to start tree
    currentNode = graph[0];
    originNode = graph[0];
    mst.push_back({currentNode->nodeID, currentNode->nodeID, 0}); // Because start node. Not an edge.

    // Initialize set of edges to consider
    while(currentNode->nextNode != nullptr){
        currentNode = currentNode->nextNode;
        edgeSet.push_back({graph[0]->nodeID, currentNode->nodeID, currentNode->prevNode->nextNodeWeight});
    }

    // Iterate until all verts in MST (size = graph size)
    for(int i = 0; i < graph.size() - 1; i++ ){
        // Find min edge in edge set (sort)
        std::sort(edgeSet.begin(), edgeSet.end(), Graph::sortcol);

        // Add vertex to min span tree
        mst.push_back(edgeSet[0]);
        int adjListIndex = this->findNode(edgeSet[0][1]);
        currentNode = adjacencyList[adjListIndex];
        originNode = currentNode;
        edgeSet.erase(edgeSet.begin());

        // Erase any items in edgeset where start and dest are both in the MST
        bool startInEdgeSet = false;
        bool destInEdgeSet = false;
        for (int i = 0; i < edgeSet.size(); i++){
            //Compare edgetSet items 0 (start) and 1 (dest) to see if both in edgeset
            int start = edgeSet[i][0];
            int dest = edgeSet[i][1];
            for (int j = 0; j < mst.size(); j++){
                if(mst[j][1] == start){
                    startInEdgeSet = true;
                }
                if(mst[j][1] == dest){
                    destInEdgeSet = true;
                }

                if(startInEdgeSet && destInEdgeSet){
                    break;
                }
            }
            if(startInEdgeSet && destInEdgeSet){
                edgeSet.erase(edgeSet.begin() + i); // If start and dest nodes in MST, no need to consider edge
                startInEdgeSet = false;
                destInEdgeSet = false;
            }
        }

        // Look at edges connected to this new current vertex
        while(currentNode->nextNode != nullptr){
            currentNode = currentNode->nextNode;
            // If not in MST (i.e. the dest is not the source), add edges connected to new current vertex
            bool inMST = false;
            for (int i = 0; i < mst.size(); i++){
                if (currentNode->nodeID == mst[i][1]){
                    inMST = true;
                    break;
                }
            }
            if(! inMST){
                edgeSet.push_back({originNode->nodeID, currentNode->nodeID, currentNode->prevNode->nextNodeWeight});
            }
        }
    }
    //Print MST (skip 0 because it is the start node captured in start/dest info of index 1)
    std::cout << "Final spanning tree: " << std::endl;
    for (int i = 1; i < mst.size(); i++){
        std::cout << mst[i][0] << " --> " << mst[i][1] << " Weight: "<< mst[i][2] << std::endl;  // print start/dest node and weight
        totalWeight += mst[i][2];  // accumulate weight
    }
    std::cout << "\nTotal Cost: " << totalWeight << std::endl;
    std::cout << "\nPrim's Alogrithm MST completed." << std::endl;
}

int randomRangeGen(int endRange, int startRange = 0) {
    // General implementation borrowed from:
    // https://www.digitalocean.com/community/tutorials/random-number-generator-c-plus-plus

    // Retrieve a random number between startRange and EndRange
    int random = startRange + (rand() % ((endRange - startRange) + 1));

    return random;
}

int main() {
    std::string filename = R"(C:\Users\Wolf\Dropbox\Grad School\Spring 2024\CS7350\Module 4\HW4Code\exampleImport.csv)";
    std::srand(time(nullptr));
    /* *****************************TESTING SECTION ********************************************************************
// First round testing
//    Graph graph;
//    graph.readFromFile(filename);
//    graph.displayGraph();
//    graph.writeToFile("test.csv");

    // Test out the complete undirected graph
//    Graph undirectedCompleteGraph;
//    undirectedCompleteGraph.q1Graph(9);
//    undirectedCompleteGraph.displayGraph();
//    undirectedCompleteGraph.writeToFile("undirectedGraph.csv");

//    Graph readUndirected;
//    filename = R"(C:\Users\Wolf\Dropbox\Grad School\Spring 2024\CS7350\Module 4\HW4Code\cmake-build-debug\undirectedGraph.csv)";
//    readUndirected.readFromFile(filename);
//    readUndirected.displayGraph();
//    readUndirected.primsMinSpan();

//    // Test MST
//    filename = R"(C:\Users\Wolf\Dropbox\Grad School\Spring 2024\CS7350\Module 4\HW4Code\weightedImport.csv)";
//    Graph mst;
//    mst.readFromFile(filename);
//    mst.displayGraph();
//    mst.primsMinSpan();
//
//    // Test second MST
//    filename = R"(C:\Users\Wolf\Dropbox\Grad School\Spring 2024\CS7350\Module 4\HW4Code\exgraph2.csv)";
//    Graph mst2;
//    mst2.readFromFile(filename);
//    mst2.displayGraph();
//    mst2.primsMinSpan();

//    // Figure out why PRIM is not throwing away edge for visited node
//    filename = R"(C:\Users\Wolf\Dropbox\Grad School\Spring 2024\CS7350\Module 4\HW4Code\cmake-build-debug\brokecomplete1.csv)";
//    Graph brokeComplete;
//    brokeComplete.readFromFile(filename);
//    brokeComplete.displayGraph();
//    brokeComplete.primsMinSpan();

    // Test creating an undirected cycle Graph V = N and E = V
    std::cout << "**************************Cycle Graph Tests**************************" << std::endl;
    Graph cycleRandomGraph;
    cycleRandomGraph.createCycleGraph(6, true);
    cycleRandomGraph.displayGraph();
    cycleRandomGraph.primsMinSpan();
    i = 1;
    filename = "cycle" + std::to_string(i) + ".csv";
    cycleRandomGraph.writeToFile(filename);


    // Write out complete graph with random weights of size V = 6 E = |V *(V - 1) /2 |
    std::cout << "**************************Complete Graph Tests**************************" << std::endl;
    Graph completeRandomGraph;
    int v = 50;
    completeRandomGraph.createCompleteGraph(v);
    int i = 1;
//    filename = "complete" + std::to_string(v) + ".csv";
//    completeRandomGraph.writeToFile(filename);
    completeRandomGraph.displayGraph();

    //Time  PRIM per Q2
    auto start = std::chrono::high_resolution_clock::now();  // start timer
    completeRandomGraph.primsMinSpan();
    auto stop = std::chrono::high_resolution_clock::now(); // stop timer
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "For V: " << v << "\n" <<"Time taken by problem: " << duration.count()
              << " microseconds\n" << std::endl;

************************************************ END TESTING SECTION *************************************************/

    // Writing Test Files Section
    int numFiles = 10;
    int numNodes = 10;
    int numEdges = 20;
    for (int i = 0; i < numFiles; i++){
        Graph completeRandomGraph;
        Graph cycleRandomGraph;
        Graph randomRandomGraph;

        // Make the graphs
        completeRandomGraph.createCompleteGraph(numNodes);
        cycleRandomGraph.createCycleGraph(numNodes,true);
        randomRandomGraph.createRandomGraph(numNodes, numEdges);

        // Write the files
        std::string completeFileName = filename = "complete" + std::to_string(i) + ".csv";
        std::string cycleFileName = filename = "cycle" + std::to_string(i) + ".csv";
        std::string randomFileName = filename = "random" + std::to_string(i) + ".csv";
        completeRandomGraph.writeToFile(completeFileName);
        cycleRandomGraph.writeToFile(cycleFileName);
        randomRandomGraph.writeToFile(randomFileName);

        // Increment sizes and do it all again
        numNodes += 10;
        numEdges += 10;
    }
    
    // End of Writing Test Files

//    // Test creating an undirected cycle Graph V = N and E = V
//    std::cout << "**************************Cycle Graph Tests**************************" << std::endl;
//    Graph cycleRandomGraph;
//    cycleRandomGraph.createCycleGraph(6, true);
//    cycleRandomGraph.displayGraph();
//    cycleRandomGraph.primsMinSpan();
//    i = 1;
//    filename = "cycle" + std::to_string(i) + ".csv";
//    cycleRandomGraph.writeToFile(filename);
//
//    std::cout << "**************************Complete Graph Tests**************************" << std::endl;
//    Graph completeRandomGraph;
//    int v = 50;
//    completeRandomGraph.createCompleteGraph(v);
//    int i = 1;
////    filename = "complete" + std::to_string(v) + ".csv";
////    completeRandomGraph.writeToFile(filename);
//    completeRandomGraph.displayGraph();
//    //Time  PRIM per Q2
//    auto start = std::chrono::high_resolution_clock::now();  // start timer
//    completeRandomGraph.primsMinSpan();
//    auto stop = std::chrono::high_resolution_clock::now(); // stop timer
//    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
//    std::cout << "For V: " << v << "\n" <<"Time taken by problem: " << duration.count()
//              << " microseconds\n" << std::endl;

    // Edge case to solve for: Reading in/writing out a graph with disconnected nodes (nodes with degree 0)
    std::cout << "\ndone" << std::endl;
    return 0;
}