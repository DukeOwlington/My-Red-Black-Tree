#ifndef RB_TREE_H_INCLUDED
#define RB_TREE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;
// TOP-DOWN INSERT
Node *InsertNode(Node *root, int key);
// TOP-DOWN DELETE
Node *DeleteNode(Node *root, int key);
void DeleteTree(Node *root);
// recursive search for a given key in the tree
void Find(Node *root, int key);
// inorder print
void PrintInOrder(Node *root);

#endif
