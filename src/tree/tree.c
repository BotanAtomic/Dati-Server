//
// Created by Botan on 18/11/18.
//

#include <malloc.h>
#include "comparator.h"

#include "utils.h"
#include "tree.h"
#include "buffer.h"

BinaryTree *createBinaryTree(comparator comparator, converter converter, char *key) {
    BinaryTree *binaryTree = malloc(sizeof(BinaryTree));
    binaryTree->comparator = comparator;
    binaryTree->converter = converter;
    binaryTree->key = key;
    binaryTree->root = NULL;
    return binaryTree;
}

TreeNode *createTreeNode(Node *node) {
    TreeNode *temp = malloc(sizeof(TreeNode));
    temp->left = temp->right = NULL;
    temp->node = node;
    return temp;
}

void searchNode(BinaryTree *binaryTree, void *key, char comparator, void (*callback)(TableValue *, int *), int *args) {
    if (binaryTree->root == NULL) {
        writeUByte(0, *args);
        return;
    }

    int result;
    TreeNode *cursor = binaryTree->root;
    while (cursor != NULL) {
        result = binaryTree->comparator(key, cursor->node->comparable);
        if (result < 0) {
            if (comparator == SUPERIOR && !cursor->node->root->removed)
                callback(cursor->node->root, args);

            cursor = comparator == SUPERIOR ? cursor->right : cursor->left;
        } else if (result > 0) {
            if (comparator == INFERIOR && !cursor->node->root->removed)
                callback(cursor->node->root, args);

            cursor = cursor->right;
        } else {
            if (comparator == EQUAL && !cursor->node->root->removed)
                callback(cursor->node->root, args);

            cursor = comparator == SUPERIOR ? cursor->right : cursor->left;
        }
    }

}


TreeNode *insertTreeNode(BinaryTree *binaryTree, Node *node) {
    TreeNode *root = binaryTree->root;

    if (root == NULL) {
        root = createTreeNode(node);
    } else {
        int isLeft = 0;
        int r = 0;
        TreeNode *cursor = root;
        TreeNode *prev = NULL;

        while (cursor != NULL) {
            r = binaryTree->comparator(node->comparable, cursor->node->comparable);
            prev = cursor;
            if (r <= 0) {
                isLeft = 1;
                cursor = cursor->left;
            } else if (r > 0) {
                isLeft = 0;
                cursor = cursor->right;
            }

        }
        if (isLeft)
            prev->left = createTreeNode(node);
        else
            prev->right = createTreeNode(node);
    }

    binaryTree->root = root;
    return root;
}

TreeNode *deleteTreeNode(BinaryTree *binaryTree, TreeNode *root, void *key) {
    if (root == NULL)
        return NULL;

    TreeNode *cursor;
    int r = binaryTree->comparator(key, root->node->comparable);

    if (r < 0) {
        root->left = deleteTreeNode(binaryTree, root->left, key);
    } else if (r > 0) {
        root->right = deleteTreeNode(binaryTree, root->right, key);
    } else {
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
            TreeNode *parent = NULL;

            while (cursor->left != NULL) {
                parent = cursor;
                cursor = cursor->left;
            }
            root->node = cursor->node;
            if (parent != NULL)
                parent->left = deleteTreeNode(binaryTree, parent->left,
                                              parent->left->node->comparable);
            else
                root->right = deleteTreeNode(binaryTree, root->right,
                                             root->right->node->comparable);
        }

    }

    binaryTree->root = root;
    return root;

}
