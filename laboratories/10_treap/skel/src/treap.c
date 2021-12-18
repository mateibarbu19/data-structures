#include "treap.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Creeaza structura Treap-ului */
Treap_tree *treap_create(int (*cmp)(void *, void *)) {
  srand(time(NULL));
  Treap_tree *tree = malloc(sizeof(Treap_tree));
  tree->compar = cmp;
  if (tree == NULL) {
    return NULL;
  }
  tree->root = NULL;
  return tree;
}

void node_free(Node **node) {
  if (!(*node)) {
    return;
  }
  node_free(&(*node)->left);
  node_free(&(*node)->right);

  free((*node)->data);
  free(*node);
  *node = NULL;
}

void treap_free(Treap_tree *tree) {
  if (tree->root != NULL) {
    node_free(&tree->root);
  }
  free(tree);
}
/* Creeaza un nod
 * @param1: Valoarea ce trebuie pusa in nod.
 * @param2: Numarul de octeti pe care scrie valoarea.
 */
Node *node_create(void *value, int data_size) {
  Node *node = malloc(sizeof(Node));
  int i;
  if (node == NULL) {
    return NULL;
  }
  node->data = malloc(data_size);

  /* Copiere octet cu octet din value, in nodul curent. */
  for (i = 0; i < data_size; ++i) {
    *(char *)(node->data + i) = *(char *)(value + i);
  }

  /* Nodurile frunze au inaltimea 0. */
  node->priority = rand() % MAX_PRIORITY;
  node->left = NULL;
  node->right = NULL;

  return node;
}

int max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}
/* Nodurile NULL au prioritatea -1 pentru a pastra proprietatea de max-heap. */
int priority(Node *node) {
  if (node == NULL) {
    return -1;
  }
  return node->priority;
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
  Node *lson, *lrson;
  if (node == NULL || *node == NULL) {
    return;
  }
  lson = (*node)->left;
  if (lson != NULL) {
    lrson = lson->right;
    (*node)->left = lrson;
    lson->right = (*node);

    *node = lson;
  }
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
  Node *rson, *rlson;
  if (node == NULL || *node == NULL) {
    return;
  }
  rson = (*node)->right;
  if (rson != NULL) {
    rlson = rson->left;
    (*node)->right = rlson;
    rson->left = (*node);

    *node = rson;
  }
}
/* Inserare in Treap
 *
 * @param1: Nodul radacina al subarborelui din parcurgerea recursiva.
 * @param2: Valoare de adaugat in Treap.
 * @param3: Numarul de octeti pe care se scrie valoarea.
 * @param4: Functia de comparare pentru datele din Treap.
 */
void treap_insert(Node **node, void *value, int data_size,
                  int (*compar)(void *, void *)) {
  if (!node || !value || !data_size || !compar) {
    return;
  }
  if (*node == NULL) {
    *node = node_create(value, data_size);
    return;
  }
  if (compar(value, (*node)->data) < 0) {
    treap_insert(&((*node)->left), value, data_size, compar);
    if (priority((*node)->left) > priority(*node)) {
      rotate_right(node);
    }
  } else if (compar(value, (*node)->data) > 0) {
    treap_insert(&((*node)->right), value, data_size, compar);
    if (priority((*node)->right) > priority(*node)) {
      rotate_left(node);
    }
  }
}

/* Stergere din Treap
 *
 * @param1: Nodul radacina al subarborelui din parcurgerea recursiva.
 * @param2: Valoare de adaugat in Treap.
 * @param3: Numarul de octeti pe care se scrie valoarea.
 * @param4: Functia de comparare pentru datele din Treap.
 */
void treap_delete(Node **node, void *value, int data_size,
                  int (*compar)(void *, void *)) {
  if (!node || !(*node) || !value || !data_size || !compar) {
    return;
  }
  if (compar(value, (*node)->data) < 0) {
    treap_delete(&((*node)->left), value, data_size, compar);
  } else if (compar(value, (*node)->data) > 0) {
    treap_delete(&((*node)->right), value, data_size, compar);
  } else if (!(*node)->left && !(*node)->right) {
    /* de la acest if în jos am găsit valoarea */
    node_free(node);
  } else if (priority((*node)->left) > priority((*node)->right)) {
    rotate_right(node);
    treap_delete(&(*node)->right, value, data_size, compar);
  } else {
    rotate_left(node);
    treap_delete(&(*node)->left, value, data_size, compar);
  }
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

/* Verifica daca un arbore respecta proprietatile unui treap
 *
 * @param1: Nodul curent in parcurgerea recursiva.
 * @param2: Functia de comparare a datelor din fiecare nod.
 * @return: Daca arborele e Treap, vom returna numarul de noduri al arborelui,
 * 			altfel, vom returna o valoare negativa.
 */
int check_treap(Node *node, int (*compar)(void *, void *)) {
  if (node == NULL) {
    return 0;
  } else if (node->left == NULL && node->right == NULL) {
    return 1;
  } else if (node->left == NULL) {
    if (priority(node) >= priority(node->right) &&
        compar(node->data, node->right->data) <= 0) {
      return 1 + check_treap(node->right, compar);
    }
    return INT_MIN;
  } else if (node->right == NULL) {
    if (priority(node) >= priority(node->left) &&
        compar(node->data, node->left->data) >= 0) {
      return 1 + check_treap(node->left, compar);
    }
    printf("%d %d %d\n", priority(node), priority(node->left),
           priority(node->right));
    return INT_MIN;
  } else {
    if (priority(node) >= priority(node->left) &&
        priority(node) >= priority(node->right) &&
        compar(node->data, node->left->data) >= 0 &&
        compar(node->data, node->right->data) <= 0) {
      return 1 + check_treap(node->left, compar) +
             check_treap(node->right, compar);
    }
    printf("%d %d %d\n", priority(node), priority(node->left),
           priority(node->right));
    return INT_MIN;
  }
}
