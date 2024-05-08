#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if(new==NULL) return NULL;
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * nuevo = createTreeNode(key, value);
    if(tree == NULL || tree->root == NULL) return;

    if(searchTreeMap(tree, key) == NULL){
        TreeNode * aux = tree->root;
        TreeNode * parent = NULL;

        while(aux != NULL){
            parent = aux;
            if(tree->lower_than(key, aux->pair->key) == 1){
                aux = aux->left;
            }
            else{
                aux = aux->left;
            }
        }
        nuevo->parent = parent;
        if(tree->lower_than(key, parent->pair->key) == 1){
            parent->left = nuevo;
        }
        else{
            parent->right = nuevo;
        }
        tree->current = nuevo;
    }
}

TreeNode * minimum(TreeNode * x){
    if(x->left == NULL) return x;
    TreeNode * aux = x->left;
    while(aux->left != NULL)
        {
            aux = aux->left;
        }
    return aux;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode * aux = tree->root;
    TreeNode * parent = NULL;
    void * claveNodo = node->pair->key;
   
    while(parent != NULL) {
        void * claveAux = aux->pair->key;
        if(is_equal(tree, claveAux, claveNodo)==1){
            break;
        } 
        parent = aux;
        if(tree->lower_than(claveAux, claveNodo) == 1) {
            aux = aux->left;
        } else {
            aux = aux->right;
        }
    }
    
    if (aux->left == NULL && aux->right == NULL) {
        if (aux != tree->root) {
            if(parent->left == aux) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
        } else tree->root = NULL;
        free(aux);
    }
    else if(aux->left == NULL || aux->right == NULL) {
        TreeNode* hijo = (aux->left != NULL) ? aux->left : aux->right;;
        if(aux != tree->root) {
            if(parent->left == aux)
                parent->left = hijo;
            else
                parent->right = hijo;
        } else {
            tree->root = hijo;
        }
        free(aux);
    }
    else {
        TreeNode * sucesor = minimum(aux->right);
        void* succKey = sucesor->pair->key;
        void* succData = sucesor->pair->value;
        removeNode(tree, succKey); 
        aux->pair->key = succKey;
        aux->pair->value = succData;
    }
    
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = tree->root;
    while(aux!=NULL){
        if(is_equal(tree,key,aux->pair->key))
        {
            tree->current = aux;
            return aux->pair;
        }
        else if(tree->lower_than(key,aux->pair->key) == 1){
            aux = aux->left;
        }
        else aux = aux->right;
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
