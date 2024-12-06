/*  Transmission Line Topology Calculation and Analysis (TransTopCalc TTC)
 * Library Copyright (C) 1985-2017  Sasan H Ardalan
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    http://www.SiliconDsp.com
 *    Silicon DSP  Corporation
 */

/***********************************************************************
 * General transmission line and propagation modeling
 * program.
 *
 * Wrtitten  1986, Sasan H. Ardalan
 *	Dept. of Electrical and Computer Engineering
 *	NC State University
 *  	Raleigh, NC 27695
 ***********************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "cx.h"
#include "trantopcalc.h"
#define MAXLINE 80
#define SEG_NAME_BEGIN 1000

static int NodeInit(struct node *root_P, FILE *fp);
static BOOLEAN NodeLeaf(struct node *np_P);
static void NodePrint(struct node *root_P, FILE *fp);

struct node *Trc_NewNode(struct node *root_P)
/*
 *  The function NewNode allocates memory for a node,
 *  and returns the node pointer.
 */
{
  struct node *tmp_P;

  tmp_P = (struct node *)calloc(1, sizeof(struct node));
  tmp_P->right_P = (struct node *)NULL;
  tmp_P->left_P = (struct node *)NULL;
  tmp_P->parent_P = root_P;
  strcpy(tmp_P->leftName, "");
  strcpy(tmp_P->rightName, "");
  tmp_P->type = NODE_TYPE_BINARY;
  return (tmp_P);
}

#define RIGHT 1
#define LEFT 0
/*
 * The recursive routine SearchTreeName() traverses the tree
 * and compares the node name with name.
 * If a match is found, then the pointer to the node is returned.
 */
struct node *Trc_InsSearchTreeName(struct node *root_P, char *name) {
  struct node *np_P = (struct node *)NULL;

  if (root_P == NULL)
    return (NULL);
  //    fprintf(stdout,"searching (%s) %s \n", name,root_P->name);

  if (strcmp(root_P->leftName, name) == 0) {
    return (root_P);
  } else if (strcmp(root_P->rightName, name) == 0) {
    return (root_P);
  }

  if (NodeLeaf(root_P) == FALSE) {
    np_P = Trc_InsSearchTreeName(root_P->left_P, name);
    if (np_P == (struct node *)NULL)
      np_P = Trc_InsSearchTreeName(root_P->right_P, name);
  }

  return (np_P);
}

/*
 * The function Insert inserts a new node into the tree.
 */
int Trc_Insert(struct node *np_P, struct node **root_P)
//   struct node *np_P;	/* node we are inserting */
//   struct node **root_P;	/* root_P of the current tree */
{
  int rc;
  struct node *n_P;
  struct node *parent_P;
  int side = 0;
  if (*root_P == (struct node *)NULL) {
    *root_P = np_P;
    return (1);
  }

#ifdef DEBUG
  fprintf(stdout, "Inserting: node =%s\n", np_P->name);
#endif

  // sha     parent_P=InsSearchTreeName(*root_P,np_P->name,&side);
  parent_P = Trc_InsSearchTreeName(*root_P, np_P->name);
  if (parent_P == (struct node *)NULL) {
    fprintf(stdout, "Problem in loading topology: node =%s\n", np_P->name);
    return (0);
  }
  if (strcmp(parent_P->leftName, np_P->name) == 0) {
    parent_P->left_P = np_P;
    np_P->parent_P = parent_P;

#ifdef DEBUG
    fprintf(stdout, "attaching %s to %s left side\n", np_P->name,
            parent_P->name);

#endif

  } else {

    parent_P->right_P = np_P;
    np_P->parent_P = parent_P;

#ifdef DEBUG
    fprintf(stdout, "attaching %s to %s right side\n", np_P->name,
            parent_P->name);
#endif
  }
  return (1);

  return (0);
}

/*
 * The function Insert inserts a new node into the tree.
 */
Trc_Insert2(struct node *np_P, struct node **root_P)
//   struct node *np_P;	/* node we are inserting */
//   struct node **root_P;	/* root_P of the current tree */
{
  int rc;
  struct node *n_P;
  if (*root_P == (struct node *)NULL) {
    *root_P = np_P;
    return (1);
  }
  if ((*root_P)->left_P == (struct node *)NULL) {
    (*root_P)->left_P = np_P;
    return (1);
  }
  if ((*root_P)->right_P == (struct node *)NULL) {
    (*root_P)->right_P = np_P;
    return (1);
  }
  return (0);
}

#if 0
/***************************************/
/* This function returns FALSE if the  */
/* node is full i.e. has both children */
/***************************************/
BOOLEAN NodeFull(np_P)
struct node *np_P;
{
	if ((np_P->left_P == (struct node *) NULL) ||
		np_P->right_P == (struct node *) NULL ) return(FALSE);
	return(TRUE);
}
#endif

/*
 * This function returns TRUE if the node is a leaf
 */
static BOOLEAN NodeLeaf(struct node *np_P) {
  if ((np_P->left_P == (struct node *)NULL) &&
      np_P->right_P == (struct node *)NULL)
    return (TRUE);
  return (FALSE);
}
/*
 * This function returns TRUE if the node is full
 */
static BOOLEAN NodeFull(struct node *np_P) {
  switch (np_P->type) {
  case NODE_TYPE_BINARY:
    if (np_P->left_P && np_P->right_P)
      return (TRUE);
    break;

  case NODE_TYPE_UNARY:
    if (np_P->left_P)
      return (TRUE);
    break;

  case NODE_TYPE_LEAF:
    if ((np_P->left_P == (struct node *)NULL) &&
        np_P->right_P == (struct node *)NULL)
      return (TRUE);
    break;
  }
  return (FALSE);
}
/*
 * This function returns the pointer to the parent node of np_P
 */
static struct node *GetParent(struct node *np_P) {
  struct node *tmp_P;
  tmp_P = np_P->parent_P;
  return (tmp_P);
}

/*
 * Write the node information pointed to by root_P into a file
 */
static void NodePrintNoLoad(struct node *root_P, FILE *fp) {
  char *rightName, *leftName;
  float x, y;
  BOOLEAN term;
  char buf2[64];

  switch (root_P->type) {
  case NODE_TYPE_LEAF:
    rightName = "";
    leftName = "";
    term = TRUE;
    fprintf(fp, "%s %s %s\n", root_P->name, leftName, rightName);

    break;
  case NODE_TYPE_UNARY:
    rightName = "";
    fprintf(fp, "%s %s %s\n", root_P->name, root_P->leftName, rightName);
    break;
  case NODE_TYPE_BINARY:
    fprintf(fp, "%s %s %s\n", root_P->name, root_P->leftName,
            root_P->rightName);

    break;
  }
}

static void StoreTreeNodes(struct node *root_P, FILE *fp) {
  if (root_P == NULL)
    return;
  NodePrintNoLoad(root_P, fp);
  if (NodeLeaf(root_P) == FALSE) {
    StoreTreeNodes(root_P->left_P, fp);
    StoreTreeNodes(root_P->right_P, fp);
  }
}

/*
 * Write the node information pointed to by root_P into a file
 */
static void NodePrintLoad(struct node *root_P, FILE *fp) {
  char *rightName, *leftName;
  float x, y;
  BOOLEAN term;
  char buf2[256];

  switch (root_P->type) {
  case NODE_TYPE_LEAF:
    term = TRUE;
    if (root_P->infinite == 1)
      strcpy(buf2, "open");
    else
      sprintf(buf2, "%e %e", root_P->zLoad.re, root_P->zLoad.im);
    fprintf(fp, "%s  %s %f %s \n", root_P->name, root_P->typeName,
            root_P->length, buf2);

    break;
  case NODE_TYPE_UNARY:

    fprintf(fp, "%s  %s %f \n", root_P->name, root_P->typeName, root_P->length);
    break;
  case NODE_TYPE_BINARY:
    fprintf(fp, "%s  %s %f \n", root_P->name, root_P->typeName, root_P->length);

    break;
  }
}

static void StoreTreeLoads(struct node *root_P, FILE *fp) {
  if (root_P == NULL)
    return;
  NodePrintLoad(root_P, fp);
  if (NodeLeaf(root_P) == FALSE) {
    StoreTreeLoads(root_P->left_P, fp);
    StoreTreeLoads(root_P->right_P, fp);
  }
}
/*
 * This recursive function stores the tree into a file through the
 * pointer, fp.
 */

void Trc_StoreTree(struct node *root_P, FILE *fp) {
  if (root_P == NULL)
    return;
  StoreTreeNodes(root_P, fp);
  fprintf(fp, "end\n");
  StoreTreeLoads(root_P, fp);
}

/*
 * Write the node information pointed to by root_P into a file
 */
static void NodePrint(struct node *root_P, FILE *fp) {
  char *rightName, *leftName;
  float x, y;
  BOOLEAN term;
  char buf2[64];

  switch (root_P->type) {
  case NODE_TYPE_LEAF:
    rightName = "end";
    leftName = "end";
    term = TRUE;
    if (root_P->infinite == 1)
      strcpy(buf2, "open");
    else
      sprintf(buf2, "%e %e", root_P->zLoad.re, root_P->zLoad.im);
    fprintf(fp, "%s %s %s %s %f %s \n", root_P->name, leftName, rightName,
            root_P->typeName, root_P->length, buf2);

    break;
  case NODE_TYPE_UNARY:
    rightName = "end";
    fprintf(fp, "%s %s %s %s %f \n", root_P->name, root_P->leftName, rightName,
            root_P->typeName, root_P->length);
    break;
  case NODE_TYPE_BINARY:
    fprintf(fp, "%s %s %s %s %f \n", root_P->name, root_P->leftName,
            root_P->rightName, root_P->typeName, root_P->length);

    break;
  }
}

static struct node *RecallNewNode()
/* The function NewNode allocates memory for a node.
 */
{
  struct node *tmp;

  tmp = (struct node *)calloc(1, sizeof(struct node));
  tmp->right_P = (struct node *)NULL;
  tmp->left_P = (struct node *)NULL;
  return (tmp);
}

/*
 * The recursive function RecoverMem() traverses the tree and
 * deallocates memory taken up by nodes.
 * Thus this function frees up memory when a tree is no longer needed
 */

void Trc_RecoverMem(struct node *root_P) {
  if (root_P == NULL)
    return;
  if (NodeLeaf(root_P) == FALSE) {
    Trc_RecoverMem(root_P->left_P);
    Trc_RecoverMem(root_P->right_P);
  }
  free((struct node *)root_P);
}

/*
 * The recursive routine SearchTreeName() traverses the tree
 * and compares the node name with name.
 * If a match is found, then the pointer to the node is returned.
 */
struct node *Trc_SearchTreeName(struct node *root_P, char *name) {
  struct node *np_P = (struct node *)NULL;

  if (root_P == NULL)
    return (NULL);

  if (NodeLeaf(root_P) == FALSE) {
    np_P = Trc_SearchTreeName(root_P->left_P, name);
    if (np_P == (struct node *)NULL)
      np_P = Trc_SearchTreeName(root_P->right_P, name);
  }
  if (strcmp(root_P->name, name) == 0) {
    np_P = root_P;
  }
  return (np_P);
}

struct node *Trc_RecallTree(FILE *fp)
/* fp is pointer to topology data */

/* The function recall_tree reads the topology file and creates the
 * tree structure.
 * The topology contains the following information:
 * 	length
 *	parent node name (character string)
 *      left node name  (character string)
 *      right node name  (character string)
 * If the node has no branches then
 * must be followed by end as the left node name and the right
 * node name and the load must be specified. The load must
 * be a complex value or the word "open" if the branch is
 * open.
 */
{
  struct node *current_P;
  struct node *root_P = (struct node *)NULL;
  char buf[MAXLINE + 10];
  float xin;
  float yin;
  float x, y;

  BOOLEAN term;
  BOOLEAN recall_error;
  struct node *tmp;
  int np;
  int args;
  char st[100];
  int done = FALSE;

  do {
    if (fgets(buf, MAXLINE, fp) == NULL) {
      fprintf(stdout, "End of file error in recall \n");
      recall_error = TRUE;
      return (root_P);
    }
    current_P = Trc_NewNode(root_P);

#ifdef DEBUG
    fprintf(stdout, "Read=%s \n", buf);
#endif

    /*
     * read a line
     */
    args = sscanf(buf, "%s%s%s", st, st, st);
    if (args == 3) {
      sscanf(buf, "%s%s%s", current_P->name, current_P->leftName,
             current_P->rightName);
      current_P->type = NODE_TYPE_BINARY;
    } else if (args == 1) {
      sscanf(buf, "%s", current_P->name);
      current_P->type = NODE_TYPE_LEAF;
      if (strcmp(current_P->name, "end") == 0)
        done = TRUE;
    } else {
      sscanf(buf, "%s%s", current_P->name, current_P->leftName);
      current_P->type = NODE_TYPE_UNARY;
    }
#ifdef DEBUG
    fprintf(stdout, "Before insert  current_P->name=%s \n", current_P->name);
#endif

    if (!done) {
      /*
       * insert node into the tree
       */
      if (Trc_Insert(current_P, &root_P) == 0) {
#ifdef DEBUG
        fprintf(stdout, "Insertion of node \"%s\" failed.\n", current_P->name);
#endif
      }
#ifdef DEBUG
      fprintf(stdout, "After insert root_P->name=%s current_P->name=%s \n",
              root_P->name, current_P->name);
#endif
    }

#if 0000

    if (current_P->type == NODE_TYPE_LEAF) {

      while ((NodeFull(root_P) == TRUE) &&
             (root_P->parent_P != (struct node *)NULL))

        root_P = GetParent(root_P);
    }

    else {
      root_P = current_P;
    }

#endif
  } while (done == FALSE);
  /*
   * Now read in all values into nodes
   */
  if (NodeInit(root_P, fp))
    return (NULL);

  return (root_P);
}

struct node *Trc_RecallTreeOrig(FILE *fp)
/* fp is pointer to topology data */

/* The function recall_tree reads the topology file and creates the
 * tree structure.
 * The topology contains the following information:
 * 	length
 *	parent node name (character string)
 *      left node name  (character string)
 *      right node name  (character string)
 * If the node has no branches then
 * must be followed by end as the left node name and the right
 * node name and the load must be specified. The load must
 * be a complex value or the word "open" if the branch is
 * open.
 */
{
  struct node *current_P;
  struct node *root_P = (struct node *)NULL;
  char buf[MAXLINE + 10];
  float xin;
  float yin;
  float x, y;

  BOOLEAN term;
  BOOLEAN recall_error;
  struct node *tmp;
  int np;
  int args;
  char st[100];

  do {
    if (fgets(buf, MAXLINE, fp) == NULL) {
      fprintf(stderr, "End of file error in recall \n");
      recall_error = TRUE;
      return (root_P);
    }
    current_P = Trc_NewNode(root_P);
    /*
     * read a line
     */
    args = sscanf(buf, "%s%s%s", st, st, st);
    if (args == 3) {
      sscanf(buf, "%s%s%s", current_P->name, current_P->leftName,
             current_P->rightName);
      current_P->type = NODE_TYPE_BINARY;
    } else if (args == 1) {
      sscanf(buf, "%s", current_P->name);
      current_P->type = NODE_TYPE_LEAF;
    } else {
      sscanf(buf, "%s%s", current_P->name, current_P->leftName);
      current_P->type = NODE_TYPE_UNARY;
    }

    /*
     * insert node into the tree
     */
    if (Trc_Insert(current_P, &root_P) == 0) {
#ifdef DEBUG
      fprintf(stdout, "Insertion of node \"%s\" failed.\n", current_P->name);
#endif
    }

    if (current_P->type == NODE_TYPE_LEAF) {

      while ((NodeFull(root_P) == TRUE) &&
             (root_P->parent_P != (struct node *)NULL))

        root_P = GetParent(root_P);
    }

    else {
      root_P = current_P;
    }

  } while (
      !((root_P->parent_P == (struct node *)NULL) && NodeFull(root_P) == TRUE));
  /*
   * Now read in all values into nodes
   */
  if (NodeInit(root_P, fp))
    return (NULL);

  return (root_P);
}

/*
 * The routine InitNode() initializes the node values in memory based
 * on the data in the buffer buf depending on whether a parent
 * or termination node is specified.
 */

static int NodeInit(struct node *root_P, FILE *fp)

{
  int i, j;
  BOOLEAN found;
  char buf[MAXLINE + 10];
  char transName[TYPE_NAME_LENGTH], buf1[TYPE_NAME_LENGTH],
      buf2[TYPE_NAME_LENGTH];
  float length;
  struct transType trmTypes_A[NUMBER_TYPES];
  int nt;
  char st[100];
  char nodeName[NODE_NAME_LENGTH];
  char ch;
  int args;
  struct node *current_P;
  /*
   * read information from topology file and store
   * into node structure
   */
  if (Trc_CnLoadTypes(trmTypes_A, &nt))
    return (1);
  while (fgets(buf, MAXLINE, fp) != NULL) {

    args = sscanf(buf, "%s%s%f%s%s", nodeName, transName, &length, buf1, buf2);
    current_P = Trc_SearchTreeName(root_P, nodeName);
    current_P->length = length;
    strcpy(current_P->typeName, transName);
    if (NodeLeaf(current_P)) {
      if (strcmp(buf1, "open") == 0)
        current_P->infinite = 1;
      else {
        sscanf(buf1, "%lf", &current_P->zLoad.re);
        sscanf(buf2, "%lf", &current_P->zLoad.im);
        current_P->infinite = 0;
      }
    }
    found = FALSE;
    for (i = 0; i < nt; i++) {
      if (strcmp(current_P->typeName, trmTypes_A[i].typeName) == 0) {
        current_P->r = trmTypes_A[i].r;
        current_P->l = trmTypes_A[i].l;
        current_P->c = trmTypes_A[i].c;
        current_P->g = trmTypes_A[i].g;
        for (j = 0; j < MAX_PARAMS; j++) {
          current_P->p[j] = trmTypes_A[i].params_A[j];
        }

        found = TRUE;
      }
      if (found == TRUE)
        break;
    }
    if (found == FALSE) {

      ch = current_P->typeName[0];
      if ((ch != 'r') && (ch != 'l') && (ch != 'c') && (ch != 'g') &&
          (ch != 'R') && (ch != 'L') && (ch != 'C') && (ch != 'G')) {

        fprintf(stderr, "Warning: No Match for type\n");
        CsInfo("Warning: No Match for type\n");
      } else {
        current_P->lumped = TRUE;
        Trc_CnGetLumpedValues(current_P);
      }
    }
  }
  return (0);
}
/*
 * The routine CnLoadTypes() reads the file "trans_types.dat"
 * and stores the transmission line r,l,c,g parameters and name
 * into the data structure trmTypes_A.
 */
int Trc_CnLoadTypes(struct transType trmTypes_A[], int *numTypes_P) {
  int nt;
  float r, l, c, g;
  float p[MAX_PARAMS];
  char name[80], buf[256];
  int args;
  int i;
  FILE *fp;
  if ((fp = fopen("trans_types.dat", "r")) == NULL) {
    Trc_Message("No file for transmission line types(trans_types.dat)  exists");
    return (1);
    // exit(1);
  }
  nt = 0;
  while (fgets(buf, sizeof(buf), fp) != NULL) {
    args = sscanf(buf, "%s %f %f %f %f %f %f %f %f %f %f", name, &p[0], &p[1],
                  &p[2], &p[3], &p[4], &p[5], &p[6], &p[7], &p[8], &p[9]);
    strcpy(trmTypes_A[nt].typeName, name);
    args--;

    if (strlen(buf) < 2 || args <= 0)
      continue;

    // fprintf(stdout,"args=%d \n",args);

    for (i = 0; i < args; i++)
      trmTypes_A[nt].params_A[i] = p[i];
    for (i = args; i < MAX_PARAMS; i++)
      trmTypes_A[nt].params_A[i] = 0.0;

    trmTypes_A[nt].r = p[0];
    trmTypes_A[nt].l = p[1];
    trmTypes_A[nt].c = p[2];
    trmTypes_A[nt].g = p[3];
    nt = nt + 1;
  }
  *numTypes_P = nt;
  fclose(fp);
  return (0);
}

/*
 * This function returns TRUE if the node is a leaf
 */
BOOLEAN Trc_NodeLeaf(struct node *np_P) {
  if ((np_P->left_P == (struct node *)NULL) &&
      np_P->right_P == (struct node *)NULL)
    return (TRUE);
  return (FALSE);
}

#if 000 // backup

/*
 * The routine CnLoadTypes() reads the file "trans_types.dat"
 * and stores the transmission line r,l,c,g parameters and name
 * into the data structure trmTypes_A.
 */
CnLoadTypes(struct transType trmTypes_A[], int *numTypes_P) {
  int nt;
  float r, l, c, g;
  char name[80], buf[80];
  FILE *fp;
  if ((fp = fopen("trans_types.dat", "r")) == NULL) {
    Trc_Message("No file for transmission line types(trans_types.dat)  exists");
    exit(1);
  }
  nt = 0;
  while (fgets(buf, sizeof(buf), fp) != NULL) {
    sscanf(buf, "%s %f %f %f %f", name, &r, &l, &c, &g);
    strcpy(trmTypes_A[nt].typeName, name);
    trmTypes_A[nt].r = r;
    trmTypes_A[nt].l = l;
    trmTypes_A[nt].c = c;
    trmTypes_A[nt].g = g;

    trmTypes_A[nt].params_A[0] = r;
    trmTypes_A[nt].params_A[1] = l;
    trmTypes_A[nt].params_A[2] = c;
    trmTypes_A[nt].params_A[3] = g;

    nt = nt + 1;
  }
  *numTypes_P = nt;
  fclose(fp);
}

#endif
