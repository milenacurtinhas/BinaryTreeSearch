#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_tree.h"

//                        -----KEY/VALUE PAIR FUNCTIONS-----
KeyValPair *key_val_pair_construct(void *key, void *val){
    KeyValPair *kvp = malloc(sizeof(KeyValPair));

    kvp->key = key;
    kvp->value = val;

    return kvp;
}

void key_val_pair_destroy(KeyValPair *kvp){
    free(kvp);
}

//                        -----NODE FUNCTIONS-----
Node *node_construct(void *key, void *value, Node *left, Node *right, Node *parent){
    Node *n = malloc(sizeof(Node));

    n->key = key;
    n->value = value;
    n->left = left;
    n->right = right;
    n->parent = parent;

    return n;
}

Node *_add_recursive(BinaryTree *bt, Node *node, void *key, void *value) {
    if(node == NULL)
        return node_construct(key, value, NULL, NULL, NULL);

    if(bt->cmp_fn(key, node->key) < 0){
        node->left = _add_recursive(bt, node->left, key, value);
        node->left->parent = node;
    }
    else if(bt->cmp_fn(key, node->key) > 0){
        node->right = _add_recursive(bt, node->right, key, value);
        node->right->parent = node;
    }
    else if(bt->cmp_fn(key, node->key) == 0){
        void *old_val = node->value;
        node->value = value;
        bt->val_destroy_fn(old_val);
        bt->key_destroy_fn(key);
    }
    
    return node;
}

void node_destroy(Node *node){
    free(node);
}

//                        -----BINARY TREE FUNCTIONS-----
BinaryTree *binary_tree_construct(CmpFn cmp_fn, KeyDestroyFn key_destroy_fn, ValDestroyFn val_destroy_fn){
    BinaryTree *bt = malloc(sizeof(BinaryTree));

    bt->root = NULL;
    bt->cmp_fn = cmp_fn;
    bt->key_destroy_fn = key_destroy_fn;
    bt->val_destroy_fn = val_destroy_fn;

    return bt;
}

void binary_tree_add(BinaryTree *bt, void *key, void *value){
    bt->root = _add_recursive(bt, bt->root, key, value);
}

int binary_tree_empty(BinaryTree *bt){
    if(bt->root)
        return 0;
    else
        return 1;
}

KeyValPair *binary_tree_min(BinaryTree *bt){
    Node *n = bt->root;

    while(n->left){
        n = n->left;
    }

    KeyValPair *kvp = key_val_pair_construct(n->key, n->value);

    return kvp;
}

KeyValPair *binary_tree_max(BinaryTree *bt){
    Node *n = bt->root;

    while(n->right){
        n = n->right;
    }

    KeyValPair *kvp = key_val_pair_construct(n->key, n->value);

    return kvp;
}

void *binary_tree_get(BinaryTree *bt, void *key){
    Node *x = bt->root;

    while(x){
        if(bt->cmp_fn(key, x->key) == 0)
            return x->value;
        else if(bt->cmp_fn(key, x->key) < 0)
            x = x->left;
        else
            x = x->right;
    }

    return NULL;
}

void binary_tree_destroy(BinaryTree *bt){
    Node *n = bt->root;
    while(n){
        if(n->left)
            n = n->left;
        else if(n->right)
            n = n->right;
        else{
            Node *p = n->parent;
            
            if(p){
                if(p->left == n)
                    p->left = NULL;
                else
                    p->right = NULL;
            }
            
            bt->key_destroy_fn(n->key);
            bt->val_destroy_fn(n->value);
            node_destroy(n);
            n = p;
        }
    }

    free(bt);
}

//                        -----BINARY TREE FUNCTIONS-----
