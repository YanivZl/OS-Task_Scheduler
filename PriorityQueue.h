#include <stdlib.h>
#include "Task.h"

//Node
typedef struct node {
    Task t;
 
    // Lower values indicate higher priority
    int priority;
 
    struct node* next;
 
} Node;

Node* newNode(int d, int p)
{
    Node* temp = (Node*)malloc(sizeof(Node));

    

    temp->next = NULL;
 
    return temp;
}