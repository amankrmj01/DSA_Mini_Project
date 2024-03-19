#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t lock;

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
typedef struct
{
    int duration;
    Hospital *hospital;
} AmbulanceArgs;

#define INF 99999999

typedef struct queue
{
    int items[100];
    int front;
    int rear;
} Queue;

void setAdjacencyList(Node *node, int numEdges, Edge *edges[])
{
    node->adjList = (Edge **)malloc(numEdges * sizeof(Edge *));
    node->adjListCount = numEdges; 

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
        graph->hospitals[i]->maxEmergency = 10;
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
    return -1;
}

Graph *primMST(Graph *graph)
{
    Graph *mstGraph = createGraph();

    int visited[10] = {0};
    int minDist[10];
    for (int i = 0; i < 10; i++)
    {
        minDist[i] = INT_MAX;
    }
    minDist[0] = 0;

    Node *currentNode = graph->nodes[0];

    for (int i = 0; i < 9; i++)
    {
        int minIndex = minDistanceIndex(minDist, visited);
        currentNode = graph->nodes[minIndex];
        visited[minIndex] = 1;

        for (int j = 0; j < currentNode->adjListCount; j++)
        {
            Node *nextNode = currentNode->adjList[j]->node;
            int weight = currentNode->adjList[j]->weight;
            int nextNodeIndex = getNodeIndex((void **)graph->nodes, nextNode, 10);
            if (!visited[nextNodeIndex] && weight < minDist[nextNodeIndex])
            {
                minDist[nextNodeIndex] = weight;
                setAdjacencyList(mstGraph->nodes[nextNodeIndex], 1, (Edge *[]){&(Edge){currentNode, minDist[nextNodeIndex]}});
            }
        }
    }

    return mstGraph;
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

int getNodeSIndex(void **nodes, void *targetNode, int numNodes)
{
    for (int i = 0; i < numNodes; i++)
    {
        if (strcmp(((Node *)nodes[i])->name, ((Node *)targetNode)->name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void bfs(Graph *graph, Node *startNode, int *distances)
{
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

void *return_ambulance(void *arg)
{
    AmbulanceArgs *args = (AmbulanceArgs *)arg;
    int duration = args->duration;
    Hospital *hospital = args->hospital;
    sleep(duration * 2);
    pthread_mutex_lock(&lock);
    hospital->availableAmbulance++;
    printf("Ambulance returned to %s and is now available.\n", hospital->name);
    pthread_mutex_unlock(&lock);
    return NULL;
}

void adjustAmbulanceAvailability(Hospital *hospital, int distance, char *source)
{
    pthread_mutex_lock(&lock);
    if (hospital->availableAmbulance > 0)
    {
        hospital->availableAmbulance--;
        printf("Ambulance dispatched to %s from %s Available ambulances: %d\n", source, hospital->name, hospital->availableAmbulance);
        int *duration = malloc(sizeof(int));
        *duration = distance;
        AmbulanceArgs *args = malloc(sizeof(AmbulanceArgs));
        args->duration = distance;
        args->hospital = hospital;
        pthread_t thread;
        pthread_create(&thread, NULL, return_ambulance, (void *)args);
        pthread_detach(thread);
        free(args);
    }
    else
    {
        printf("No ambulances available at %s. Please wait.\n", hospital->name);
    }
    pthread_mutex_unlock(&lock);
}

void isHospitalAvailable(Hospital *hospital, int distance, char *source)
{
    if (hospital->availableAmbulance > 0)
    {
        printf("Ambulance is available at %s\n", hospital->name);
        adjustAmbulanceAvailability(hospital, distance, source);
    }
    else
    {
        printf("Ambulance is not available at %s\n", hospital->name);
    }
}

void nearestHospital(Graph *graph, int distance[], int location, char *source)
{
    int min = INT_MAX;
    int index = -1;

    int distanceCopy[5];
    memcpy(distanceCopy, distance, sizeof(distanceCopy));
    int indexCopy[5] = {0, 1, 2, 3, 4};

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4 - i; j++)
        {
            if (distanceCopy[j] > distanceCopy[j + 1])
            {
                int temp = distanceCopy[j];
                distanceCopy[j] = distanceCopy[j + 1];
                distanceCopy[j + 1] = temp;

                int tempIndex = indexCopy[j];
                indexCopy[j] = indexCopy[j + 1];
                indexCopy[j + 1] = tempIndex;
            }
        }
    }

    printf("IndexCopy: ");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", indexCopy[i]);
    }

    printf("\n");
    printf("Nearest Hospital is %s at a distance of %d\n", graph->hospitals[indexCopy[0]]->name, distance[indexCopy[0]]);
    isHospitalAvailable(graph->hospitals[indexCopy[0]], distance[indexCopy[0]], source);
}

int main()
{
    pthread_mutex_init(&lock, NULL);

    Graph *graph = createGraph();

    int hospitalDistances[5];

    printGraph(graph);
    Graph *mstGraph = primMST(graph);

    printf("\nMinimum Spanning Tree (MST):\n");
    printGraph(mstGraph);

    int distances[10] = {0};

    struct node *sourceNode;
    char *source;

    int choice;
    do
    {
        printf("\n------ Menu ------\n");
        printf("1. Emergency\n");
        printf("2. General\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            printf("Enter the location of the emergency: ");
            int emergencyLocation;
            scanf("%d", &emergencyLocation);
            sourceNode = graph->nodes[emergencyLocation];
            source = sourceNode->name;

            printf("\n--- Emergency Case ---\n");
            bfs(graph, sourceNode, distances);
            nearestHospital(graph, distances, emergencyLocation, source);
            break;
        }
        case 2:
        {
            printf("\n--- General Case ---\n");
            printf("Enter the patient details:\n");
            break;
        }
        case 3:
            printf("Exiting program. Goodbye!\n");
            break;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 3);

    return 0;
}
