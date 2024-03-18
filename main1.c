#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct edge {
    struct node* node;
    int weight;
} Edge;

typedef struct node {
    char name[100];
    int isVisited;
    int isHospital;
    Edge** adjList;
} Node;

typedef struct hospital {
    char name[100];
    int emergencyCapacity;
    int generalCapacity;
    int occupiedEmergency;
    int occupiedGeneral;
    int availableAmbulance;
    int maxAmbulance;
} Hospital;

typedef struct graph {
    Node* nodes[10];
    Hospital* hospitals[5];
} Graph;

void dijkstra(Graph* graph, int src, int* hospitalDistances) {
    for (int i = 0; i < 5; i++) {
        hospitalDistances[i] = INT_MAX;
    }

    Node* sourceNode = graph->nodes[src];

    // Initialize distances array and source node's distance to zero
    int distances[10];
    for (int i = 0; i < 10; i++) {
        distances[i] = INT_MAX;
    }
    distances[src] = 0;

    // Implement Dijkstra's algorithm
    for (int i = 0; i < 10; i++) {
        int minDist = INT_MAX;
        int minNode = -1;

        for (int j = 0; j < 10; j++) {
            if (!graph->nodes[j]->isVisited && distances[j] < minDist) {
                minDist = distances[j];
                minNode = j;
            }
        }

        if (minNode == -1) {
            break; // No more reachable nodes
        }

        graph->nodes[minNode]->isVisited = 1;

        for (int k = 0; k < 5; k++) {
            if (graph->nodes[minNode]->isHospital) {
                hospitalDistances[k] = distances[minNode];
            }
        }

        for (int k = 0; k < 3; k++) {
            Edge* edge = graph->nodes[minNode]->adjList[k];
            if (edge && edge->node) {
                int nodeIndex = edge->node - graph->nodes[0]; // Calculate the index of the node
                int newDist = distances[minNode] + edge->weight;
                if (newDist < distances[nodeIndex]) {
                    distances[nodeIndex] = newDist;
                }
            }
        }
    }
}

void setAdjacencyList(Node *node, int numEdges, Edge *edges[])
{
    node->adjList = (Edge **)malloc(numEdges * sizeof(Edge *));

    for (int i = 0; i < numEdges; i++)
    {
        node->adjList[i] = (Edge*)malloc(sizeof(Edge));
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
    setAdjacencyList(graph->nodes[5], 3, (Edge *[]){&(Edge){graph->nodes[0], 50}, &(Edge){graph->nodes[9], 55}, &(Edge){graph->nodes[7], 60}});
    setAdjacencyList(graph->nodes[6], 1, (Edge *[]){&(Edge){graph->nodes[0], 65}});
    setAdjacencyList(graph->nodes[7], 3, (Edge *[]){&(Edge){graph->nodes[5], 70}, &(Edge){graph->nodes[1], 75}, &(Edge){graph->nodes[4], 80}});
    setAdjacencyList(graph->nodes[8], 2, (Edge *[]){&(Edge){graph->nodes[0], 85}, &(Edge){graph->nodes[2], 90}});
    setAdjacencyList(graph->nodes[9], 3, (Edge *[]){&(Edge){graph->nodes[5], 95}, &(Edge){graph->nodes[1], 100}, &(Edge){graph->nodes[3], 105}});

    return graph;
}


void printGraph(Graph *graph) {
    for (int i = 0; i < 10; i++) {
        printf("Node %s (Is Hospital: %d)\n", graph->nodes[i]->name, graph->nodes[i]->isHospital);

        if (graph->nodes[i]->adjList) {
            for (int j = 0; j < 3; j++) {
                if (graph->nodes[i]->adjList[j]) {
                    printf("  -> Connected to: %s (Weight: %d)\n", graph->nodes[i]->adjList[j]->node->name, graph->nodes[i]->adjList[j]->weight);
                }
            }
        }

        printf("\n");
    }

    // Print hospital information
    // for (int i = 0; i < 5; i++) {
    //     printf("Hospital %s\n", graph->hospitals[i]->name);
    //     printf("  Emergency Capacity: %d\n", graph->hospitals[i]->emergencyCapacity);
    //     printf("  General Capacity: %d\n", graph->hospitals[i]->generalCapacity);
    //     printf("  Max Ambulance: %d\n", graph->hospitals[i]->maxAmbulance);
    //     printf("\n");
    // }
}



void neudikstra(Graph* graph, int src, int* hospitalDistances, Node** nodesPtr)
{
    for(int i = 0; i < 5; i++)
    {
        hospitalDistances[i] = INT_MAX;
    }

    Node* sourceNode = graph->nodes[src];

    // Initialize distances array and source node's distance to zero
    int distances[10];
    int visited[10];
    for(int i = 0; i < 10; i++)
    {
        distances[i] = INT_MAX;
        visited[i] = 0;
    }
    distances[src] = 0;

    for(int i = 0;i<10;i++)
    {

    }

}

int main() {
    // Create the graph as per the provided createGraph function
    Graph* graph = createGraph();

    // Assume source node is "A" (0) and there are 5 hospitals
    int hospitalDistances[5];
    int sourceNode = 0;

    Node* nodes[5] = {graph->nodes[0], graph->nodes[1], graph->nodes[2], graph->nodes[3], graph->nodes[4]};
    Node** nodesPtr = nodes;

    neudikstra(graph, sourceNode, hospitalDistances, nodesPtr);

    // Call Dijkstra's algorithm
    dijkstra(graph, sourceNode, hospitalDistances);

    // Output the hospital distances from the source node
    printf("Distances from node %s to hospitals:\n", graph->nodes[sourceNode]->name);
    for (int i = 0; i < 5; i++) {
        printf("%s: %d\n", graph->hospitals[i]->name, hospitalDistances[i]);
    }

    return 0;
}
