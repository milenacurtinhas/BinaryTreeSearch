#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

typedef int (*CmpFn)(void *, void *);
typedef void (*KeyDestroyFn)(void *);
typedef void (*ValDestroyFn)(void *);

typedef struct Node
{
    void *key;
    void *value;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
    
} Node;

typedef struct
{
    Node *root;
    CmpFn cmp_fn;
    KeyDestroyFn key_destroy_fn;
    ValDestroyFn val_destroy_fn;
} BinaryTree;

Node *node_construct(void *key, void *value, Node *left, Node *right, Node *parent);
Node *_add_recursive(BinaryTree *bt, Node *node, void *key, void *value);
void node_destroy(Node *node);

BinaryTree *binary_tree_construct(CmpFn cmp_fn, KeyDestroyFn key_destroy_fn, ValDestroyFn val_destroy_fn);
void binary_tree_add(BinaryTree *bt, void *key, void *value);
int binary_tree_empty(BinaryTree *bt);
void *binary_tree_get(BinaryTree *bt, void *key);
void binary_tree_destroy(BinaryTree *bt);

#endif