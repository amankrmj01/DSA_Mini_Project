#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct edge
{
    struct node *node;
    int weight;
} Edge;

typedef struct node
{
    char name[100];
    int isVisited;
    int isHospital;
    int adjListCount;
    Edge **adjList;
} Node;

typedef struct hospital
{
    char name[100];
    int emergencyCapacity;
    int generalCapacity;
    int occupiedEmergency;
    int occupiedGeneral;
    int availableAmbulance;
    int maxAmbulance;
} Hospital;

typedef struct graph
{
    Node *nodes[10];
    Hospital *hospitals[5];
} Graph;

void setAdjacencyList(Node *node, int numEdges, Edge *edges[])
{
    node->adjList = (Edge **)malloc(numEdges * sizeof(Edge *));
    node->adjListCount = numEdges; // Set the adjacency list count

    for (int i = 0; i < numEdges; i++)
    {
        node->adjList[i] = (Edge *)malloc(sizeof(Edge));
        node->adjList[i]->node = edges[i]->node;
        node->adjList[i]->weight = edges[i]->weight;
    }
}

Graph *createGraph()
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    int n = 10;
    for (int i = 0; i < 10; ++i)
    {
        graph->nodes[i] = (Node *)malloc(sizeof(Node));
        graph->nodes[i]->isHospital = 0;
    }
    graph->nodes[0]->isHospital = 1;
    strcpy(graph->nodes[0]->name, "A");
    graph->nodes[1]->isHospital = 1;
    strcpy(graph->nodes[1]->name, "B");
    graph->nodes[2]->isHospital = 1;
    strcpy(graph->nodes[2]->name, "C");
    graph->nodes[3]->isHospital = 1;
    strcpy(graph->nodes[3]->name, "D");
    graph->nodes[4]->isHospital = 1;
    strcpy(graph->nodes[4]->name, "E");
    strcpy(graph->nodes[5]->name, "F");
    strcpy(graph->nodes[6]->name, "G");
    strcpy(graph->nodes[7]->name, "H");
    strcpy(graph->nodes[8]->name, "I");
    strcpy(graph->nodes[9]->name, "J");

    for (int i = 0; i < 5; ++i)
    {
        graph->hospitals[i] = (Hospital *)malloc(sizeof(Hospital));
        sprintf(graph->hospitals[i]->name, "Hospital %c", ' ' + i + 33);
        graph->hospitals[i]->emergencyCapacity = 10;
        graph->hospitals[i]->occupiedEmergency = 0;
        graph->hospitals[i]->occupiedGeneral = 0;
        graph->hospitals[i]->availableAmbulance = 0;
        graph->hospitals[i]->maxAmbulance = 5;
        graph->hospitals[i]->generalCapacity = 20;
    }

    // Creating adjacency list
    setAdjacencyList(graph->nodes[0], 3, (Edge *[]){&(Edge){graph->nodes[6], 10}, &(Edge){graph->nodes[5], 15}, &(Edge){graph->nodes[8], 20}});
    setAdjacencyList(graph->nodes[1], 2, (Edge *[]){&(Edge){graph->nodes[7], 25}, &(Edge){graph->nodes[9], 30}});
    setAdjacencyList(graph->nodes[2], 1, (Edge *[]){&(Edge){graph->nodes[8], 35}});
    setAdjacencyList(graph->nodes[3], 1, (Edge *[]){&(Edge){graph->nodes[9], 40}});
    setAdjacencyList(graph->nodes[4], 1, (Edge *[]){&(Edge){graph->nodes[7], 45}});
    setAdjacencyList(graph->nodes[5], 3, (Edge *[]){&(Edge){graph->nodes[0], 15}, &(Edge){graph->nodes[9], 55}, &(Edge){graph->nodes[7], 60}});
    setAdjacencyList(graph->nodes[6], 1, (Edge *[]){&(Edge){graph->nodes[0], 10}});
    setAdjacencyList(graph->nodes[7], 3, (Edge *[]){&(Edge){graph->nodes[5], 60}, &(Edge){graph->nodes[1], 25}, &(Edge){graph->nodes[4], 45}});
    setAdjacencyList(graph->nodes[8], 2, (Edge *[]){&(Edge){graph->nodes[0], 20}, &(Edge){graph->nodes[2], 35}});
    setAdjacencyList(graph->nodes[9], 3, (Edge *[]){&(Edge){graph->nodes[5], 55}, &(Edge){graph->nodes[1], 30}, &(Edge){graph->nodes[3], 40}});

    return graph;
}

void printGraph(Graph *graph)
{
    for (int i = 0; i < 10; i++)
    {
        Node *currentNode = graph->nodes[i];
        printf("Node %s: ", currentNode->name);

        for (int j = 0; j < currentNode->adjListCount; j++)
        {
            Edge *currentEdge = currentNode->adjList[j];
            printf("(%s, weight: %d)  ", currentEdge->node->name, currentEdge->weight);
        }
        printf("\n");
    }
}

int minDistanceIndex(int minDist[], int visited[])
{
    int min = INT_MAX, minIndex;
    for (int i = 0; i < 10; i++)
    {
        if (!visited[i] && minDist[i] < min)
        {
            min = minDist[i];
            minIndex = i;
        }
    }
    return minIndex;
}

int getNodeIndex(void **nodes, void *targetNode, int numNodes)
{
    for (int i = 0; i < numNodes; i++)
    {
        if (nodes[i] == targetNode)
        {
            return i;
        }
    }
    return -1; // Return -1 if the node is not found
}

Graph *primMST(Graph *graph)
{
    // Create a new graph for the MST
    Graph *mstGraph = createGraph();

    // Initialize arrays to track visited nodes and distances
    int visited[10] = {0};
    int minDist[10];
    for (int i = 0; i < 10; i++)
    {
        minDist[i] = INT_MAX;
    }

    // Set the source node's distance to 0
    minDist[0] = 0;

    // Choose the source node as the first node
    Node *currentNode = graph->nodes[0];

    // Perform Prim's algorithm
    for (int i = 0; i < 9; i++)
    {
        int minIndex = minDistanceIndex(minDist, visited);
        currentNode = graph->nodes[minIndex];
        visited[minIndex] = 1;

        // Update distances based on the current node's adjacency list
        for (int j = 0; j < currentNode->adjListCount; j++)
        {
            Node *nextNode = currentNode->adjList[j]->node;
            int weight = currentNode->adjList[j]->weight;
            int nextNodeIndex = getNodeIndex((void **)graph->nodes, nextNode, 10);
            if (!visited[nextNodeIndex] && weight < minDist[nextNodeIndex])
            {
                minDist[nextNodeIndex] = weight;
                // Add the minimum weighted edge to the MST graph
                setAdjacencyList(mstGraph->nodes[nextNodeIndex], 1, (Edge *[]){&(Edge){currentNode, minDist[nextNodeIndex]}});
            }
        }
    }

    return mstGraph;
}

int getNodesIndex(void **nodes, void *targetNode, int numNodes) {
    for (int i = 0; i < numNodes; i++) {
        printf("%d     %d    ",nodes[i],targetNode,nodes[i]);
        printf("%s    ",((Node *)nodes[i])->name);
        printf("%s\n",((Node *)targetNode)->name);
        if (nodes[i] == targetNode) {
            return i;
        }
    }
    // printf("Node not found    %s\n",((Node *)targetNode)->name);
    return -1; // If the node is not found
}

// Function to get the index with the minimum distance in the distances array
int minDistancesIndex(int distances[], int visited[], int numNodes) {
    int minDist = INT_MAX;
    int minIndex = -1;

    for (int i = 0; i < numNodes; i++) {
        if (!visited[i] && distances[i] <= minDist) {
            minDist = distances[i];
            minIndex = i;
        }
    }

    return minIndex;
}
int *calculateTimeToHospitals(Graph *graph, Node *sourceNode)
{
    printf("\n\n");
    int *timeToHospitals = (int *)malloc(5 * sizeof(int));
    int visited[10] = {0};
    int distances[10];

    // Initialize distances array with INT_MAX except for the source node
    for (int i = 0; i < 10; i++)
    {
        distances[i] = INT_MAX;
    }
    // printf("%d", graph->nodes);
    printf("one");
    distances[getNodesIndex((void **)graph->nodes, sourceNode, 10)] = 0;
    printf("one\n");

    // Apply Dijkstra's algorithm
    for (int i = 0; i < 9; i++)
    {
        int minDistIndex = minDistancesIndex(distances, visited,10);
        Node *currentNode = graph->nodes[minDistIndex];
        visited[minDistIndex] = 1;

        for (int j = 0; j < currentNode->adjListCount; j++)
        {
            Node *neighbor = currentNode->adjList[j]->node;
            int weight = currentNode->adjList[j]->weight;
            int neighborIndex = getNodesIndex((void **)graph->nodes, neighbor, 10);

            if (!visited[neighborIndex] && distances[minDistIndex] != INT_MAX &&
                distances[minDistIndex] + weight < distances[neighborIndex])
            {
                distances[neighborIndex] = distances[minDistIndex] + weight;
            }
        }
    }

    // Extract hospital distances
    for (int i = 0; i < 5; i++)
    {
        int hospitalIndex = getNodesIndex((void **)graph->nodes, graph->hospitals[i], 10);
        timeToHospitals[i] = distances[i];
    }

    return timeToHospitals;
}

void printTimeToHospitals(Node *sourceNode, int *timeToHospitals, Hospital *hospitals[])
{
    printf("Time taken from source node %s to reach each hospital:\n", sourceNode->name);
    for (int i = 0; i < 5; i++)
    {
        printf("Hospital %s: %d\n", hospitals[i]->name, timeToHospitals[i]);
    }
    printf("\n");
}

int main()
{
    // Create the graph as per the provided createGraph function
    Graph *graph = createGraph();

    // Assume source node is "A" (0) and there are 5 hospitals
    int hospitalDistances[5];

    Node *nodes[5] = {graph->nodes[0], graph->nodes[1], graph->nodes[2], graph->nodes[3], graph->nodes[4]};
    Node **nodesPtr = nodes;

    printGraph(graph);
    Graph *mstGraph = primMST(graph);

    // Print the Minimum Spanning Tree
    printf("\nMinimum Spanning Tree (MST):\n");
    printGraph(mstGraph);

    Node *sourceNode = mstGraph->nodes[3]; // Node D as source

    int *timeToHospitals = calculateTimeToHospitals(mstGraph, sourceNode);

    // Print the time taken to reach each hospital from the source node
    printTimeToHospitals(sourceNode, timeToHospitals, mstGraph->hospitals);

    // Clean up
    free(timeToHospitals);

    return 0;
}
