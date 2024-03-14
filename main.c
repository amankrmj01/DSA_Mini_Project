#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    char name[100];
    int isHospital;
    Node **adjList;
} Node;

typedef struct edge {
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


Graph* createGraph()
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
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
        strcpy(graph->hospitals[i]->name, "Hospital "+(i+1));
        graph->hospitals[i]->occupiedEmergency = 0;
        graph->hospitals[i]->occupiedGeneral = 0;
        graph->hospitals[i]->availableAmbulance = 0;
        graph->hospitals[i]->maxAmbulance = 5;
        graph->hospitals[i]->ermergencyCapacity = 10;
        graph->hospitals[i]->GenralCapacity = 20;
    }

    // Create adjacency list
    graph->nodes[0]->adjList = (Node **)malloc(3 * sizeof(Node *));
    graph->nodes[0]->adjList[0] = graph->nodes[6]; // G
    graph->nodes[0]->adjList[1] = graph->nodes[5]; // F
    graph->nodes[0]->adjList[2] = graph->nodes[8]; // I

    graph->nodes[1]->adjList = (Node **)malloc(2 * sizeof(Node *));
    graph->nodes[1]->adjList[0] = graph->nodes[7]; // H
    graph->nodes[1]->adjList[1] = graph->nodes[9]; // J

    graph->nodes[2]->adjList = (Node **)malloc(sizeof(Node *));
    graph->nodes[2]->adjList[0] = graph->nodes[8]; // I

    graph->nodes[3]->adjList = (Node **)malloc(sizeof(Node *));
    graph->nodes[3]->adjList[0] = graph->nodes[9]; // J

    graph->nodes[4]->adjList = (Node **)malloc(sizeof(Node *));
    graph->nodes[4]->adjList[0] = graph->nodes[7]; // H

    graph->nodes[5]->adjList = (Node **)malloc(3 * sizeof(Node *));
    graph->nodes[5]->adjList[0] = graph->nodes[0]; // A
    graph->nodes[5]->adjList[1] = graph->nodes[9]; // J
    graph->nodes[5]->adjList[2] = graph->nodes[7]; // H

    graph->nodes[6]->adjList = (Node **)malloc(sizeof(Node *));
    graph->nodes[6]->adjList[0] = graph->nodes[0]; // A

    graph->nodes[7]->adjList = (Node **)malloc(3 * sizeof(Node *));
    graph->nodes[7]->adjList[0] = graph->nodes[5]; // F
    graph->nodes[7]->adjList[1] = graph->nodes[1]; // B
    graph->nodes[7]->adjList[2] = graph->nodes[4]; // E

    graph->nodes[8]->adjList = (Node **)malloc(2 * sizeof(Node *));
    graph->nodes[8]->adjList[0] = graph->nodes[0]; // A
    graph->nodes[8]->adjList[1] = graph->nodes[2]; // C

    graph->nodes[9]->adjList = (Node **)malloc(3 * sizeof(Node *));
    graph->nodes[9]->adjList[0] = graph->nodes[5]; // F
    graph->nodes[9]->adjList[1] = graph->nodes[1]; // B
    graph->nodes[9]->adjList[2] = graph->nodes[3]; // D


    return graph;

}


int createWeight()
{
    return (rand() % 29 + 2) * 5;
}

void callAmbulance(Graph *graph, Node *source, Node *destination)
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

void adjacentHospitals(Graph *graph, Node *source)
{
    for (int i = 0; i < 5; ++i)
    {
        if (source->adjList[i]->isHospital)
        {
            printf("%s\n", source->adjList[i]->name);
        }
    }
}

int main()
{
    return 0;
}