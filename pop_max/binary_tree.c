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

Node *tree_minimum(Node *node){
    while(node->left != NULL){
        node = node->left;
    }
    return node;
}

void binary_tree_transplant(BinaryTree *bt, Node *u, Node *v){
    if(!u->parent){
        bt->root = v;
    }
    else if(u == u->parent->left){
        u->parent->left = v;
    }
    else{
        u->parent->right = v;
    }
    if(v)
        v->parent = u->parent;   
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

void binary_tree_remove(BinaryTree *bt, void *key){
    Node *z = NULL;
    Node *x = bt->root;

    while(x){
        if(bt->cmp_fn(key, x->key) < 0){
            x = x->left;
        }
        else if(bt->cmp_fn(key, x->key) > 0){
            x = x->right;
        }   
        else
            break;
    }
    if(bt->cmp_fn(key, x->key) == 0){
        if(!x->left)
            binary_tree_transplant(bt, x, x->right);
        else if(!x->right)
            binary_tree_transplant(bt, x, x->left);
        else{
            z = tree_minimum(x->right);
            if(z->parent != x){
                binary_tree_transplant(bt, z, z->right);
                z->right = x->right;
                z->right->parent = z;
            }
            binary_tree_transplant(bt, x, z);
            z->left = x->left;
            z->left->parent = z;
        }
        node_destroy(x);
    }
}

KeyValPair *binary_tree_pop_min(BinaryTree *bt){
    Node *n = bt->root;

    while(n->left){
        n = n->left;
    }

    KeyValPair *kvp = NULL;

    if(n){
        kvp = key_val_pair_construct(n->key, n->value);
        binary_tree_remove(bt, kvp->key);
    }
    
    return kvp;
}

KeyValPair *binary_tree_pop_max(BinaryTree *bt){
    Node *n = bt->root;

    while(n->right){
        n = n->right;
    }

    KeyValPair *kvp = NULL;

    if(n){
        kvp = key_val_pair_construct(n->key, n->value);
        binary_tree_remove(bt, kvp->key);
    }

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
