#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Creaza structura avl-ului */
Avl_tree *avl_create(int (*cmp)(void *, void *)) {
  Avl_tree *tree;
  if (!cmp) {
    return NULL;
  }
  tree = malloc(sizeof(Avl_tree));
  if (tree == NULL) {
    return NULL;
  }
  tree->compar = cmp;
  tree->root = NULL;
  return tree;
}

void node_free(Node **node) {
  Node *temp;
  if (node == NULL || *node == NULL) {
    return;
  }

  if ((*node)->left != NULL) {
    node_free(&(*node)->left);
  }
  if ((*node)->right != NULL) {
    node_free(&(*node)->right);
  }
  temp = (*node);
  if ((*node)->left == NULL && (*node)->right == NULL) {
    free(temp->data);
    (*node) = NULL;
    free(temp);
    return;
  }
  *node = NULL;
}

void avl_free(Avl_tree *tree) {
  if (tree == NULL) {
    return;
  }

  if (tree->root != NULL) {
    node_free(&tree->root);
  }
  free(tree);
}

int max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

/* Creaza un nod
 * @param1: Valoarea ce trebuie pusa in nod.
 * @param2: Numarul de octeti pe care scrie valoarea.
 */
Node *node_create(void *value, int data_size) {
  Node *node;
  int i;
  if (!value || data_size < 1) {
    return NULL;
  }
  node = malloc(sizeof(Node));
  if (node == NULL) {
    return NULL;
  }
  node->data = malloc(data_size);
  if (node->data == NULL) {
    free(node);
    return NULL;
  }

  /* Copiere octet cu octet din value, in nodul curent. */
  /* OBS: char are 1 octet. */
  for (i = 0; i < data_size; ++i) {
    *(char *)(node->data + i) = *(char *)(value + i);
  }

  /* Nodurile frunze au inaltimea 0. */
  node->height = 0;
  node->left = NULL;
  node->right = NULL;

  return node;
}

/* Nodurile NULL au inaltimea -1 pentru a respecta regula: */
/* node->height = 1 + max(node->left->height, nod->right->height) */
int height(Node *node) {
  if (node == NULL) {
    return -1;
  }
  return node->height;
}

int node_nr_of_sons(Node *node) {
  if (node == NULL) {
    return -1;
  }

  return (node->left != NULL) + (node->right != NULL);
}

/* Rotire dreapta
 * Pentru a nu fi nevoie sa mentinem pointer catre nodul parinte,
 * se vor folosi pointeri la noduri
 *
 *      node  			    lson
 *     /    \			   /    \
 *   lson    y    --->    x    node
 *   /  \     		           /   \
 *  x   lrson                lrson  y
 */
void rotate_right(Node **node) {
  Node *lson, *lrson, *rson;
  if (node == NULL || *node == NULL) {
    return;
  }

  lson = (*node)->left;
  lrson = lson->right;
  rson = (*node);

  (*node) = lson;
  (*node)->right = rson;
  (*node)->right->left = lrson;

  /* Restabilire intaltimi */
  (*node)->right->height =
      1 + max(height((*node)->right->left), height((*node)->right->right));
  (*node)->height = 1 + max(height((*node)->left), height((*node)->right));
}

/* Rotire stanga
 * Pentru a nu fi nevoie sa mentinem pointer catre nodul parinte,
 * se vor folosi pointeri la noduri
 *
 *     node  			    rson
 *    /    \			   /    \
 *   x     rson    --->  node    y
 *         /   \		 /   \
 *       rlson  y       x   rlson
 */
void rotate_left(Node **node) {
  Node *rson, *rlson, *lson;
  if (node == NULL || *node == NULL) {
    return;
  }

  rson = (*node)->right;
  rlson = rson->left;
  lson = (*node);

  (*node) = rson;
  (*node)->left = lson;
  (*node)->left->right = rlson;

  /* Restabilire inaltimi */
  (*node)->left->height =
      1 + max(height((*node)->left->left), height((*node)->left->right));
  (*node)->height = 1 + max(height((*node)->left), height((*node)->right));
}

/* Echilibrarea AVL-ului */
void avl_fix(Node **node) {
  if (!node || !(*node)) {
    return;
  }

  if (height((*node)->left) > height((*node)->right) + 1 &&
      height((*node)->left->left) >= height((*node)->left->right)) {
    /* Rotatie RR
     *
     *     node                  a
     *     /                    / \
     *    a          --->      b   node
     *   /
     *  b
     *
     */
    rotate_right(node);

  } else if (height((*node)->right) > height((*node)->left) + 1 &&
             height((*node)->right->right) >= height((*node)->right->left)) {
    /* Rotatie LL
     *
     *     node                a
     *        \               / \
     *         a     --->  node  b
     *          \
     *           b
     *
     */
    rotate_left(node);

  } else if (height((*node)->left) > height((*node)->right) + 1 &&
             height((*node)->left->left) < height((*node)->left->right)) {
    /* Rotatie LR
     *
     *     node                node               b
     *     /        left       /      right      / \
     *    a         --->      b        --->     a   node
     *     \                 /
     *      b               a
     *
     */
    rotate_left(&(*node)->left);
    rotate_right(node);

  } else if (height((*node)->right) > height((*node)->left) + 1 &&
             height((*node)->right->right) < height((*node)->right->left)) {

    /* Rotatie RL
     *
     *     node                node                  b
     *        \      right        \       left      / \
     *         a      --->         b       --->   node a
     *        /                     \
     *       b                       a
     *
     */
    rotate_right(&(*node)->right);
    rotate_left(node);
  }
}

/* Inserare in AVL
 *
 * @param1: Nodul radacina al subarborelui din parcurgerea recursiva.
 * @param2: Valoare de adaugat in AVL.
 * @param3: Numarul de octeti pe care se scrie valoarea.
 * @param4: Functia de comparare pentru datele din AVL.
 */
void avl_insert(Node **node, void *value, int data_size,
                int (*compar)(void *, void *)) {
  if (!node || !value || !data_size || !compar) {
    return;
  }
  if (*node == NULL) {
    *node = node_create(value, data_size);
    return;
  }
  if (compar(value, (*node)->data) < 0) {
    avl_insert(&((*node)->left), value, data_size, compar);
  } else if (compar(value, (*node)->data) > 0) {
    avl_insert(&((*node)->right), value, data_size, compar);
  }
  else {
    /* nu inseram valori deja existente in arbore */
  }

  (*node)->height = 1 + max(height((*node)->left), height((*node)->right));
  avl_fix(node);
}

void *get_key(Node *node, void *value, int data_size,
              int (*compar)(void *, void *)) {
  if (!node || !value || !data_size || !compar) {
    return NULL;
  }
  if (compar(value, node->data) < 0) {
    return get_key(node->left, value, data_size, compar);
  }
  if (compar(value, node->data) > 0) {
    return get_key(node->right, value, data_size, compar);
  }
  return node->data;
}

/* Elementul maxim din subarbore */
Node *max_element(Node *node) {
  if (node == NULL) {
    return NULL;
  }
  if (node->right == NULL) {
    return node;
  }
  return max_element(node->right);
}

/* Stergere din AVL
 *
 * @param1: Nodul radacina al subarborelui din parcurgerea recursiva.
 * @param2: Valoare de adaugat in AVL.
 * @param3: Numarul de octeti pe care se scrie valoarea.
 * @param4: Functia de comparare pentru datele din AVL.
 */
void avl_delete(Node **node, void *value, int data_size,
                int (*compar)(void *, void *)) {
  int i;
  Node *tmp, *max;
  if (!node || !(*node) || !value || !data_size || !compar) {
    return;
  }
  if (compar(value, (*node)->data) < 0) {
    avl_delete(&((*node)->left), value, data_size, compar);
  } else if (compar(value, (*node)->data) > 0) {
    avl_delete(&((*node)->right), value, data_size, compar);
  } else if (!node_nr_of_sons(*node)) {
    node_free(node);
    return;
  } else if (node_nr_of_sons(*node) == 1) {
    tmp = *node;
    if ((*node)->left) {
      *node = (*node)->left;
      tmp->left = NULL;
    }
    else {
      *node = (*node)->right;
      tmp->right = NULL;
    }
    node_free(&tmp);
  } else {
    /* cautam nodul cu valoarea maxima din subarborele stang */
    max = max_element((*node)->left);
    for (i = 0; i < data_size; ++i) {
      *(char *)((*node)->data + i) = *(char *)(max->data + i);
    }

    avl_delete(&((*node)->left), max->data, data_size, compar);

    /*parent_of_max = bst_node;
    max = bst_node->left;
    while (max->right != NULL) {
      parent_of_max = max;
      max = max->right;
    }

    parent_of_max->right = NULL;
    max->left = bst_node->left;
    max->right = bst_node->right;
    bst_node = max;*/
  }

  avl_fix(node);
}
void preorder_traversal(Node *node) {
  if (node == NULL) {
    return;
  }
  printf("%d ", *(int *)node->data);
  preorder_traversal(node->left);
  preorder_traversal(node->right);
}
