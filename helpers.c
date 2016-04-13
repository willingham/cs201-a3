#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "helpers.h"
#include "node.h"
#include "heap.h"
#include "rbt.h"
#include "array.h"
#include "listNode.h"

int compare(int type, node *a, node *b) // compare nodes based on heap type (max/min)
{

    if (!type) //min heap
    {
        return a->value>b->value;
    } else if (type) //max heap
    {
        return a->value<b->value;
    }
    return 0;
}

int importFile(char* fName, rbt *edgeTree, array *edgeArray, rbt *vertTree, array *vertArray) { // read ints from file into heap
  char* fname = fName;
  FILE *fp;
  char* s;
  int f = 0;
  int t = 0;
  int w = 0;
  int root = 0;
  fp = fopen(fname, "r");
  if (fp == NULL) {
    perror("error\n");
    exit(-1);
  }
  s = readToken(fp);
  if (!feof(fp)) {
    root = atoi(s);
  }
  while (!feof(fp)) {
    f = atoi(s);
    t = atoi(readToken(fp));
    s = readToken(fp);
    if (strcmp(s, ";") == 0) {
      w = 1;
    } else {
        w = atoi(s);
        s = readToken(fp);
    }

    //printf("Edge Read From File: %d, %d, %d\n", f, t, w); // insert into graph

    node *nf = newNode(); //vertex from node
    nf->value = f;
    node *nt = newNode(); //vertex to node
    nt->value = t;

    node *e = newNode();  //edge node
    e->value = w;
    e->from = f;
    e->to = t;

    node *nodeFrom = findRBT(vertTree, nf);
    if (nodeFrom == 0) {
        addRBT(vertTree, nf);
        addArray(vertArray, nf);
        nodeFrom = nf;
    }

    node *nodeTo = findRBT(vertTree, nt);
    if (nodeTo == 0) {
        addRBT(vertTree, nt);
        addArray(vertArray, nt);
        nodeTo = nt;
    }

    node *nodeEdge = findRBT(edgeTree, e);
    if (nodeEdge == 0) {
        addRBT(edgeTree, e);
        addArray(edgeArray, e);
        nodeEdge = e;
    }

    nodeEdge->leftChild = nodeFrom;
    nodeEdge->rightChild = nodeTo;


    // listNode *lnf = newListNode();  // vertex node
    // node *nf = newNode();
    // lnf->value = nf;
    // nf->value = f;
    //
    // listNode *lnt = newListNode(); // vertex node
    // node *nt = newNode();
    // lnt->value = nt;
    // nt->value = t;
    //
    // node *e = newNode();  //edge node
    // e->value = w;
    // e->from = f;
    // e->to = t;
    // listNode *lne = newListNode();
    // lne->value = e;
    //
    // nf = addNodeInSortedOrder(vertList, lnf);
    // nt = addNodeInSortedOrder(vertList, lnt);
    // e = addNodeInSortedOrder(edgeList, lne);
    //
    // e->leftChild = nf;
    // e->rightChild = nt;

    if (!feof(fp)) {
        s = readToken(fp);
    }
  }
  sortArray(edgeArray);
  sortArray(vertArray);
  return root;
}

// node ** getSortedVertexArray(list *l) {
//     heap *h = newHeap();
//     listNode *ln = l->head;
//     while (ln != NULL) {
//         insertItem(h, 0, 0, ln->val);
//         ln = ln->next;
//     }
//     // all vertices should be on heap now
//     int i = 0;
//     int size = h->size;
//     node **vertices = malloc(size * sizeof *vertices);
//     node *n = popHeap(h);
//     while (n != NULL) {
//         vertices[i] = n;
//         n = popHeap(h);
//         i++;
//     }
//     // for (i=0; i < size; i++) {
//     //     printf("vertex %d: %d\n", i+1, vertices[i]->value);
//     // }
//     return vertices;
// }
//
// node ** getSortedEdgeArray(list *l) {
//     heap *h = newHeap();
//     listNode *ln = l->head;
//     while (ln != NULL) {
//         insertNode(h, ln->value);
//         ln = ln->next;
//     }
//     // all vertices should be on heap now
//     int i = 0;
//     int size = h->size;
//     node **edges = malloc(size * sizeof *edges);
//     node *n = popHeap(h);
//     while (n != NULL) {
//         edges[i] = n;
//         n = popHeap(h);
//         i++;
//     }
//     // for (i=0; i < size; i++) {
//     //     printf("edge %d: %d - %d, %d\n", i+1, edges[i]->from, edges[i]->to, edges[i]->value);
//     //
//     // }
//     return edges;
// }

node **nodeListToArray(list *l) {
    int i = 0;
    int size = l->size;
    node **edges = malloc(size * sizeof *edges);
    listNode *ln = l->head;
    while (ln != NULL) {
        edges[i] = ln->value;
        ln = ln->next;
        i++;
    }
    return edges;
}

int rbtEdgeNodeComparator(void *e1, void *e2) {
    node *n1 = e1;
    node *n2 = e2;
    if (n1->from == n2->from && n1->to == n2->to) {
        return 0;
    } else if (n1->from == n2->to && n1->to == n2->from) {
        return 0;
    } else if (n1->from < n2->from) {
        return -1;
    } else {
        return 1;
    }
}

int arrayEdgeNodeComparator(void *e1, void *e2) {
    node *n1 = e1;
    node *n2 = e2;
    if (n1->value == n2->value) {
        return 0;
    } else if (n1->value < n2->value) {
        return -1;
    } else {
        return 1;
    }
}

int listNodeComparator(void *e1, void *e2){
    listNode *n1 = e1;
    listNode *n2 = e2;
    if (n1->val == n2->val) {
        return 0;
    } else if (n1->val < n2->val) {
        return -1;
    } else {
        return 1;
    }
}
