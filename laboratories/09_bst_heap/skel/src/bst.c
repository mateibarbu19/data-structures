/**
 * SD, 2020
 *
 * Lab #9, BST & Heap
 *
 * Task #1 - Binary Search Tree implementation
 */

#include "bst.h"
#include "utils.h"

static int __bst_node_nr_of_sons(bst_node_t *bst_node) {
  if (bst_node == NULL) {
    return -1;
  }

  return (bst_node->left != NULL) + (bst_node->right != NULL);
}

static bst_node_t *__bst_node_create(char *data) {
  char *rc;
  bst_node_t *bst_node;

  bst_node = malloc(sizeof(bst_node_t));
  DIE(bst_node == NULL, "bst_node malloc");

  bst_node->left = bst_node->right = NULL;

  bst_node->data = calloc(BST_DATA_LEN, sizeof(char));
  DIE(bst_node->data == NULL, "bst_node->data malloc");

  rc = strncpy(bst_node->data, data, BST_DATA_LEN - 1);
  DIE(rc != bst_node->data, "bst_node->data strncpy");

  return bst_node;
}

bst_tree_t *bst_tree_create(int (*cmp_f)(const char *, const char *)) {
  bst_tree_t *bst_tree;

  if (cmp_f == NULL) {
    return NULL;
  }

  bst_tree = malloc(sizeof(bst_tree_t));
  DIE(bst_tree == NULL, "bst_tree malloc");

  bst_tree->root = NULL;
  bst_tree->cmp = cmp_f;

  return bst_tree;
}

void bst_tree_insert(bst_tree_t *bst_tree, char *data) {
  int found, rc;
  bst_node_t *root, *parent, *node;

  if (bst_tree == NULL || data == NULL) {
    return;
  }

  root = bst_tree->root;
  parent = NULL;
  node = __bst_node_create(data);

  if (root == NULL) {
    bst_tree->root = node;
    return;
  }

  found = 0;
  /* ne arata daca am gasit informatia care trebuie inserata deja in arbore */
  do {
    rc = bst_tree->cmp(node->data, root->data);
    parent = root;

    if (rc < 0) {
      root = root->left;
    } else if (rc == 0) {
      found = 1;
    } else {
      root = root->right;
    }
  } while (!found && root != NULL);

  /* in urma buclei de mai sus stim ca trebuie sa legam noul nod de parent */
  if (!found) {
    rc = bst_tree->cmp(node->data, parent->data);

    if (rc < 0) {
      parent->left = node;
    } else {
      parent->right = node;
    }
  }
}

static bst_node_t *__bst_tree_remove(bst_node_t *bst_node, char *data,
                                     int (*f)(const char *, const char *)) {
  int rc;
  bst_node_t *tmp, *max, *parent_of_max;

  if (!bst_node) {
    return NULL;
  }

  rc = f(data, bst_node->data);

  if (rc < 0) {
    bst_node->left = __bst_tree_remove(bst_node->left, data, f);
  } else if (rc > 0) {
    bst_node->right = __bst_tree_remove(bst_node->right, data, f);
  } else {
    tmp = bst_node;
    if (__bst_node_nr_of_sons(bst_node) <= 1) {
      if (bst_node->left != NULL) {
        bst_node = bst_node->left;
      } else {
        bst_node = bst_node->right;
      }
    } else if (__bst_node_nr_of_sons(bst_node) <= 2) {
      /* cautam nodul cu valoarea maxima din subarborele stang */
      parent_of_max = bst_node;
      max = bst_node->left;
      while (max->right != NULL) {
        parent_of_max = max;
        max = max->right;
      }

      parent_of_max->right = NULL;
      max->left = bst_node->left;
      max->right = bst_node->right;
      bst_node = max;
    } else {
      perror("A aparut o eroare in arbore, nu mai este binar!");
    }
    free(tmp->data);
    free(tmp);
  }

  return bst_node;
}

void bst_tree_remove(bst_tree_t *bst_tree, char *data) {
  bst_tree->root = __bst_tree_remove(bst_tree->root, data, bst_tree->cmp);
}

static void __bst_tree_free(bst_node_t *bst_node) {
  if (!bst_node) {
    return;
  }

  __bst_tree_free(bst_node->left);
  __bst_tree_free(bst_node->right);
  free(bst_node->data);
  free(bst_node);
}

void bst_tree_free(bst_tree_t *bst_tree) {
  if (bst_tree == NULL) {
    return;
  }

  __bst_tree_free(bst_tree->root);
  free(bst_tree);
}
