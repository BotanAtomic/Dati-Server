//
// Created by Botan on 18/11/18.
//

#ifndef DATI_TREE_H
#define DATI_TREE_H

#include "node.h"

typedef char (*comparator)(void *, void *);


typedef struct tree_node {
    node *node;
    struct tree_node *left, *right;
} tree_node;

typedef struct b_tree {
    tree_node *root;
    comparator comparator;
} b_tree;

b_tree *create_binary_tree(comparator comparator);

tree_node *create_tree_node(node *node);

node *search_node(b_tree *, void *key);

void print_tree(tree_node *root);

tree_node *insert_tree_node(b_tree *, node *node);

tree_node *delete_tree_node(b_tree *, tree_node*,void *key);

#endif //DATI_TREE_H
