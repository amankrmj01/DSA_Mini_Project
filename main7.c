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
    int adjListCount;
    Edge **adjList;
} Node;

typedef struct hospital
{
    char name[100];
    int availableEmergency;
    int availableGeneral;
    int availableAmbulance;
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

typedef struct
{
    Hospital *hospital;
} GeneralArgs;

#define INF 99999999

typedef struct queue
{
    int items[100];
    int front;
    int rear;
} Queue;

void hospitalLog(char *hospitalName, char *log)
{
    char filename[105];
    sprintf(filename, "%s.txt", hospitalName);
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }
    fprintf(file, "%s", log);
    fclose(file);
}

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
    }
    strcpy(graph->nodes[0]->name, "A");
    strcpy(graph->nodes[1]->name, "B");
    strcpy(graph->nodes[2]->name, "C");
    strcpy(graph->nodes[3]->name, "D");
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
        graph->hospitals[i]->availableEmergency = 5;
        graph->hospitals[i]->availableGeneral = 20;
        graph->hospitals[i]->availableAmbulance = 5;
    }

    setAdjacencyList(graph->nodes[0], 3, (Edge *[]){&(Edge){graph->nodes[6], 15}, &(Edge){graph->nodes[5], 35}, &(Edge){graph->nodes[8], 25}});
    setAdjacencyList(graph->nodes[1], 2, (Edge *[]){&(Edge){graph->nodes[7], 25}, &(Edge){graph->nodes[9], 15}});
    setAdjacencyList(graph->nodes[2], 1, (Edge *[]){&(Edge){graph->nodes[8], 20}});
    setAdjacencyList(graph->nodes[3], 1, (Edge *[]){&(Edge){graph->nodes[9], 40}});
    setAdjacencyList(graph->nodes[4], 1, (Edge *[]){&(Edge){graph->nodes[7], 20}});
    setAdjacencyList(graph->nodes[5], 3, (Edge *[]){&(Edge){graph->nodes[0], 35}, &(Edge){graph->nodes[9], 10}, &(Edge){graph->nodes[7], 15}});
    setAdjacencyList(graph->nodes[6], 1, (Edge *[]){&(Edge){graph->nodes[0], 15}});
    setAdjacencyList(graph->nodes[7], 3, (Edge *[]){&(Edge){graph->nodes[5], 15}, &(Edge){graph->nodes[1], 25}, &(Edge){graph->nodes[4], 20}});
    setAdjacencyList(graph->nodes[8], 2, (Edge *[]){&(Edge){graph->nodes[0], 25}, &(Edge){graph->nodes[2], 20}});
    setAdjacencyList(graph->nodes[9], 3, (Edge *[]){&(Edge){graph->nodes[5], 10}, &(Edge){graph->nodes[1], 15}, &(Edge){graph->nodes[3], 40}});

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

void dfs(Graph *graph, Node *startNode, int *distances)
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

void *emptyEmergencyBed(void *arg)
{
    AmbulanceArgs *args = (AmbulanceArgs *)arg;
    Hospital *hospital = args->hospital;
    sleep(5 * 60);
    pthread_mutex_lock(&lock);
    hospital->availableEmergency++;
    printf("\nEmergency bed at %s is now available.\n", hospital->name);
    char str[200];
    sprintf(str, "Emergency bed at %s is now available.\n", hospital->name);
    hospitalLog(hospital->name, str);
    printf("\n------ Menu ------\n");
    printf("1. Emergency\n");
    printf("2. General\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    pthread_mutex_unlock(&lock);
    return NULL;
}

void adjustEmergencyAvailability(Hospital *hospital, char *source)
{
    pthread_mutex_lock(&lock);
    if (hospital->availableEmergency > 0)
    {
        hospital->availableEmergency--;
        printf("\nEmergency bed occupied at %s. Available beds: %d\n", hospital->name, hospital->availableEmergency);
        char str[200];
        sprintf(str, "Emergency bed occupied at %s. Available beds: %d\n", hospital->name, hospital->availableEmergency);
        hospitalLog(hospital->name, str);
        AmbulanceArgs *args = malloc(sizeof(AmbulanceArgs));
        args->hospital = hospital;
        pthread_t thread;
        pthread_create(&thread, NULL, emptyEmergencyBed, (void *)args);
        pthread_detach(thread);
        free(args);
    }
    else
    {
        printf("No emergency beds available at %s. Please wait.\n", hospital->name);
    }
    pthread_mutex_unlock(&lock);
}

void *return_ambulance(void *arg)
{
    AmbulanceArgs *args = (AmbulanceArgs *)arg;
    int duration = args->duration;
    Hospital *hospital = args->hospital;
    if (duration == 0)
        duration = 2;
    sleep(duration * 2);
    pthread_mutex_lock(&lock);
    hospital->availableAmbulance++;
    printf("\nAmbulance returned to %s and is now available.\n", hospital->name);
    char str[200];
    sprintf(str, "Ambulance returned to %s and is now available.\n", hospital->name);
    hospitalLog(hospital->name, str);
    printf("\n------ Menu ------\n");
    printf("1. Emergency\n");
    printf("2. General\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    pthread_mutex_unlock(&lock);
    return NULL;
}

void adjustAmbulanceAvailability(Hospital *hospital, int distance, char *source)
{
    pthread_mutex_lock(&lock);
    if (hospital->availableAmbulance > 0)
    {
        hospital->availableAmbulance--;
        printf("\nAmbulance dispatched to %s from %s Available ambulances: %d\n", source, hospital->name, hospital->availableAmbulance);
        char str[200];
        sprintf(str, "Ambulance dispatched to %s from %s Available ambulances: %d\n", source, hospital->name, hospital->availableAmbulance);
        hospitalLog(hospital->name, str);
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

int isAmbulanceAvailable(Hospital *hospital)
{
    if (hospital->availableAmbulance > 0)
        return 0;
    else
        return 1;
}

int isEmergencyAvailable(Hospital *hospital)
{
    if (hospital->availableEmergency > 0)
        return 0;
    else
        return 1;
}

void nearestHospital(Graph *graph, int distance[], int location, char *source, int emergency)
{
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

    // printf("Index: ");
    // for (int i = 0; i < 5; i++)
    // {
    //     printf("%d ", indexCopy[i]);
    // }

    if (emergency != 1)
    {
        printf("\nNearest Hospital in order:\n");
        for (int i = 0; i < 5; i++)
        {
            printf("%s\t %d\n", graph->hospitals[indexCopy[i]]->name, distance[indexCopy[i]]);
        }
        return;
    }

    printf("\n");

    {
        int i = 0, j = 0;

        int totaldistance = 0;
        int ambIndex = 0;
        int hospIndex = 0;
        while (i < 5)
        {
            if (isAmbulanceAvailable(graph->hospitals[indexCopy[i]]))
                i++;
            else
            {
                totaldistance = distance[indexCopy[i]];
                ambIndex = indexCopy[i];
                break;
            }
        }

        while (j < 5)
        {

            if (isEmergencyAvailable(graph->hospitals[indexCopy[j]]))
                j++;
            else
            {
                totaldistance += distance[indexCopy[j]];
                hospIndex = indexCopy[j];
                break;
            }
        }
        printf("Nearest Hospital is %s at a distance of %d\n and amulace will arrive in %d from %s\n", graph->hospitals[hospIndex]->name, distance[hospIndex], distance[ambIndex], graph->hospitals[ambIndex]->name);
        adjustAmbulanceAvailability(graph->hospitals[ambIndex], totaldistance, source);
        adjustEmergencyAvailability(graph->hospitals[hospIndex], source);
    }
}

void *return_general(void *arg)
{
    GeneralArgs *args = (GeneralArgs *)arg;
    sleep(120);
    pthread_mutex_lock(&lock);
    args->hospital->availableGeneral++;
    printf("\nGeneral slot freed at %s and is now available.\n", args->hospital->name);
    char str[200];
    sprintf(str, "General slot freed at %s and is now available.\n", args->hospital->name);
    hospitalLog(args->hospital->name, str);
    pthread_mutex_unlock(&lock);
    free(args);
    return NULL;
}

void adjustGeneralAvailability(Hospital *hospital)
{
    pthread_mutex_lock(&lock);
    if (hospital->availableGeneral > 0)
    {
        hospital->availableGeneral--;
        printf("\nGeneral slot booked at %s. Available general slots: %d\n", hospital->name, hospital->availableGeneral);
        char str[200];
        sprintf(str, "\nGeneral slot booked at %s. Available general slots: %d\n", hospital->name, hospital->availableGeneral);
        hospitalLog(hospital->name, str);
        GeneralArgs *args = malloc(sizeof(GeneralArgs));
        args->hospital = hospital;
        pthread_t thread;
        pthread_create(&thread, NULL, return_general, (void *)args);
        pthread_detach(thread);
    }
    else
    {
        printf("\nNo general slots are available at %s.\n", hospital->name);
    }
    pthread_mutex_unlock(&lock);
}

void getdetails(char *hospitalName)
{
    printf("Enter the patient details:\n");
    char filename[105];
    sprintf(filename, "%s.txt", hospitalName);
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }

    char name[100];
    int age;
    printf("Enter name: ");
    scanf("%s", name);
    printf("Enter age: ");
    scanf("%d", &age);

    fprintf(file, "Name: %s, Age: %d\n", name, age);

    fclose(file);
}

void isGeneralAvailable(Hospital *hospital)
{
    if (hospital->availableGeneral > 0)
    {
        printf("\nGeneral slots available at %s.\n", hospital->name);
        getdetails(hospital->name);
        adjustGeneralAvailability(hospital);
    }
    else
    {
        printf("\nNo slots are available at %s.\n", hospital->name);
    }
}

void bookGeneralSlot(Graph *graph, int distances[])
{
    char generalLocation;
    int c;
    int generalLocationIndex1;
    do
    {
        printf("\nEnter Hospital in which you want to go : ");
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
        scanf("%c", &generalLocation);
        generalLocationIndex1 = (int)generalLocation - 'A';
        if (graph->hospitals[generalLocationIndex1]->availableGeneral == 0)
        {
            printf("\nAll slots are full at %s. Please choose another hospital.\n", graph->hospitals[generalLocationIndex1]->name);
        }
    } while (graph->hospitals[generalLocationIndex1]->availableGeneral == 0);
    isGeneralAvailable(graph->hospitals[generalLocationIndex1]);
}

int main()
{
    pthread_mutex_init(&lock, NULL);

    Graph *graph = createGraph();

    printGraph(graph);

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
            char emergencyLocation;
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            scanf("%c", &emergencyLocation);

            int emergencyLocationIndex = (int)emergencyLocation - 'A';
            sourceNode = graph->nodes[emergencyLocationIndex];
            source = sourceNode->name;

            printf("\n--- Emergency Case ---\n");
            dfs(graph, sourceNode, distances);
            for (int i = 0; i < 10; i++)
            {
                printf("Distance from source to node %d: %d\n", i, distances[i]);
            }
            printf("\n\n");
            nearestHospital(graph, distances, emergencyLocation, source, 1);
            break;
        }
        case 2:
        {
            printf("\n--- General Case ---\n");
            printf("Enter your location : ");
            char generalLocation;
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            scanf("%c", &generalLocation);

            int generalLocationIndex = (int)generalLocation - 'A';
            sourceNode = graph->nodes[generalLocationIndex];
            source = sourceNode->name;
            dfs(graph, sourceNode, distances);
            printf("Distance from your location %s to other Hospitals:\n", sourceNode->name);
            // for (int i = 0; i < 5; i++)
            // {
            //     printf("%s\t %d\n", graph->hospitals[i]->name,distances[i]);
            // }
            nearestHospital(graph, distances, generalLocation, source, 0);
            bookGeneralSlot(graph, distances);

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
