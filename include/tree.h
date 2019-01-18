//
// Created by Botan on 18/11/18.
//

#ifndef DATI_TREE_H
#define DATI_TREE_H

#include "database.h"

typedef char (*comparator)(void *, void *);

typedef void *(*converter)(char *);

typedef struct TreeNode {
    Node *node;
    struct TreeNode *left, *right;
} TreeNode;

typedef struct BinaryTree {
    char *key;
    TreeNode *root;
    comparator comparator;
    converter converter;
} BinaryTree;

BinaryTree *createBinaryTree(comparator, converter, char *);

TreeNode *createTreeNode(Node *);

void searchNode(BinaryTree *, void *, int);

void printTree(TreeNode *);

TreeNode *insertTreeNode(BinaryTree *, Node *);

TreeNode *deleteTreeNode(BinaryTree *, TreeNode *, void *);

#endif //DATI_TREE_H
