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
    newNode->weightToOrigin = 0; // Signals there are no connected nodes
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
            newEdge->weightToOrigin = 0;

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
                currentNode->weightToOrigin = edgeWeight;
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
            std::cout << " -> " << currentNode->nodeID;
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
            int weight = 0;
            destId = currentNode->nextNode->nodeID;
            weight = currentNode->weightToOrigin;
            // If all three are valid (not -1), then write to file
            if ((startId != -1) && (destId != -1) && (weight != 0)){
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

void Graph::q1Graph(int numVerts) {
    // Creates a complete undirected graph with passed in number of vertices with edges of weight 1
    // This means each vertex must have a connection (in both direction) to every other node

    //Create adjacency list
    for (int i = 0; i < numVerts; i++) {
        this->addNode(i);
    }

    // Create the undirected edges
    for (int j = 0; j < numVerts; j++) {
        for(int k = 0; k < numVerts; k++){
            this->addUndirectedEdge(j,k, 1);
        }
    }
}

void Graph::q2Graph(int numVerts, bool unDirected) {
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
            if(i == (numVerts - 1)){this->addUndirectedEdge(i, 0, 1);}

                // else, set previous to [V - 1] and next to [V + 1]
            else {
                this->addUndirectedEdge(i, (i + 1), 1);
            }
        }
    }
    else{
        for (int i = 0; i < numVerts; i++) {
            // if tail (end of list), set next to [0], and previous to [V - 1]
            if(i == (numVerts - 1)) {
                this->addEdge(i, 0, 1);
            }
                // else, set previous to [V - 1] and next to [V + 1]
            else{
                this->addEdge(i, (i + 1), 1);
            }
        }
    }
}

void Graph::kruskalsMinSpan() {
    // Inspired by https://www.geeksforgeeks.org/kruskals-minimum-spanning-tree-algorithm-greedy-algo-2.
    // https://www.geeksforgeeks.org/detect-cycle-undirected-graph/?ref=lbp
    // https://www.simplilearn.com/tutorials/data-structure-tutorial/kruskal-algorithm
    std::vector<int> xyweightList; // Element 0 is x (start), 1 is y (dest), 2 is weight

    // Create a list of all start vertices, dest vertices, and weights from the adjacency list.
    std::vector<std::vector<int>> mst;

    for (int i = 0; i < this->adjacencyList.size(); i++) {
        int startNode = this->adjacencyList[i]->nodeID;
    }
    //Find the last item in the vector with a weight <= to the current item weight

    // Add current item to the next available spot in the vector after said last item

    // Check start-dest from lowest to highest weight and add to MST if it does not create a cycle
    // Chedck for cycle by seeing if you can traverse from the dest node to the start node

    // invoke MST display method

}

void Graph::primsMinSpan() {
    // Inspired by https://www.freecodecamp.org/news/prims-algorithm-explained-with-pseudocode/
    std::vector<Node*> mst;
    std::vector<Node*> nodesToVisit;
    int numNodes = 0;
    int lowestWeightIdx; // To track the current lowest weight in the NodesToVisit list
    int minWeight = 100000;
    Node *currentNode;

    std::cout << '\n' << "*****Prim's Minimum Spanning Tree*****" << std::endl;
    while( numNodes < this->adjacencyList.size()){
        // On first run, Arbitrarily choose first node in the adjacency list to start
        if(mst.empty()){
            currentNode = this->adjacencyList[0];
        }
        // else choose the lowest weight item in the nodesToVisit list
        else {
            currentNode = nodesToVisit[lowestWeightIdx];
            std::cout << "-( " << minWeight << " )-> ";
        }

        // Add the node to the MST
        mst.push_back(currentNode);
        numNodes++;
        std::cout << currentNode->nodeID;

        // Add neighbors to the nodesToVisit list (if we haven't already explored all nodes)
        while((currentNode->nextNode != nullptr) && (numNodes < this->adjacencyList.size())) {
            int currentWeight = -9999;
            currentNode = currentNode->nextNode;
            // Check if the next node is already in the MST.
            std::vector<Node*>::iterator it;
            it = std::find(mst.begin(), mst.end(), currentNode);
            // If not in list, add to the list of nodes to explore.
            if(it == mst.end()){
                // If noNodesToVisit, then add current node and mark current item as lowest weight (first item in nodesToVisit)
                if(nodesToVisit.empty()) {
                    lowestWeightIdx = 0;  // Because list was empty
                    minWeight = currentNode->weightToOrigin;
                    nodesToVisit.push_back(currentNode);
                }
                    // Else if nodesToVisit not empty, compare the current nodes nextNode weight to
                else {
                    int currentWeight = currentNode->weightToOrigin;
                    // Compare nextNode weight to current Lowest weight in the NodesToVisit list
                    if(currentWeight < minWeight){
                        nodesToVisit.push_back(currentNode);
                        minWeight = currentWeight;
                        lowestWeightIdx = (nodesToVisit.size() - 1);  // Set the lowest weight index to the last item
                    }
                }
            }
        }
    }
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

    Graph readUndirected;
    filename = R"(C:\Users\Wolf\Dropbox\Grad School\Spring 2024\CS7350\Module 4\HW4Code\cmake-build-debug\undirectedGraph.csv)";
    readUndirected.readFromFile(filename);
    readUndirected.displayGraph();

    // Test MST
    filename = R"(C:\Users\Wolf\Dropbox\Grad School\Spring 2024\CS7350\Module 4\HW4Code\weightedImport.csv)";
    Graph mst;
    mst.readFromFile(filename);
    mst.displayGraph();
    mst.primsMinSpan();

    // Edge case to solve for: Reading in/writing out a graph with disconnected nodes (nodes with degree 0)
    std::cout << "done" << std::endl;
    return 0;
}