//
// Created by Botan on 18/11/18.
//

#include <malloc.h>
#include <variable.h>
#include <utils.h>
#include <tree.h>

#include "tree.h"

b_tree *create_binary_tree(comparator comparator) {
    b_tree *b_tree = malloc(sizeof(b_tree));
    b_tree->comparator = comparator;
    return b_tree;
}

tree_node *create_tree_node(node *node) {
    tree_node *temp = malloc(sizeof(tree_node));
    temp->left = temp->right = NULL;
    temp->node = node;
    return temp;
}

node *search_node(b_tree *b_tree, void *key) {
    if (b_tree->root == NULL)
        return NULL;

    int r;
    tree_node *cursor = b_tree->root;
    while (cursor != NULL) {
        r = b_tree->comparator(key, cursor->node->comparable);
        if (r < 0)
            cursor = cursor->left;
        else if (r > 0)
            cursor = cursor->right;
        else
            return cursor->node;
    }
    return NULL;
}

void print_tree(tree_node *root) {
    if (root != NULL) {
        print_tree(root->left);
        if (root->node->type == STRING)
            printf("%s ", root->node->value);
        else
            printf("%d ", root->node->value);

        print_tree(root->right);
    }

}


tree_node *insert_tree_node(b_tree *b_tree, node *node) {
    tree_node *root = b_tree->root;

    if (root == NULL) {
        root = create_tree_node(node);
    } else {
        int is_left = 0;
        int r = 0;
        tree_node *cursor = root;
        tree_node *prev = NULL;

        while (cursor != NULL) {
            r = b_tree->comparator(node->comparable, cursor->node->comparable);
            prev = cursor;
            if (r < 0) {
                is_left = 1;
                cursor = cursor->left;
            } else if (r > 0) {
                is_left = 0;
                cursor = cursor->right;
            }

        }
        if (is_left)
            prev->left = create_tree_node(node);
        else
            prev->right = create_tree_node(node);
    }

    b_tree->root = root;
    return root;
}

tree_node *delete_tree_node(b_tree *b_tree, tree_node *root, void *key) {
    if (root == NULL)
        return NULL;

    tree_node *cursor;
    int r = b_tree->comparator(key, root->node->comparable);

    if (r < 0)
        root->left = delete_tree_node(b_tree, root->left, key);
    else if (r > 0)
        root->right = delete_tree_node(b_tree, root->right, key);
    else {
        if (root->left == NULL) {
            cursor = root->right;
            free(root);
            root = cursor;
        } else if (root->right == NULL) {
            cursor = root->left;
            free(root);
            root = cursor;
        } else {
            cursor = root->right;
            tree_node *parent = NULL;

            while (cursor->left != NULL) {
                parent = cursor;
                cursor = cursor->left;
            }
            root->node = cursor->node;
            if (parent != NULL)
                parent->left = delete_tree_node(b_tree, parent->left, parent->left->node->comparable);
            else
                root->right = delete_tree_node(b_tree, root->right, root->right->node->comparable);
        }

    }

    b_tree->root = root;
    return root;

}
