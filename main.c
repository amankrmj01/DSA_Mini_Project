#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct edge Edge;

typedef struct node
{
    char name[100];
    int isHospital;
    Edge **adjList;
} Node;

typedef struct edge
{
    Node *node;
    int weight;
} Edge;

typedef struct hospital
{
    char name[100];
    int ermergencyCapacity;
    int GenralCapacity;
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
        strcpy(graph->hospitals[i]->name, "Hospital " + (i + 1));
        graph->hospitals[i]->occupiedEmergency = 0;
        graph->hospitals[i]->occupiedGeneral = 0;
        graph->hospitals[i]->availableAmbulance = 0;
        graph->hospitals[i]->maxAmbulance = 5;
        graph->hospitals[i]->ermergencyCapacity = 10;
        graph->hospitals[i]->GenralCapacity = 20;
    }

    // // Create adjacency list
    // // For node A
    // graph->nodes[0]->adjList = (Edge **)malloc(3 * sizeof(Edge *));
    // graph->nodes[0]->adjList[0] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[0]->adjList[0]->node = graph->nodes[6]; // G
    // graph->nodes[0]->adjList[0]->weight = 10;

    // graph->nodes[0]->adjList[1] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[0]->adjList[1]->node = graph->nodes[5]; // F
    // graph->nodes[0]->adjList[1]->weight = 15;

    // graph->nodes[0]->adjList[2] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[0]->adjList[2]->node = graph->nodes[8]; // I
    // graph->nodes[0]->adjList[2]->weight = 20;

    // // For node B
    // graph->nodes[1]->adjList = (Edge **)malloc(2 * sizeof(Edge *));
    // graph->nodes[1]->adjList[0] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[1]->adjList[0]->node = graph->nodes[7]; // H
    // graph->nodes[1]->adjList[0]->weight = 25;

    // graph->nodes[1]->adjList[1] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[1]->adjList[1]->node = graph->nodes[9]; // J
    // graph->nodes[1]->adjList[1]->weight = 30;

    // // For node C
    // graph->nodes[2]->adjList = (Edge **)malloc(sizeof(Edge *));
    // graph->nodes[2]->adjList[0] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[2]->adjList[0]->node = graph->nodes[8]; // I
    // graph->nodes[2]->adjList[0]->weight = 35;

    // // For node D
    // graph->nodes[3]->adjList = (Edge **)malloc(sizeof(Edge *));
    // graph->nodes[3]->adjList[0] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[3]->adjList[0]->node = graph->nodes[9]; // J
    // graph->nodes[3]->adjList[0]->weight = 40;

    // // For node E
    // graph->nodes[4]->adjList = (Edge **)malloc(sizeof(Edge *));
    // graph->nodes[4]->adjList[0] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[4]->adjList[0]->node = graph->nodes[7]; // H
    // graph->nodes[4]->adjList[0]->weight = 45;

    // // For node F
    // graph->nodes[5]->adjList = (Edge **)malloc(3 * sizeof(Edge *));
    // graph->nodes[5]->adjList[0] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[5]->adjList[0]->node = graph->nodes[0]; // A
    // graph->nodes[5]->adjList[0]->weight = 50;

    // graph->nodes[5]->adjList[1] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[5]->adjList[1]->node = graph->nodes[9]; // J
    // graph->nodes[5]->adjList[1]->weight = 55;

    // graph->nodes[5]->adjList[2] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[5]->adjList[2]->node = graph->nodes[7]; // H
    // graph->nodes[5]->adjList[2]->weight = 60;

    // // For node G
    // graph->nodes[6]->adjList = (Edge **)malloc(sizeof(Edge *));
    // graph->nodes[6]->adjList[0] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[6]->adjList[0]->node = graph->nodes[0]; // A
    // graph->nodes[6]->adjList[0]->weight = 65;

    // // For node H
    // graph->nodes[7]->adjList = (Edge **)malloc(3 * sizeof(Edge *));
    // graph->nodes[7]->adjList[0] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[7]->adjList[0]->node = graph->nodes[5]; // F
    // graph->nodes[7]->adjList[0]->weight = 70;

    // graph->nodes[7]->adjList[1] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[7]->adjList[1]->node = graph->nodes[1]; // B
    // graph->nodes[7]->adjList[1]->weight = 75;

    // graph->nodes[7]->adjList[2] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[7]->adjList[2]->node = graph->nodes[4]; // E
    // graph->nodes[7]->adjList[2]->weight = 80;

    // // For node I
    // graph->nodes[8]->adjList = (Edge **)malloc(2 * sizeof(Edge *));
    // graph->nodes[8]->adjList[0] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[8]->adjList[0]->node = graph->nodes[0]; // A
    // graph->nodes[8]->adjList[0]->weight = 85;

    // graph->nodes[8]->adjList[1] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[8]->adjList[1]->node = graph->nodes[2]; // C
    // graph->nodes[8]->adjList[1]->weight = 90;

    // // For node J
    // graph->nodes[9]->adjList = (Edge **)malloc(3 * sizeof(Edge *));
    // graph->nodes[9]->adjList[0] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[9]->adjList[0]->node = graph->nodes[5]; // F
    // graph->nodes[9]->adjList[0]->weight = 95;

    // graph->nodes[9]->adjList[1] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[9]->adjList[1]->node = graph->nodes[1]; // B
    // graph->nodes[9]->adjList[1]->weight = 100;

    // graph->nodes[9]->adjList[2] = (Edge *)malloc(sizeof(Edge));
    // graph->nodes[9]->adjList[2]->node = graph->nodes[3]; // D
    // graph->nodes[9]->adjList[2]->weight = 105;

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

// int calculateTime(Graph *graph, Node *source, Node *destination)
// {
//     int minTime = INT_MAX;
//     Node *nearestHospital = NULL;

//     // Find the nearest hospital
//     for (int i = 0; i < 5; ++i)
//     {
//         if (graph->hospitals[i]->availableAmbulance > 0)
//         {
//             int time = calculateDistance(source, graph->hospitals[i]->node) + calculateDistance(graph->hospitals[i]->node, destination);
//             if (time < minTime)
//             {
//                 minTime = time;
//                 nearestHospital = graph->hospitals[i]->node;
//             }
//         }
//     }

//     if (nearestHospital != NULL)
//     {
//         callAmbulance(graph, source, nearestHospital);
//         callAmbulance(graph, nearestHospital, destination);
//         return minTime;
//     }
//     else
//     {
//         printf("No ambulance available\n");
//         return -1;
//     }
// }

// int calculateDistance(Graph *graph, Node *source)
// {
//     // Create a priority queue to store nodes with their distances
//     PriorityQueue *queue = createPriorityQueue();

//     // Set the distance of the source node to 0 and enqueue it
//     source->distance = 0;
//     enqueue(queue, source);

//     // Create a list to store hospitals available nearest from the source node
//     List *hospitalList = createList();

//     while (!isEmpty(queue))
//     {
//         // Dequeue the node with the minimum distance
//         Node *currentNode = dequeue(queue);

//         // Iterate through the adjacent nodes of the current node
//         for (int i = 0; i < currentNode->numAdjacentNodes; i++)
//         {
//             Node *adjacentNode = currentNode->adjacentNodes[i];
//             int distance = currentNode->distance + calculateEdgeWeight(currentNode, adjacentNode);

//             // If the new distance is smaller than the current distance, update the distance and enqueue the adjacent node
//             if (distance < adjacentNode->distance)
//             {
//                 adjacentNode->distance = distance;
//                 enqueue(queue, adjacentNode);

//                 // Check if the adjacent node is a hospital and add it to the hospital list
//                 if (adjacentNode->isHospital)
//                 {
//                     insertAtEnd(hospitalList, adjacentNode);
//                 }
//             }
//         }
//     }

//     // Create an array to store the hospital list in order of nearest
//     Node *nearestHospitals[hospitalList->size];
//     int index = 0;

//     // Iterate through the hospital list and add hospitals to the array
//     Node *currentHospital = hospitalList->head;
//     while (currentHospital != NULL)
//     {
//         nearestHospitals[index] = currentHospital;
//         currentHospital = currentHospital->next;
//         index++;
//     }

//     // Check the hospitals in order of nearest
//     for (int i = 0; i < index; i++)
//     {
//         Node *hospital = nearestHospitals[i];

//         // Check if the hospital has an available ambulance and emergency bed
//         if (hospital->availableAmbulance > 0 && hospital->emergencyBedAvailable)
//         {
//             // Send the destination node
//             return calculateDistance(source, hospital);
//         }
//     }

//     // If no hospital satisfies the conditions, return -1
//     return -1;
// }

// void callAmbulance(Graph *graph, Node *source, Node *destination)
{
    for (int i = 0; i < 5; ++i)
    {
        if (graph->hospitals[i]->availableAmbulance > 0)
        {
            graph->hospitals[i]->availableAmbulance--;
            printf("Ambulance called from %s to %s\n", source->name, destination->name);
            return;
        }
    }
    printf("No ambulance available\n");
}

int main()
{
    return 0;
}