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
    int availableEmergency;
    int maxEmergency;
    int availableGeneral;
    int maxGeneral;
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
        graph->hospitals[i]->availableEmergency = 10;
        graph->hospitals[i]-> maxEmergency= 10;
        graph->hospitals[i]->maxGeneral = 20;
        graph->hospitals[i]->availableGeneral = 20;
        graph->hospitals[i]->availableAmbulance = 5;
        graph->hospitals[i]->maxAmbulance = 5;
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

#define INF 99999999

typedef struct queue
{
    int items[100];
    int front;
    int rear;
} Queue;

Queue *createQueue();
void enqueue(Queue *q, int);
int dequeue(Queue *q);
int isEmpty(Queue *q);

int getNodeSIndex(void **nodes, void *targetNode, int numNodes)
{
    for (int i = 0; i < numNodes; i++)
    {
        if (strcmp(((Node *)nodes[i])->name, ((Node *)targetNode)->name) == 0)
        {
            return i;
        }
    }
    return -1; // Return -1 if the node is not found
}

void bfs(Graph *graph, Node *startNode, int *distances)
{
    printf("BFS\n");
    Queue *q = createQueue();

    for (int i = 0; i < 10; i++)
    {
        distances[i] = INF;
    }

    int startNodeIndex = getNodeSIndex((void **)graph->nodes, startNode, 10);
    distances[startNodeIndex] = 0;

    enqueue(q, startNodeIndex);

    while (!isEmpty(q))
    {
        int currentNodeIndex = dequeue(q);
        Node *currentNode = graph->nodes[currentNodeIndex];

        for (int i = 0; i < currentNode->adjListCount; i++)
        {
            Edge *edge = currentNode->adjList[i];
            int adjNodeIndex = getNodeSIndex((void **)graph->nodes, edge->node, 10);

            if (distances[currentNodeIndex] + edge->weight < distances[adjNodeIndex])
            {
                distances[adjNodeIndex] = distances[currentNodeIndex] + edge->weight;
                enqueue(q, adjNodeIndex);
            }
        }
    }
}

Queue *createQueue()
{
    Queue *q = malloc(sizeof(Queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

int isEmpty(Queue *q)
{
    if (q->rear == -1)
        return 1;
    else
        return 0;
}

void enqueue(Queue *q, int value)
{
    if (q->rear == 100 - 1)
    {
    }
    else
    {
        if (q->front == -1)
            q->front = 0;
        q->rear++;
        q->items[q->rear] = value;
    }
}

int dequeue(Queue *q)
{
    int item;
    if (isEmpty(q))
    {
        item = -1;
    }
    else
    {
        item = q->items[q->front];
        q->front++;
        if (q->front > q->rear)
        {
            q->front = q->rear = -1;
        }
    }
    return item;
}

void isHospitalAvailable(Hospital *hospital)
{
    if (hospital->availableAmbulance > 0)
    {
        printf("Ambulance is available at %s\n", hospital->name);
    }
    else
    {
        printf("Ambulance is not available at %s\n", hospital->name);
    }
}

void nearestHospital(Graph *graph, int distance[], int location)
{
    int min = INT_MAX;
    int index = -1;
    while (1)
    {
        for (int i = 0; i < 5; i++)
        {
            if (i == location)
            {
                continue;
            }
            if (distance[i] < min)
            {
                min = distance[i];
                index = i;
            }
        }
        isHospitalAvailable(graph->hospitals[index]);
        break;
    }
    printf("Nearest Hospital is %s at a distance of %d\n", graph->hospitals[index]->name, distance[index]);
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

    int distances[10] = {0};

    // Call the bfs function
    printf("\nDistances from source node to all other nodes:\n");
    int location = 9;
    struct node *sourceNode = graph->nodes[location]; // Node D as source

    bfs(graph, sourceNode, distances); // if G is at index 6

    // Print the distances from the source node to all other nodes
    for (int i = 0; i < 10; i++)
    {
        printf("Distance from source to node %d: %d\n", i, distances[i]);
    }
    printf("\n\n");

    nearestHospital(graph, distances, location);

    return 0;
}
