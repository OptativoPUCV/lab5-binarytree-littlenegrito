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
    if(tree->root == NULL || tree == NULL) return;
    
    void * claveNodo = node->pair->key;
    while(aux != node) {
        void * claveAux = aux->pair->key;
        if(tree->lower_than(claveNodo, claveAux)==1) {
            aux = aux->left;
        } else {
            aux = aux->right;
        }
    }
    TreeNode * parent = aux->parent;
    
    if (aux->left == NULL && aux->right == NULL) {
        if(is_equal(tree, parent->left->pair, aux->pair) ==1){
            parent->left = NULL;
            free(aux);
            return;
        }      
        if(is_equal(tree, parent->right->pair, aux->pair) ==1){
            parent->right = NULL;
            free(aux);
            return;
            }
    }
    else if(((aux->left == NULL) && (aux->right != NULL)) || ((aux->right == NULL) && (aux->left!= NULL))) {
        if(is_equal(tree, parent->left->pair, aux->pair) == 1){
            if(aux->left == NULL){
                parent->left = aux->right;
                aux->right->parent = parent;
                free(aux);
                return;
            }
            else{
                parent->right = aux->left;
                aux->left->parent = parent;
                free(aux);
                return;
            }
        }
        else{
            if(aux->left == NULL){
                parent->left = aux->right;
                aux->right->parent = parent;
                free(aux);
                return;
            }
            else{
                parent->right = aux->left;
                aux->left->parent = parent;
                free(aux);
                return;
            }
        }
    }
    else {
        TreeNode * sucesor = minimum(aux->right);
        aux->pair->key = sucesor->pair->key;
        aux->pair->value = sucesor->pair->value;
        removeNode(tree, sucesor); 
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
    TreeNode * aux = tree->root;
    Pair * valor = NULL;
    TreeNode * next = NULL;
    while (aux != NULL) {
        if (tree->lower_than(aux->pair->key, key) == 0) {
            valor = aux->pair;
            break;
        } else if (tree->lower_than(aux->pair->key, key) == 1) {
            next = aux;
            aux = aux->left;
        } else {
            aux = aux->right;
        }
    }
    if (valor== NULL) {
        if (next != NULL) {
            valor = next->pair;
        }
    }

    return valor;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * minimo = minimum(tree->root);
    if(minimo!= NULL) return minimo->pair;
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode * aux = tree->current;
    if(aux->right != NULL) {
        tree->current = minimum(aux->right);
        return tree->current->pair;
    }
    TreeNode * parent = aux->parent;
    while(parent != NULL && aux == parent->right)
    {        
        aux = parent;
        parent = parent->parent;
    }
    tree->current = parent;
    if(parent != NULL) return parent->pair;
    return NULL;
}
