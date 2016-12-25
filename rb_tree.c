#include "rb_tree.h"

typedef struct Node {
  int key;
  int red;
  Node *left;
  Node *right;
  struct Node *link[2];
} Node;

static Node *NewNode(int key) {
  Node *new_node;
  new_node = (Node *) malloc(sizeof(Node));
  new_node->key = key;
  new_node->red = 1;  // 1 - red, 0 - black
  new_node->link[0] = new_node->link[1] = NULL;
  return new_node;
}

static void PrintNode(Node *root) {
  printf("Key: %d", root->key);
}

static int IsRed(Node *root) {
  return root != NULL && root->red == 1;
}

static Node *SingleRotation(Node *root, int dir) {
  Node *save = root->link[!dir];

  root->link[!dir] = save->link[dir];
  save->link[dir] = root;

  root->red = 1;
  save->red = 0;

  return save;
}

static Node *DoubleRotation(Node *root, int dir) {
  root->link[!dir] = SingleRotation(root->link[!dir], !dir);

  return SingleRotation(root, dir);
}

Node *InsertNode(Node *root, int key) {
  // empty tree case
  if (root == NULL) {
    root = NewNode(key);
  } else {
    Node head = {0};  // false root

    Node *g, *t;  // grandparent & parent
    Node *p, *q;  // iterator & parent
    int dir = 0, last;

    // Set up helpers
    t = &head;
    g = p = NULL;
    q = t->link[1] = root;

    /* Search down the tree */
    for (;;) {
      if (q == NULL) {
        /* Insert new node at the bottom */
        p->link[dir] = q = NewNode(key);

        if (q == NULL) {
          return 0;
        }
      } else if (IsRed(q->link[0]) && IsRed(q->link[1])) {
        /* Color flip */
        q->red = 1;
        q->link[0]->red = 0;
        q->link[1]->red = 0;
      }

      // fix red violation
      if (IsRed(q) && IsRed(p)) {
        int dir2 = t->link[1] == g;

        if (q == p->link[last]) {
          t->link[dir2] = SingleRotation(g, !last);
        } else {
          t->link[dir2] = DoubleRotation(g, !last);
        }
      }

      // stop if found
      if (q->key == key) {
        break;
      }

      last = dir;
      dir = q->key < key;

      // update helpers
      if (g != NULL) {
        t = g;
      }

      g = p, p = q;
      q = q->link[dir];
    }

    // update root
    root = head.link[1];
  }

  // make root black
  root->red = 0;

  return root;
}

Node *DeleteNode(Node *root, int key) {
  if (root != NULL) {
    Node head = { 0 };  // false tree root
    Node *q, *p, *g;  // helpers
    Node *f = NULL;  // found item
    int dir = 1;

    // set up helpers
    q = &head;
    g = p = NULL;
    q->link[1] = root;

    // search and push a red down
    while (q->link[dir] != NULL) {
      int last = dir;

      // update helpers
      g = p, p = q;
      q = q->link[dir];
      dir = q->key < key;

      // save found node
      if (q->key == key) {
        f = q;
      }

      // push the red node down
      if (!IsRed(q) && !IsRed(q->link[dir])) {
        if (IsRed(q->link[!dir])) {
          p = p->link[last] = SingleRotation(q, dir);
        } else if (!IsRed(q->link[!dir])) {
          Node *s = p->link[!last];

          if (s != NULL) {
            if (!IsRed(s->link[!last]) && !IsRed(s->link[last])) {
              // color flip
              p->red = 0;
              s->red = 1;
              q->red = 1;
            } else {
              int dir2 = g->link[1] == p;

              if (IsRed(s->link[last])) {
                g->link[dir2] = DoubleRotation(p, last);
              } else if (IsRed(s->link[!last])) {
                g->link[dir2] = SingleRotation(p, last);
              }

              // ensure correct coloring
              q->red = g->link[dir2]->red = 1;
              g->link[dir2]->link[0]->red = 0;
              g->link[dir2]->link[1]->red = 0;
            }
          }
        }
      }
    }

    // replace and remove if found
    if (f != NULL) {
      f->key = q->key;
      p->link[p->link[1] == q] = q->link[q->link[0] == NULL];
      free(q);
    }

    // update the root and make it black
    root = head.link[1];

    if (root != NULL) {
      root->red = 0;
    }
  }

  return root;
}

void DeleteTree(Node *root) {
  if (root != NULL) {
    DeleteTree(root->left);
    DeleteTree(root->right);
    free(root);
  }
}

void Find(Node *root, int key) {
  if (root == NULL) {
    puts("No such element\n");
    return;
  }
  if (key < root->key)
    Find(root->left, key);
  else if (key > root->key)
    Find(root->right, key);
  else
    PrintNode(root);
}

void PrintInOrder(Node *root) {
  if (root) {
    PrintInOrder(root->link[0]);
    printf("%d  ", root->key);
    PrintInOrder(root->link[1]);
  }
}
