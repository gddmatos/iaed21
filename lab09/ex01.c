#include <stdio.h>
#include <stdlib.h>

typedef struct stru_node {
        struct stru_node *next;
        int v;
    } node;

/* remove the first element of the list and return the new head */
node * pop(node * head);
/* add integer e as the first element of the list and return the new head */
node * push(node * head, int e);
/* frees all memory associated with the list and returns NULL */
node * destroy(node * head);
/* print the elements of the integers in the list, one per line */
void print(node * head);

int main()
{
    /* code */
    return 0;
}

node * pop(node * head) {
    node * curr = head;
    head = curr->next;

    free(curr);
    return head;
}

node * push(node * head, int e) {
    node * curr;
    curr = (node *)malloc(sizeof node);

    curr->next = head;
    curr->v = e;

    return curr;
}

node * destroy(node * head) {
    node * curr = head;

    while (curr != NULL)
    {
        curr = head;
        free(curr);
        head = head->next;
    }
    
    return NULL;
}
